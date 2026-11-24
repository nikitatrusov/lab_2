#include "render.h"

void render_hud(GameState *game, Player *Nikra, Level *level_test) {
    printf("\nHP: %i/%2i | Выносливость: %d/%2d | Золото: %3i | Этаж: %4i | Время: %02i:00\n", 
            Nikra->cur_health, Nikra->max_health,
            Nikra->cur_stamina, Nikra->max_stamina,
            Nikra->gold,
            level_test->floor_number,
            game->current_hour
        );
}


void render_map(Level *level_test, Player *Nikra) {
    for (int y = 0; y < level_test->height; y++) {
        for ( int x = 0; x < level_test->width; x++) {
            if (x == Nikra->playerX && y == Nikra->playerY)
                putchar('@');
            else
                putchar(level_test->mapLocation[y][x]);
        }
        putchar('\n');
    }
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