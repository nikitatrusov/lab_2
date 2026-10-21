/*
ToDo 1)провести инициализацию всех переменных в топе файла, перепроверить вопрос безопасности ввода, особенно в failsafe_sys_russian
     2)устранить старый техдолг


*/


#include <stdio.h>
#include <windows.h>
#define MAX_INV_SIZE 10         //размер инвентаря
#define MAX_ITEMS_INGAME 10     //количество предметов в проекте
#define MAX_ITEM_NAME_LEN 64    //Длина названия предмета
#define MAX_USER_NAME_LEN 64    //Длина имени пользователя

int failsafe_sys_numbers(const char *message_text);
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes);

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
} Gamestate;


//Список предметов
Item item_names[MAX_ITEMS_INGAME];


//Чтение файла с списком предметов
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

void diary_write(void) {

    FILE *file = fopen("diary.txt", "a");

    if (file == NULL) {
        perror("Не удалось найти файл");
    } else {
        printf("Файл прочитан успешно");
        fputs("Приветики балабол", file);
    }
    fclose(file);
}

//Вывод инвентаря на экран
void inventory_showcase(int *inventory) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]].name);
    };
}

//Вывод индексов предметов на экран
void items_showcase() {
    printf("\n--- ПРЕДМЕТЫ ---\n");
    for (int i = 0; i < MAX_ITEMS_INGAME; i++) {
        printf("ID = %d | Название: ( %s )\n",
               item_names[i].id, 
               item_names[i].name);
    }
}

//Положить предмет в инвентарь
void item_to_inventory(int *inventory) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys_numbers("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        items_showcase();
        item_id=failsafe_sys_numbers("\nВведите индекс предмета: ");
        
        //проверка существования предмета
        if (item_id > -1 && item_id < MAX_ITEMS_INGAME) {
            inventory[slot_id] = item_names[item_id].id;
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
void clean_inventory_slot(int *inventory) {
    
    int slot_id;
    inventory_showcase(inventory);
    slot_id = failsafe_sys_numbers("Введите индекс слота, который хотите очистить: ");

    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        inventory[slot_id] = 0;
    } else {
        printf("Такого слота не существует");
    }
}

void inventory_item_search_via_name(int *inventory) {
    int search_status = 0;
    int amount_of_items = 0;
    char found_item_slots[MAX_INV_SIZE] = {0};
    char searched_item[MAX_ITEM_NAME_LEN * 2];
    failsafe_sys_russian("Введите название предмета, который хочешь найти: ", searched_item, MAX_ITEM_NAME_LEN * 2);

    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (strcmp(searched_item, item_names[inventory[i]].name) == 0) {
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
void favorite_item_showcase(int *inventory) {
    int count[10] = {0};
                
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (inventory[i] != 0) {
            count[inventory[i]]++;
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
void time_showcase(Gamestate game) {
    printf("Текущее время: День %d, %02d:00\n", game.current_day, game.current_hour);   
}


//Функция изменения времени
void time_advance(Gamestate *game) {
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
    
    Gamestate game = {
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
                inventory_showcase(inventory);
                break;
            case 4:
                item_to_inventory(inventory);
                break;
            case 5:
                clean_inventory_slot(inventory);
                break;
            case 6:
                favorite_item_showcase(inventory);
                break;
            case 7:
                inventory_item_search_via_name(inventory);
                break;
            case 8:
                failsafe_sys_russian("Приветствую тебя, фермер. Как тебя зовут?\n", user_name, sizeof(user_name));
                printf("Привет, %s", user_name);
                break;
            case 9:
                diary_write();
                break;
            default:
                break;
        }
    }
    return 0;
}