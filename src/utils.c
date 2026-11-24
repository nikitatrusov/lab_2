#include "utils.h"
#include "player.h"

//Список предметов
Item item_names[MAX_ITEMS_INGAME];

char user_name[MAX_USER_NAME_LEN];
int inventory[MAX_INV_SIZE] = {0};

    
GameState game = {
    .current_hour = 4,
    .current_day = 1,
    .gameState = 0,
    .inventory = {0}
};

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


int randRange(int min, int max) {
    return min + rand() % (max - min +1);
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
    if (hours < 1) return;                                                               //сдвигает в случае неправильного ввода на след строку

    while (hours !=0) {
        if (game->current_hour == 2) {
            printf("Ты не можешь отдыхать ночью, ночью нужно спать\n");
            break;
        }

        game->current_hour++;
        game->current_day += game->current_hour / 24;
        game->current_hour = game->current_hour % 24;
        hours--;
    }
}


void time_advance_formap(GameState *game) {
        game->current_hour++;
        game->current_day += game->current_hour / 24;
        game->current_hour = game->current_hour % 24;
}


//Чтение файла с списком предметов item_catalogue.txt
void item_catalog_reader(void) {

    FILE *file = fopen("../item_catalogue.txt", "r");

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

    FILE *file = fopen("../diary.txt", "a");

    if (file == NULL) {
        perror("Не создать найти файл");
        fclose(file);
    } else {
        printf("Файл найден");
        fprintf(file, "Имя: %s, Сейчас %d часов %2d-го дня\n", Nikra->player_name, game->current_hour, game->current_day);
        for (int i = 0; i < MAX_INV_SIZE; i++) {
            fprintf(file, "Слот %d: [%d] (%s)\n", i, game->inventory[i], item_names[game->inventory[i]].name);
            
        }
    }
    fclose(file);
}


void render_time_menu(GameState *game) {
    printf("\n \n====== Меню =====\n"
           "[0] Выход\n"
           "[1] Посмотреть часы\n"
           "[2] Промотать время(поработать)\n"
           "[3] Лечь спать до утра\n"
               "\n \n"
        );

    int activeCall = ask_state_menu("Выбери действие: \n");

    switch(activeCall) {
        case 0:
            break;
        case 1:
            time_showcase(*game);
            break;
        case 2:
            time_advance(game);
            break;
        case 8:
            time_sleep(game, Nikra);
            break;
        default:
            break;
        }
}


void render_inventory_menu() {
    printf("\n \n====== Меню =====\n"
           "[0] Выход\n"
           "[1] Посмотреть инвентарь\n"
           "[2] Положить предмет в слот\n"
           "[3] Выбросить предмет\n"
           "[4] Показать любимый ресурс\n"
           "[4] Найти предмет в инвентаре\n"
               "\n \n"
        );
}


int ask_state_menu(char *request) {
    int player_action;
    player_action = failsafe_sys_numbers(request);
    return player_action;
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


void time_sleep(GameState *game, Player *Nikra) {
    printf("Доброе утро фермер, за работу!\n");
    game->current_hour = 8;
    game->current_day++;
    Nikra->cur_stamina=Nikra->max_stamina;
}