#include "level_features.h"

// 0 — ничего, 1 — поднялся, -1 — спустился, 2 — вышел из логова сатаны
int check_stairs(Level *level, Player *Nikra, GameState *game) {
    char here = level->mapLocation[Nikra->playerY][Nikra->playerX];

    if (here == 'U') {

        Nikra->cur_stamina = Nikra->cur_stamina-4;
        time_advance_formap(game);

        int reward = BASE_REWARD_MULTIPLIER * level_test.floor_number;
        Nikra->gold += reward;
        printf("Вы получили награду: %d золота\n", reward);

        Nikra->playerX = level->width / 2;
        Nikra->playerY = level->height / 2;
        if (generate_map(level, Nikra) != 0) return -1;
        level->floor_number++;
        return 1;
    }

    if (here == 'D') {
        if (level->floor_number == 1) {
            return 2;   // вышел из башни
        }
        level->floor_number--;
        if (generate_map(level, Nikra) != 0) return -1;
        // игрок начинает на новой U
        for (int y = 1; y < level->height - 1; y++)
            for (int x = 1; x < level->width - 1; x++)
                if (level->mapLocation[y][x] == 'U') {
                    Nikra->playerX = x;
                    Nikra->playerY = y;
                    return -1;
                }
        return -1;
    }

    return 0;
}

int fan_direction(char c, int *dx, int *dy) {
    switch (c) {
        case '>': *dx = +1; break;
        case '<': *dx = -1; break;
        case '^': *dy = -1; break;
        case 'v': *dy = +1; break;
        default:  return 0;   // не вентилятор
    }
}

int apply_fan(Level *level, Player *Nikra) {
    Direction dir = dir_from_fan(level->mapLocation[Nikra->playerY][Nikra->playerX]);
    if (dir == DIR_NONE) return 0;

    int moved = 0;
    int remaining_fly = fly_range;
    int total = 0;

    while (remaining_fly > 0 && total++ < max_flight_range) {
        int nx = Nikra->playerX + DIR_DX[dir];
        int ny = Nikra->playerY + DIR_DY[dir];

        if (nx < 0 || nx >= level->width)  break;
        if (ny < 0 || ny >= level->height) break;
        char c = level->mapLocation[ny][nx];
        if (level->mapLocation[ny][nx] == '#') return 0;
        if (level->mapLocation[ny][nx] == 'C') return 0;
        if (level->mapLocation[ny][nx] == 'T') return 0;

        Nikra->playerX = nx;
        Nikra->playerY = ny;
        moved = 1;
        remaining_fly--;

        Direction next = dir_from_fan(c);
        if (next != DIR_NONE) { dir = next; remaining_fly = fly_range; }
    }
    return moved;
}