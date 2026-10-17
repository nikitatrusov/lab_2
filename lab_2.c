#include <stdio.h>
#include <windows.h>
#define inv_size 10 //в дальнейшем коде используется как верхняя граница 
#define MAX_ITEMS 10 //количество предметов в проекте
#define MAX_NAME_LEN 64 //Длина названия предмета
char items_names[MAX_ITEMS];

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} Item;

//Индексы предметов
const char* item_names[] = {
    "Пусто", //индекс 0
    "Дерево", //индекс 1
    "Камень", //индекс 2
    "Семена", //индекс 3
    "Мотыга", //индекс 4
    "Лопата", //индекс 5
    "Лейка", //индекс 6
    "Зелье роста", //индекс 7
    "Зелье анти-сорняк", //индекс 8
    "Зелье водника", //индекс 9
} ;


void item_catalog_reader(void) {
    FILE *file = fopen("item_catalogue.txt", "r");
    if (file == NULL) {
        printf("Не удалось найти файл");
        items_names[MAX_ITEMS] = 0;
    } else {
        printf("Файл создан успешно");
        fclose(file);
    }
}


//Функция проверки ввода
int failsafe_sys(const char *message_text) {

        int enter_data;
        int result;

        while (1) {
            printf(message_text);   // переделать все через puts(str) !!!!
            // scanf дате 1, если число успешно считано
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
    fgets(user_name, size, stdin);
    user_name[strcspn(user_name, "\n")] = '\0';
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


//Вывод инвентаря на экран
void inventory_showcase(int *inventory) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < inv_size; i++) {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
    };
}


//Вывод списка индексов предметов на экран
void items_showcase(void) {
    printf("\nСписок предметов: \n" 
    "Пусто - 0\n" //индекс 0
    "Дерево - 1\n" //индекс 1
    "Камень - 2\n" //индекс 2
    "Семена - 3\n" //индекс 3
    "Мотыга - 4\n" //индекс 4
    "Лопата - 5\n" //индекс 5
    "Лейка - 6\n" //индекс 6
    "Зелье роста - 7\n" //индекс 7
    "Зелье анти-сорняк - 8\n" //индекс 8
    "Зелье водника - 9\n");
}


//Положить предмет в инвентарь
void item_to_inventory(int *inventory) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < MAX_ITEMS) {
        items_showcase();
        item_id=failsafe_sys("\nВведите индекс предмета: ");
        
        //проверка существования предмета
        if (item_id > -1 && item_id < inv_size) {
            inventory[slot_id] = item_id;
            printf("Вы положили %s в слот %2d", item_names[item_id], slot_id); 
            } 
        else {
            printf("Такого предмета не существует");
        }
        
    }
    else {
        printf("Такого слота не существует");
    }
}


//Любимый предмет в инвентаре
void clean_inventory_slot(int *inventory) {
    
    int clear_slot_id;
    int slot_id;
    clear_slot_id = failsafe_sys("Введите индекс слота, который хотите очистить: ");

    if (slot_id > -1 && slot_id < inv_size) {
        inventory[clear_slot_id] = 0;
    }
}


//Функция показа любимого предмета
void favorite_item_showcase(int *inventory) {
    int count[10] = {0};
                
    for (int i = 0; i < 10; i++) { //Не забыть на уровне компилятора написать size!!!
        if (inventory[i] != 0) {
            count[inventory[i]]++;
        }
    }

    int max_count = 0;
    int fav_item = 0;
    for (int i = 0; i < 10; i++) { //Не забыть на уровне компилятора написать size!!!
        if (count[i] > max_count) {
            max_count = count[i];
            fav_item = i;
        }
    }
    printf("Любимый предмет: %s, он встречается %d раз\n", item_names[fav_item], max_count);
}

int main() {

    int current_day = 1;
    int current_hour = 8;
    int gameState = 0;
    int activeCall;
    char user_name[34];
    int inventory[inv_size] = {0};

    //Чтобы не съедало кириллицу powershell'ом
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    //Заглушка, убрать
    name_asker(user_name, sizeof(user_name));
    printf("%s", user_name);

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
            default:
                break;
        }
    }
    return 0;
}