#include <stdio.h>
#include <windows.h>
int current_day = 1;
int current_hour = 8;
int activeCall;
int gameState = 0;
int inventory[10] = {0};

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
               "[6] 6e6e6e6e\n"
               "[7] лабуба\n"
               "\n \n"
        );

        //Контр-дурак, первый этап
        if (scanf("%d", &activeCall) != 1) {
            while(getchar() != '\n');
            printf("Придурок, введи один из вариантов действия\n");
            continue;
 
        }
        
        //Зачистка буфера если гений ввел 1abc
        while  (getchar() != '\n');       

        // printf("%d\n", activeCall); //затычка для проверки и дебага

        
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
                for (int i = 0; i < 10; i++) {
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
                    printf("Введите индекс предмета:\n");
                    scanf("%d", &item_id);
                    
                    //проверка существования предмета
                    if (item_id > -1 && item_id < 10) {
                        inventory[slot_id] = item_id;
                    }

                    printf("Вы положили %s в слот %2d", item_names[item_id], slot_id); 
                }
                
                break;
            default:
                break;
        }
    }

    return 0;
}




/*
    if (scanf("%d", &activeCall) != 1) {
        while(getchar() != '\n');
            printf("Придурок, введи один из вариантов действия")
         continue;
    }

    Защита от дурака, идея: 
    сканим ввод сканф'ом, если пользователь ввел не число, то идет дальше в цикл getchar, который по одному символу его чистит
    пока не дойдет до некст строки



    //После присвоения нового Call'a пробуем чистить буфер чтобы мусор не копился в буфере
    while  (getchar() != '\n');
*/