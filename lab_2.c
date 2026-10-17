#include <stdio.h>
#include <windows.h>
#define INV_SIZE 10 //размер инвентаря
#define MAX_ITEMS 10 //количество предметов в проекте
#define MAX_NAME_LEN 64 //Длина названия предмета

int failsafe_sys(const char *message_text);

//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} Item;

//Список предметов
Item item_names[MAX_ITEMS];


//Чтение файла с списком предметов
void item_catalog_reader(void) {

    FILE *file = fopen("item_catalogue.txt", "r");

    if (file == NULL) {
        perror("Не удалось найти файл");
    } else {
        printf("Файл прочитан успешно");

        //буферы для считывания данных с item_catalogue.txt
        int temp_id;
        char temp_name[MAX_NAME_LEN];

        while (fscanf(file, "%d\t%63[^\n]", &temp_id, temp_name) == 2) {
            if (temp_id >= 0 && temp_id < MAX_ITEMS) {
                item_names[temp_id].id = temp_id;
                strcpy(item_names[temp_id].name, temp_name);
            }
        }
        fclose(file);
    }
}

//Вывод инвентаря на экран
void inventory_showcase(int *inventory) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < INV_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]].name);
    };
}

//Вывод индексов предметов на экран
void items_showcase() {
    printf("\n--- ПРЕДМЕТЫ ---\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        printf("ID = %d | Название: ( %s )\n",
               item_names[i].id, 
               item_names[i].name);
    }
}

//Положить предмет в инвентарь
void item_to_inventory(int *inventory) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < INV_SIZE) {
        items_showcase();
        item_id=failsafe_sys("\nВведите индекс предмета: ");
        
        //проверка существования предмета
        if (item_id > -1 && item_id < MAX_ITEMS) {
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
    slot_id = failsafe_sys("Введите индекс слота, который хотите очистить: ");

    if (slot_id > -1 && slot_id < INV_SIZE) {
        inventory[slot_id] = 0;
    } else {
        printf("Такого слота не существует");
    }
}

//Функция показа любимого предмета
void favorite_item_showcase(int *inventory) {
    int count[10] = {0};
                
    for (int i = 0; i < INV_SIZE; i++) {
        if (inventory[i] != 0) {
            count[inventory[i]]++;
        }
    }

    int max_count = 0;
    int fav_item = 0;
    for (int i = 0; i < INV_SIZE; i++) {
        if (count[i] > max_count) {
            max_count = count[i];
            fav_item = i;
        }
    }
    printf("Любимый предмет: %s, он встречается %d раз\n", item_names[fav_item].name, max_count);
}


//Функция проверки ввода
int failsafe_sys(const char *message_text) {

        int enter_data;
        int result;

        while (1) {
            printf(message_text);   // переделать все через puts(str) !!!!
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


//Функция запроса имени пользователя
void name_asker(char *user_name, size_t size) {
    printf("Приветствую тебя, фермер. Как тебя зовут?\n");

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); //Дескриптор стандартного ввода(ссылка на консоль), чтобы читать прям оттуда
    wchar_t wbuf[size];                             //буфер из широких символов, чтобы кириллица не ломалась
    DWORD charsRead = 0;                            //сколько в итоге символов прочитала функция

    ReadConsoleW(hStdin, wbuf, size - 1, &charsRead, NULL); //Читает строку из консоли. Откуда читать, куда ложить, макс символов
                                                            //куда записать число прочитанных символов, и NULL
    wbuf[charsRead] = L'\0';                                //ReadConsoleW не ставит в конце \0, поэтому это делаем мы, чтобы wcscspn остановился
    wbuf[wcscspn(wbuf, L"\r\n")] = L'\0';                   //Ставим на место конца строки (\r\n) \0

    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, user_name, (int)size, NULL, NULL); //перевод из unicod'a в UTF-8
}


//Функция показа времени
void time_showcase(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);   
}


//Функция изменения времени
void time_advance(int *hour, int *day) {
    int hours;
    printf("Было %d:00 %2d-го дня, сколько часов ты проведешь за работой:",*hour, *day); //Подстава. так как мне впадлу переписать fail_sys
    hours = failsafe_sys("");                                                            //еще раз, есть этот принтф, который в свою очередь
    if (hours < 0) return;                                                               //сдвигает в случае неправильного ввода на след строку
    *hour += hours;                                                                      //когда нибудь я этим займусь, честно честно
    *day += *hour / 24;
    *hour = *hour % 24;
}



int main() {

    int current_day = 1;
    int current_hour = 8;
    int gameState = 0;
    int activeCall;
    char user_name[34];
    int inventory[INV_SIZE] = {0};

    //Чтобы не съедало кириллицу powershell'ом
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); 

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
               "[7] Ввести имя пользователя\n"
               "\n \n"
        );

        activeCall = failsafe_sys("Выбери действие: ");
        
        switch(activeCall) {
            case 0:
                gameState = 1;
                break;
            case 1:
                time_showcase(current_day, current_hour);
                break;
            case 2:
                time_advance(&current_hour, &current_day);
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
                name_asker(user_name, sizeof(user_name));
                printf("Привет, %s", user_name);
                break;
            default:
                break;
        }
    }
    return 0;
}