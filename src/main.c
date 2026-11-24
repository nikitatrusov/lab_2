#include "player.h"
#include "map.h"
#include "utils.h"
#include "interactions.h"
#include "render.h"
#include "inventory.h"

int main() {
    int gameState = 0;
    int activeCall;
    srand(time(NULL));

    //Чтобы не съедало кириллицу powershell'ом
    SetConsoleCP(65001);      // Ввод в UTF-8
    SetConsoleOutputCP(65001); // Вывод в UTF-8

    item_catalog_reader();
    character_creation();

    while (gameState == 0) {

        //Меню действий
        printf("\n \n====== Меню =====\n"
               "[0] Выход\n"
               "[1] Инвентарь\n"
               "[2] Время\n"
               "\n \n"
        );

        activeCall = failsafe_sys_numbers("Выбери действие: ");
        
        switch(activeCall) {
            case 0:
                gameState = 1;
                free(Nikra);
                break;
            case 1:
                render_inventory_menu();
                break;
            case 2:
                render_time_menu(&game);
                break;
            case 8:
                printf("Привет, %s", Nikra->player_name);
                character_showcase();
                break;
            default:
                break;
        }
    }
    return 0;
}