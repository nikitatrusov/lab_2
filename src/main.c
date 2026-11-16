#include "player.c"
#include "map.c"
#include "utils.c"

int main() {
    int gameState = 0;
    int activeCall;

    init_player();

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
                free(Nikra);
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
                begin_journey(&level_test, Nikra);
                break;
            default:
                break;
        }
    }
    return 0;
}