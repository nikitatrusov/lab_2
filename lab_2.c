/*
ToDo 1)провести инициализацию всех переменных в топе файла, перепроверить вопрос безопасности ввода, особенно в failsafe_sys_russian
     2)устранить старый техдолг


*/


#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX_INV_SIZE 10         //размер инвентаря
#define MAX_ITEMS_INGAME 10     //количество предметов в проекте
#define MAX_ITEM_NAME_LEN 64    //Длина названия предмета
#define MAX_USER_NAME_LEN 64    //Длина имени пользователя

int failsafe_sys_numbers(const char *message_text);
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes);
int map_decoder(const char *encoded, char *decoded, int cap);

//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;

//Основная структура данных игры
typedef struct {
    int current_hour;
    int current_day;
    int gamestate;
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


//Список предметов
Item item_names[MAX_ITEMS_INGAME];


//Времянка
MapInfo test = {10,5,1,"aboba"};


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
    FILE *input = fopen("Input.txt", "r");
    if (input == NULL) {
        printf("Не удалось открыть файл");
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

        if (first_len == -1) {                 //Сравниваем все строки по длине первой строки. Так что важно в ней не наложать
            first_len = len;
        } else if (len != first_len) {
            printf("Ошибка, строка %d имеет длину %d, согласно первой строке ожидается %d\n", line_number, len, first_len);
            fclose(input);
            return;
        }

        printf("%s\n", decoded);
    }

    FCLOSE(input);
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
void items_showcase() {
    printf("\n--- ПРЕДМЕТЫ ---\n");
    for (int i = 0; i < MAX_ITEMS_INGAME; i++) {
        printf("ID = %d | Название: ( %s )\n", item_names[i].id, item_names[i].name);
    }
}

//Положить предмет в инвентарь
void item_to_inventory(GameState *game) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys_numbers("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        items_showcase();
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
void clean_inventory_slot(GameState *game) {
    
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
    int search_status = 0;
    int amount_of_items = 0;
    char found_item_slots[MAX_INV_SIZE] = {0};
    char searched_item[MAX_ITEM_NAME_LEN * 2];
    failsafe_sys_russian("Введите название предмета, который хочешь найти: ", searched_item, MAX_ITEM_NAME_LEN * 2);

    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (strcmp(searched_item, item_names[game.inventory[i]].name) == 0) {
            amount_of_items ++;
            found_item_slots[amount_of_items] = i;
            search_status ++;
        }
    }
        
    if (search_status != 1) {
        printf("Да, объект %s найден, он находится в слоте/ах: ", searched_item);
        for (int i = 0; i < amount_of_items; i++) {
            printf("%d", found_item_slots[i]);
                if (i < amount_of_items - 1) {
                    printf(", "); 
                } 
        }
                
    } else {
        printf("Нет, предмета %s, у тебя нет\n", searched_item);
    }
}

//Функция показа любимого предмета
void favorite_item_showcase(GameState game) {
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
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); //Дескриптор стандартного ввода(ссылка на консоль), чтобы читать прям оттуда
        wchar_t wbuf[MAX_ITEM_NAME_LEN];                //буфер из широких символов, чтобы кириллица не ломалась
        DWORD charsRead = 0;                            //сколько в итоге символов прочитала функция

        fputs(message_text, stdout);                                    //Безопасно выводим текст
        fflush(stdout);                                                 //Принудительный сброс буфера
        ReadConsoleW(hStdin, wbuf, max_bytes - 1, &charsRead, NULL);    //Читает строку из консоли. Откуда читать, куда ложить, макс символов
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
        .gamestate = 0,
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
                item_to_inventory(&game);
                break;
            case 5:
                clean_inventory_slot(&game);
                break;
            case 6:
                favorite_item_showcase(game);
                break;
            case 7:
                inventory_item_search_via_name(game);
                break;
            case 8:
                failsafe_sys_russian("Приветствую тебя, фермер. Как тебя зовут?\n", user_name, sizeof(user_name));
                printf("Привет, %s", user_name);
                break;
            case 9:
                diary_write(&game);
                char decoded[1000];
                printf("\n");
                read_map();
                break;
            default:
                break;
        }
    }
    return 0;
}