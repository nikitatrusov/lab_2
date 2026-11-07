/*
ToDo 1)Перепроверить вопрос безопасности ввода, особенно в failsafe_sys_russian
     2)устранить старый техдолг
    3)Унифицировать вывод строк на экран через отдельную функцию

*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX_INV_SIZE 10         //размер инвентаря
#define MAX_ITEMS_INGAME 10     //количество предметов в проекте
#define MAX_ITEM_NAME_LEN 64    //Длина названия предмета
#define MAX_USER_NAME_LEN 64    //Длина имени пользователя
#define MAX_FLOORS 10           //Количество этажей    
#define T_chance 5
#define C_chance 5
#define rand_gen_max 100
#define rand_gen_min 0

//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;

//Основная структура данных игры
typedef struct {
    int current_hour;
    int current_day;
    int gameState;
    int activeCall;
    char user_name[MAX_USER_NAME_LEN];
    int inventory[MAX_INV_SIZE];
} GameState;

//Информация о карте
typedef struct {
    int size_x;
    int size_y;
    int size_z;
    char map_name[100]; //Временно для тестов!!!!!
} MapInfo;

typedef struct {
    char **mapLocation;
    int width, height;
    int playerX, playerY;
    int floor_number;
} Level;

Level level_test = {
    .width = 10,
    .height = 10,
    .playerX = 5,
    .playerY = 5,
    .floor_number = 1
};

//Список предметов
Item item_names[MAX_ITEMS_INGAME];


int failsafe_sys_numbers(const char *message_text);
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes);
int randRange(int min, int max);

int map_decoder(const char *encoded, char *decoded, int cap);
void diary_write(GameState *game);
void item_catalog_reader(void);
void read_map(void);
int apply_fan(Level *level, int dx, int dy);

void inventory_showcase(GameState game);
void inventory_item_add(GameState *game);
void inventory_slot_clean(GameState *game);
void inventory_item_search_via_name(GameState game);
void inventory_favorite_item_showcase(GameState game);
void items_index_showcase();

void time_showcase(GameState game);
void time_advance(GameState *game);


int generate_map(Level *level_test) {
    if (level_test->mapLocation != NULL) {
        for (int i = 0; i < level_test->height; i++) {
            free(level_test->mapLocation[i]);
        }
        free(level_test->mapLocation);
        level_test->mapLocation = NULL;
    }

    level_test->mapLocation = (char**)calloc(level_test->height, sizeof(char*));

    if (level_test->mapLocation == NULL) {
        printf("Что- то пошло не так\n");
        return 1;
    }

    for (int i = 0; i < level_test->height; i++) {
        level_test->mapLocation[i] = (char*)calloc(level_test->width, sizeof(char));
    }
    //level_test->mapLocation = calloc(level_test->width * level_test->height, 1);

    for (int y = 0; y < level_test->height; y++) {
        for ( int x = 0; x < level_test->width; x++) {
            if (x == 0 || y == 0 || x == level_test->width-1 || y == level_test->height-1)
                level_test->mapLocation[y][x] = '#';
            else
                level_test->mapLocation[y][x] = '.';
        }
    }

    for (int y = 1; y < level_test->height-1; y++) {
        for ( int x = 1; x < level_test->width-1; x++) {
            int tileId = x + y * level_test->width;
            if (randRange(rand_gen_min, rand_gen_max) < T_chance)
                level_test->mapLocation[y][x] = 'T';
            if (randRange(rand_gen_min, rand_gen_max) < C_chance)
                level_test->mapLocation[y][x] = 'C';
        }
    }

    level_test->mapLocation[level_test->playerY][level_test->playerX] = 'D'; //Лестница на выход

    int ux, uy, tries = 0;
    do {
        ux = 1 + rand() % (level_test->width - 2);
        uy = 1 + rand() % (level_test->height - 2);
        tries++;
    } while (level_test->mapLocation[uy][ux] != '.' && tries < 100);
    level_test->mapLocation[uy][ux] = 'U';


    int fans = 2 + rand() % 3;   // 2..4
    for (int i = 0; i < fans; i++) {
        int fx, fy, tries = 0;
        do {
            fx = 1 + rand() % (level_test->width - 2);
            fy = 1 + rand() % (level_test->height - 2);
            tries++;
        } while (level_test->mapLocation[fy][fx] != '.' && tries < 100);
        if (level_test->mapLocation[fy][fx] != '.') continue;

        const char dirs[] = "><^v";
        level_test->mapLocation[fy][fx] = dirs[rand() % 4];
    }
    return 0;
}





void render_map(Level *level_test) {
    for (int y = 0; y < level_test->height; y++) {
        for ( int x = 0; x < level_test->width; x++) {
            if (x == level_test->playerX && y == level_test->playerY)
                putchar('@');
            else
                putchar(level_test->mapLocation[y][x]);
        }
        putchar('\n');
    }
}


int move_check(Level *level_test, int dx, int dy) {
    int nx = level_test->playerX + dx;
    int ny = level_test->playerY + dy;

    if (nx < 0 || nx >= level_test->width)  return 0;
    if (ny < 0 || ny >= level_test->height) return 0;
    if (level_test->mapLocation[ny][nx] == '#') return 0;
    if (level_test->mapLocation[ny][nx] == 'C') return 0;
    if (level_test->mapLocation[ny][nx] == 'T') return 0;

    level_test->playerX = nx;
    level_test->playerY = ny;
    dx = 0; dy = 0;
    while (apply_fan(level_test, dx, dy) != 0)
        apply_fan(level_test, dx, dy);
    return 1;
}

// 0 — ничего, 1 — поднялся, -1 — спустился, 2 — вышел из логова сатаны
int check_stairs(Level *level) {
    char here = level->mapLocation[level->playerY][level->playerX];

    if (here == 'U') {
        level->playerX = level->width / 2;
        level->playerY = level->height / 2;
        if (generate_map(level) != 0) return -1;
        level->floor_number++;
        return 1;
    }

    if (here == 'D') {
        if (level->floor_number == 1) {
            return 2;   // вышел из башни
        }
        level->floor_number--;
        if (generate_map(level) != 0) return -1;
        // игрок начинает на новой U
        for (int y = 1; y < level->height - 1; y++)
            for (int x = 1; x < level->width - 1; x++)
                if (level->mapLocation[y][x] == 'U') {
                    level->playerX = x;
                    level->playerY = y;
                    return -1;
                }
        return -1;
    }

    return 0;
}

int apply_fan(Level *level, int dx, int dy) {
    char here = level->mapLocation[level->playerY][level->playerX];

    switch (here) {
        case '>': dx = +1; break;
        case '<': dx = -1; break;
        case '^': dy = -1; break;
        case 'v': dy = +1; break;
        default:  return 0;   // не вентилятор
    }


    for (int step = 0; step < 3; step++) {
        int nx = level->playerX + dx;
        int ny = level->playerY + dy;

        if (nx < 0 || nx >= level->width)  break;
        if (ny < 0 || ny >= level->height) break;
        if (level->mapLocation[ny][nx] == '#') return 0;
        if (level->mapLocation[ny][nx] == 'C') return 0;
        if (level->mapLocation[ny][nx] == 'T') return 0;

        level->playerX = nx;
        level->playerY = ny;
    }
    return 1;
}

void begin_journey(Level *level_test) {
    level_test->floor_number = 1;
    level_test->playerX = level_test->width / 2;
    level_test->playerY = level_test->height / 2;

    if (generate_map(level_test) != 0) {
        printf("Не удалось создать карту!!! \n");
        return;
    }

    printf("Этаж %d | wasd (q — выход): \n", level_test->floor_number);
    char line[16];
    while (1) {
        render_map(level_test);
        printf("wasd (q — выход): ");
        if (!fgets(line, sizeof line, stdin)) break;   // EOF / ошибка ввода
        int dx = 0, dy = 0;
        switch (line[0]) {
            case 'q': break;
            case 'w': dy = -1; break;
            case 's': dy = +1; break;
            case 'a': dx = -1; break;
            case 'd': dx = +1; break;
            default: continue;
        }
        if (move_check(level_test, dx, dy)) {
            if (check_stairs(level_test) == 2 && line[0] == 'q') {
                printf("Вы вышли из башни на 1-м этаже.\n");
                return;
            }
        }
    }
}


//Чтение файла с списком предметов item_catalogue.txt
void item_catalog_reader(void) {

    FILE *file = fopen("item_catalogue.txt", "r");

    if (file == NULL) {
        perror("Не удалось найти файл");
    } else {
        printf("Файл прочитан успешно");

        //буферы для считывания данных с item_catalogue.txt
        int temp_id;
        char temp_name[MAX_ITEM_NAME_LEN];

        while (fscanf(file, "%d\t%63[^\n]", &temp_id, temp_name) == 2) {
            if (temp_id >= 0 && temp_id < MAX_ITEMS_INGAME) {
                item_names[temp_id].id = temp_id;
                strcpy(item_names[temp_id].name, temp_name);
            }
        }
        fclose(file);
    }
}

//Запись состояния игры в файл diary.txt
void diary_write(GameState *game) {

    FILE *file = fopen("diary.txt", "a");

    if (file == NULL) {
        perror("Не создать найти файл");
        fclose(file);
    } else {
        printf("Файл найден");
        fprintf(file, "Имя: %s, Сейчас %d часов %2d-го дня\n", game->user_name, game->current_hour, game->current_day);
        for (int i = 0; i < MAX_INV_SIZE; i++) {
            fprintf(file, "Слот %d: [%d] (%s)\n", i, game->inventory[i], item_names[game->inventory[i]].name);
            
        }
    }
    fclose(file);
}

void read_map(void) {
    int safety = 1;
    FILE *input = fopen("Input.txt", "r");
    if (input == NULL) {
        printf("Не удалось открыть файл для получения кодированной карты");
        return;
    }

    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Не удалось открыть файл для записи декодированной карты");
        fclose(input);
        return;       
    }

    char line[1000];
    char decoded[1000];
    int line_number = 0;
    int first_len = -1;

    while (fgets(line, sizeof line, input) != NULL) {
        line_number ++;
        line[strcspn(line, "\r\n")] = '\0';
        int len = map_decoder(line, decoded, sizeof decoded);

        if (first_len == -1) {                 //Проверка: Сравниваем все строки по длине первой строки. Так что важно в ней не наложать
            first_len = len;
        } else if (len != first_len) {
            printf("Ошибка, строка %d имеет длину %d, согласно первой строке ожидается %d\n", line_number, len, first_len);
            safety = 0;
            return;
        } }

    if (safety = 1) {
        rewind(input);                          //Запись: Возвращаемся к чтению input, только теперь, после проверки записываем все в output
        while (fgets(line, sizeof line, input) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            map_decoder(line, decoded, sizeof decoded);
            fprintf(output, "%s\n", decoded);
            }
        
    }

    fclose(output);
    fclose(input);
}

int map_decoder(const char *encoded, char *decoded, int cap) {
    int len = 0, counter = 0;
    for (; *encoded; encoded++) {                                   //Проходимся по каждому символу в оригинальной строке
        if (*encoded >= '0' && *encoded <= '9') {                   //Говорим, что цифры - коды в диапазоне ежду символами '0' и '9'
            counter = counter * 10 + (*encoded - '0');              //'7' - '0' = 55-48 = 7. '7' -> 7. Плюс умножение на 10, если элементов больше 9
        } else {                                                    
            for (int i = 0; i < counter && len < cap - 1; i++) {    //Защита от переполнения. -1 для того чтобы влез \0
                decoded[len++] = *encoded;                          //Запихиваем символ в dcd и 
            }
            counter = 0;
        }
    }
    decoded[len] = '\0';
    return len;
}

//Вывод инвентаря на экран
void inventory_showcase(GameState game) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, game.inventory[i], item_names[game.inventory[i]].name);
    }
}

//Вывод индексов предметов на экран
void items_index_showcase() {
    printf("\n--- ПРЕДМЕТЫ ---\n");
    for (int i = 0; i < MAX_ITEMS_INGAME; i++) {
        printf("ID = %d | Название: ( %s )\n", item_names[i].id, item_names[i].name);
    }
}

//Положить предмет в инвентарь
void inventory_item_add(GameState *game) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys_numbers("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        items_index_showcase();
        item_id=failsafe_sys_numbers("\nВведите индекс предмета: ");
        
        //проверка существования предмета
        if (item_id > -1 && item_id < MAX_ITEMS_INGAME) {
            game->inventory[slot_id] = item_names[item_id].id;
            printf("Вы положили %s в слот %2d", item_names[item_id].name, slot_id); 
            } 
        else {
            printf("Такого предмета не существует");
        }
        
    }
    else {
        printf("Такого слота не существует");
    }
}

//Очистка слота инвентаря
void inventory_slot_clean(GameState *game) {
    
    int slot_id;
    inventory_showcase(*game);
    slot_id = failsafe_sys_numbers("Введите индекс слота, который хотите очистить: ");

    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        game->inventory[slot_id] = 0;
    } else {
        printf("Такого слота не существует");
    }
}

void inventory_item_search_via_name(GameState game) {
    int amount_of_items = 0;

    char found_item_slots[MAX_INV_SIZE] = {0};
    char searched_item[MAX_ITEM_NAME_LEN * 2];

    failsafe_sys_russian("Введите название предмета, который хочешь найти: ", searched_item, MAX_ITEM_NAME_LEN * 2);

    for (int i = 0; i < MAX_INV_SIZE; i++) {
        int item_id = game.inventory[i];

        if (item_id >= 0 && item_id < MAX_ITEMS_INGAME) {
            if (strcmp(searched_item, item_names[item_id].name) == 0) {
                found_item_slots[amount_of_items] = i;
                amount_of_items ++;
            }
        }
    }
        
    if (amount_of_items > 0) {
        printf("Да, объект %s найден, он находится в слоте/ах: ", searched_item);
        for (int i = 0; i < amount_of_items; i++) {
                printf("%d", found_item_slots[i]);
                if (i < amount_of_items - 1) {
                    printf(", "); 
                } 
        }
        printf("\n");
    } else {
        printf("Нет, предмета %s, у тебя нет\n", searched_item);
    }
}

//Функция показа любимого предмета
void inventory_favorite_item_showcase(GameState game) {
    int count[10] = {0};
                
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (game.inventory[i] != 0) {
            count[game.inventory[i]]++;
        }
    }

    int max_count = 0;
    int fav_item = 0;
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (count[i] > max_count) {
            max_count = count[i];
            fav_item = i;
        }
    }
    printf("Любимый предмет: %s, он встречается %d раз\n", item_names[fav_item].name, max_count);
}


int randRange(int min, int max) {
    return min + rand() % (max - min +1);
}


//Функция проверки ввода для числовых значений
int failsafe_sys_numbers(const char *message_text) {

        int enter_data;
        int result;

        while (1) {
            printf("%s", message_text);   // переделать все через puts(str) !!!!
            // scanf дает 1, если число успешно считано
            result = scanf("%d", &enter_data);

            if (result == 1) {
                //Пользователь НЕ дурак, возвращаем число
                while (getchar() != '\n');
                return enter_data;
            } else {
                // Ошибка: введены не цифры (буквы, символы и т.д.)
                printf("Зачем буквы ввел Ара, не делай так дорогой, введи число\n");
                while (getchar() != '\n');
            }
        }
}

//Функция проверки ввода для русского языка
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes) {
        HANDLE h_stdin = GetStdHandle(STD_INPUT_HANDLE); //Дескриптор стандартного ввода(ссылка на консоль), чтобы читать прям оттуда
        wchar_t wbuf[MAX_ITEM_NAME_LEN];                //буфер из широких символов, чтобы кириллица не ломалась
        DWORD charsRead = 0;                            //сколько в итоге символов прочитала функция

        fputs(message_text, stdout);                                    //Безопасно выводим текст
        fflush(stdout);                                                 //Принудительный сброс буфера
        ReadConsoleW(h_stdin, wbuf, max_bytes - 1, &charsRead, NULL);    //Читает строку из консоли. Откуда читать, куда ложить, макс символов
                                                                        //куда записать число прочитанных символов, и NULL
                                                                        //ReadConsoleW не ставит в конце \0, поэтому это делаем мы, чтобы wcscspn остановился
                                                                        //Ставим на место конца строки (\r\n) \0
        wbuf[charsRead] = L'\0';
        wbuf[wcscspn(wbuf, L"\r\n")] = L'\0';

        WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, out_message, (int)max_bytes, NULL, NULL); //WideChar в CP_UTF8, выводим в out_message
}


//Функция показа времени
void time_showcase(GameState game) {
    printf("Текущее время: День %d, %02d:00\n", game.current_day, game.current_hour);   
}


//Функция изменения времени
void time_advance(GameState *game) {
    int hours;
    printf("Было %d:00 %2d-го дня, сколько часов ты проведешь за работой:", game->current_hour, game->current_day); //Подстава. так как мне впадлу переписать fail_sys
    hours = failsafe_sys_numbers("");                                                            //еще раз, есть этот принтф, который в свою очередь
    if (hours < 0) return;                                                               //сдвигает в случае неправильного ввода на след строку
    game->current_hour += hours;                                                                      //когда нибудь я этим займусь, честно честно
    game->current_day += game->current_hour / 24;
    game->current_hour = game->current_hour % 24;
}



int main() {

    int current_day = 1;
    int current_hour = 8;
    int gameState = 0;
    int activeCall;
    char user_name[MAX_USER_NAME_LEN];
    int inventory[MAX_INV_SIZE] = {0};
    
    GameState game = {
        .current_hour = 8,
        .current_day = 1,
        .gameState = 0,
        .user_name = "Незнакомец",
        .inventory = {0}
    };

    //Чтобы не съедало кириллицу powershell'ом
    SetConsoleCP(65001);      // Ввод в UTF-8
    SetConsoleOutputCP(65001); // Вывод в UTF-8

    item_catalog_reader();

    while (gameState == 0) {

        //Меню действий
        printf("\n \n====== Меню =====\n"
               "[0] Выход\n"
               "[1] Посмотреть часы\n"
               "[2] Промотать время(поработать)\n"
               "[3] Посмотреть инвентарь\n"
               "[4] Положить предмет в слот\n"
               "[5] Выбросить предмет\n"
               "[6] Показать любимый ресурс\n"
               "[7] Найти предмет в инвентаре\n"
               "[8] Ввести имя пользователя\n"
               "[9] Записать информацию в дневник\n"
               "[10] Пойти в приключение"
               "\n \n"
        );

        activeCall = failsafe_sys_numbers("Выбери действие: ");
        
        switch(activeCall) {
            case 0:
                gameState = 1;
                break;
            case 1:
                time_showcase(game);
                break;
            case 2:
                time_advance(&game);
                break;
            case 3:
                inventory_showcase(game);
                break;
            case 4:
                inventory_item_add(&game);
                break;
            case 5:
                inventory_slot_clean(&game);
                break;
            case 6:
                inventory_favorite_item_showcase(game);
                break;
            case 7:
                inventory_item_search_via_name(game);
                break;
            case 8:
                failsafe_sys_russian("Приветствую тебя, фермер. Как тебя зовут?\n", user_name, sizeof(user_name));
                printf("Привет, %s", game.user_name);
                break;
            case 9:
                diary_write(&game);
                char decoded[1000];
                printf("\n");
                read_map();
                break;
            case 10:
                begin_journey(&level_test);
                break;
            default:
                break;
        }
    }
    return 0;
}