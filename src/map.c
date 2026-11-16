#include "map.h"
#include "player.h"
#include <stdio.h>
#include "player.h"


int generate_map(Level *level_test, Player *dodikjan) {
    if (level_test->mapLocation != NULL) {
        for (int i = 0; i < level_test->height; i++) {
            free(level_test->mapLocation[i]);
        }
        free(level_test->mapLocation);
        level_test->mapLocation = NULL;
    }

    level_test->mapLocation = (char**)calloc(level_test->height, sizeof(char*));

    if (level_test->mapLocation == NULL) {
        printf("Что- то пошло не так\n");
        return 1;
    }

    for (int i = 0; i < level_test->height; i++) {
        level_test->mapLocation[i] = (char*)calloc(level_test->width, sizeof(char));
    }
    //level_test->mapLocation = calloc(level_test->width * level_test->height, 1);

    for (int y = 0; y < level_test->height; y++) {
        for ( int x = 0; x < level_test->width; x++) {
            if (x == 0 || y == 0 || x == level_test->width-1 || y == level_test->height-1)
                level_test->mapLocation[y][x] = '#';
            else
                level_test->mapLocation[y][x] = '.';
        }
    }

    for (int y = 1; y < level_test->height-1; y++) {
        for ( int x = 1; x < level_test->width-1; x++) {
            int tileId = x + y * level_test->width;
            if (randRange(rand_gen_min, rand_gen_max) < T_chance)
                level_test->mapLocation[y][x] = 'T';
            if (randRange(rand_gen_min, rand_gen_max) < C_chance)
                level_test->mapLocation[y][x] = 'C';
        }
    }

    level_test->mapLocation[dodikjan->playerY][dodikjan->playerX] = 'D'; //Лестница на выход

    int ux, uy, tries = 0;
    do {
        ux = 1 + rand() % (level_test->width - 2);
        uy = 1 + rand() % (level_test->height - 2);
        tries++;
    } while (level_test->mapLocation[uy][ux] != '.' && tries < 100);
    level_test->mapLocation[uy][ux] = 'U';


    int fans = 2 + rand() % 3;   // 2..4
    for (int i = 0; i < fans; i++) {
        int fx, fy, tries = 0;
        do {
            fx = 1 + rand() % (level_test->width - 2);
            fy = 1 + rand() % (level_test->height - 2);
            tries++;
        } while (level_test->mapLocation[fy][fx] != '.' && tries < 100);
        if (level_test->mapLocation[fy][fx] != '.') continue;

        const char dirs[] = "><^v";
        level_test->mapLocation[fy][fx] = dirs[rand() % 4];
    }
    return 0;
}





void render_map(Level *level_test, Player *dodikjan) {
    for (int y = 0; y < level_test->height; y++) {
        for ( int x = 0; x < level_test->width; x++) {
            if (x == dodikjan->playerX && y == dodikjan->playerY)
                putchar('@');
            else
                putchar(level_test->mapLocation[y][x]);
        }
        putchar('\n');
    }
}


int move_check(Level *level_test, Direction dir, Player *dodikjan) {
    int nx = dodikjan->playerX + DIR_DX[dir];
    int ny = dodikjan->playerY + DIR_DY[dir];

    if (nx < 0 || nx >= level_test->width)  return 0;
    if (ny < 0 || ny >= level_test->height) return 0;
    if (level_test->mapLocation[ny][nx] == '#') return 0;
    if (level_test->mapLocation[ny][nx] == 'C') return 0;
    if (level_test->mapLocation[ny][nx] == 'T') return 0;

    dodikjan->playerX = nx;
    dodikjan->playerY = ny;
    int guard = 0;
    while (apply_fan(level_test, dodikjan) && guard++ < 15) {}
    return 1;
}

