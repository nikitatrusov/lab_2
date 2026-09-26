#include <stdio.h>
#include <windows.h>
#define inv_size 10 //в дальнейшем коде используется как верхняя граница 
int current_day = 1;
int current_hour = 8;
int activeCall;
int gameState = 0;
int inventory[inv_size] = {0};
int enter_data;

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

int failsafe_sys() {

        //Захардкоженный failstate
        int enter_data;
        int result;

        while (1) {
            printf("Введите число: ");

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

int main() {

    //Чтобы не съедало кириллицу powershell'ом
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

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

        activeCall = failsafe_sys();
        
        switch(activeCall) {
            case 0:
                gameState = 1;
                break;
            case 1:
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);   
                break;
            case 2:
                //Важно! Использую activeCall（и буду использовать) так как это централизованная переменнаяя ввода данных
                //в которой есть защита от дурня
                printf("Было %d:00 %2d-го дня, сколько часов ты проведешь за работой: ", current_hour, current_day);
                scanf("%d", &activeCall);
                current_hour += activeCall;
                if (current_hour >= 24) {
                    current_day += current_hour / 24;
                    current_hour = current_hour % 24;
                }
                activeCall = 0;
                break;
            case 3:
                printf("--- Инвентарь ---\n");
                for (int i = 0; i < inv_size; i++) {
                    printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
                };
                break;
            case 4:
                int slot_id;
                int item_id;

                printf("Введите индекс слота (0-9):\n");
                scanf("%d", &slot_id);

                //проверка существования слота
                if (slot_id > -1 && slot_id < 10) {
                    
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

                    printf("\nВведите индекс предмета:\n");
                    scanf("%d", &item_id);
                    
                    //проверка существования предмета
                    if (item_id > -1 && item_id < inv_size) {
                        inventory[slot_id] = item_id;
                        printf("Вы положили %s в слот %2d", item_names[item_id], slot_id); 
                    }
                }

                break;
            case 5:
                int clear_slot_id;
                printf("Введите индекс слота, который хотите очистить:\n");
                scanf("%d", &clear_slot_id);

                if (slot_id > -1 && slot_id < inv_size) {
                    inventory[clear_slot_id] = 0;
                }

                break;
            case 6:
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
                break;
            default:
                break;
        }
    }
    return 0;
}