// 0 — ничего, 1 — поднялся, -1 — спустился, 2 — вышел из логова сатаны
int check_stairs(Level *level, Player *dodikjan) {
    char here = level->mapLocation[dodikjan->playerY][dodikjan->playerX];

    if (here == 'U') {
        dodikjan->playerX = level->width / 2;
        dodikjan->playerY = level->height / 2;
        if (generate_map(level, dodikjan) != 0) return -1;
        level->floor_number++;
        return 1;
    }

    if (here == 'D') {
        if (level->floor_number == 1) {
            return 2;   // вышел из башни
        }
        level->floor_number--;
        if (generate_map(level, dodikjan) != 0) return -1;
        // игрок начинает на новой U
        for (int y = 1; y < level->height - 1; y++)
            for (int x = 1; x < level->width - 1; x++)
                if (level->mapLocation[y][x] == 'U') {
                    dodikjan->playerX = x;
                    dodikjan->playerY = y;
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

int apply_fan(Level *level, Player *dodikjan) {
    Direction dir = dir_from_fan(level->mapLocation[dodikjan->playerY][dodikjan->playerX]);
    if (dir == DIR_NONE) return 0;

    int moved = 0;
    int remaining_fly = fly_range;
    int total = 0;

    while (remaining_fly > 0 && total++ < max_flight_range) {
        int nx = dodikjan->playerX + DIR_DX[dir];
        int ny = dodikjan->playerY + DIR_DY[dir];

        if (nx < 0 || nx >= level->width)  break;
        if (ny < 0 || ny >= level->height) break;
        char c = level->mapLocation[ny][nx];
        if (level->mapLocation[ny][nx] == '#') return 0;
        if (level->mapLocation[ny][nx] == 'C') return 0;
        if (level->mapLocation[ny][nx] == 'T') return 0;

        dodikjan->playerX = nx;
        dodikjan->playerY = ny;
        moved = 1;
        remaining_fly--;

        Direction next = dir_from_fan(c);
        if (next != DIR_NONE) { dir = next; remaining_fly = fly_range; }
    }
    return moved;
}

void begin_journey(Level *level_test, Player *dodikjan) {

    level_test->floor_number = 1;
    dodikjan->playerX = level_test->width / 2;
    dodikjan->playerY = level_test->height / 2;

    if (generate_map(level_test, dodikjan) != 0) {
        printf("Не удалось создать карту!!! \n");
        return;
    }

    printf("Этаж %d | wasd (q — выход): \n", level_test->floor_number);
    char line[16];
    while (1) {
        render_map(level_test, dodikjan);
        printf("wasd (q — выход): ");
        if (!fgets(line, sizeof line, stdin)) break;   // EOF / ошибка ввода

        if (line[0] == 'q') {
            if (check_stairs(level_test, dodikjan) == 2) {
                printf("Вы вышли из башни на 1-м этаже. \n");
                return;
            }
            continue;
        }

        Direction dir = dir_from_key(line[0]);
        if (dir == DIR_NONE) continue;

        if (move_check(level_test, dir, dodikjan))
            check_stairs(level_test, dodikjan);
    }
}



void read_map(void) {
    int safety = 1;
    FILE *input = fopen("Input.txt", "r");
    if (input == NULL) {
        printf("Не удалось открыть файл для получения кодированной карты");
        return;
    }

    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Не удалось открыть файл для записи декодированной карты");
        fclose(input);
        return;       
    }

    char line[1000];
    char decoded[1000];
    int line_number = 0;
    int first_len = -1;

    while (fgets(line, sizeof line, input) != NULL) {
        line_number ++;
        line[strcspn(line, "\r\n")] = '\0';
        int len = map_decoder(line, decoded, sizeof decoded);

        if (first_len == -1) {                 //Проверка: Сравниваем все строки по длине первой строки. Так что важно в ней не наложать
            first_len = len;
        } else if (len != first_len) {
            printf("Ошибка, строка %d имеет длину %d, согласно первой строке ожидается %d\n", line_number, len, first_len);
            safety = 0;
            return;
        } }

    if (safety = 1) {
        rewind(input);                          //Запись: Возвращаемся к чтению input, только теперь, после проверки записываем все в output
        while (fgets(line, sizeof line, input) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            map_decoder(line, decoded, sizeof decoded);
            fprintf(output, "%s\n", decoded);
            }
        
    }

    fclose(output);
    fclose(input);
}

int map_decoder(const char *encoded, char *decoded, int cap) {
    int len = 0, counter = 0;
    for (; *encoded; encoded++) {                                   //Проходимся по каждому символу в оригинальной строке
        if (*encoded >= '0' && *encoded <= '9') {                   //Говорим, что цифры - коды в диапазоне ежду символами '0' и '9'
            counter = counter * 10 + (*encoded - '0');              //'7' - '0' = 55-48 = 7. '7' -> 7. Плюс умножение на 10, если элементов больше 9
        } else {                                                    
            for (int i = 0; i < counter && len < cap - 1; i++) {    //Защита от переполнения. -1 для того чтобы влез \0
                decoded[len++] = *encoded;                          //Запихиваем символ в dcd и 
            }
            counter = 0;
        }
    }
    decoded[len] = '\0';
    return len;
}
