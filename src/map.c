#include "map.h"


int journey_check(GameState game, Player *Nikra) {
    if (Nikra == NULL) {
        printf("Создай персонажа прежде чем идти в поход\n");
        return 0;
    }
    else if (game.current_hour >= 2 && game.current_hour <= 8) {
        printf("Ты не можешь отправится в приключение ночью, дождись утра\n");
        return 0;
    }
    else if (STAMINA_JOURNEY_MIN <= Nikra->max_stamina / Nikra->cur_stamina) {
        printf("Ты слишком устал, нужно иметь больше 10% выносливости для начала приключения.\n");
        return 0;
    }
    return 1;
}


void free_map(Level *level_test) {
    if (level_test->mapLocation != NULL) {
        for (int i = 0; i < level_test->height; i++) {
            free(level_test->mapLocation[i]);
        }
        free(level_test->mapLocation);
        level_test->mapLocation = NULL;
    }
}


int journey_evacuation_check(GameState *game, Player *Nikra) {
    if (Nikra->cur_stamina <= 1) {
        printf("Вы потеряли сознание в ходе приключения, к вашему счастью сущности в форме гномиков спасли вас.."
               "только кажется ваши карманы стали легче.. \n");
        Nikra->gold /=2;
        return 0;
    }
    else if (game->current_hour == 2) {
        printf("Кажется твоему герою хочется спать..\n"
               "К твоему счастью, сущности в форме гномиков вытащили тебя спящего из подземелья.."
               "только кажется ваши карманы стали легче.. \n");
            Nikra->gold /=2;
            return 0;
    }
    return 1;
}


int generate_map(Level *level_test, Player *Nikra) {
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

    level_test->mapLocation[Nikra->playerY][Nikra->playerX] = 'D'; //Лестница на выход

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


int move_check(Level *level_test, Direction dir, Player *Nikra) {
    int nx = Nikra->playerX + DIR_DX[dir];
    int ny = Nikra->playerY + DIR_DY[dir];

    if (nx < 0 || nx >= level_test->width)  return 0;
    if (ny < 0 || ny >= level_test->height) return 0;
    if (level_test->mapLocation[ny][nx] == '#') return 0;
    if (level_test->mapLocation[ny][nx] == 'C') return 0;
    if (level_test->mapLocation[ny][nx] == 'T') return 0;

    Nikra->playerX = nx;
    Nikra->playerY = ny;
    int guard = 0;
    if (randRange(rand_gen_min, rand_gen_max) < 100) {
        int roll = randRange(rand_gen_min, rand_gen_max);
        if (roll > 66) {
            add_effect(3, 5, Nikra->effects, DEBUFF_POISON);
        } else if (roll > 33 && roll < 67) {
            add_effect(3, 5, Nikra->effects, DEBUFF_SPRAIN);
        } else {
            add_effect(3, 5, Nikra->effects, BUFF_LIVELINESS);
        }
    }
    while (apply_fan(level_test, Nikra) && guard++ < 15) {}
    return 1;
}


void begin_journey(Level *level_test, Player *Nikra, GameState *game) {

    if (journey_check(*game, Nikra) == 0) {
        return;
    }

    level_test->floor_number = 1;
    Nikra->playerX = level_test->width / 2;
    Nikra->playerY = level_test->height / 2;

    if (generate_map(level_test, Nikra) != 0) {
        printf("Не удалось создать карту!!! \n");
        return;
    }

    printf("Этаж %d | wasd (q — выход): \n", level_test->floor_number);
    char line[16];
    while (1) {
        if (journey_evacuation_check(game, Nikra) == 0) {
            free_map(level_test);
            return;
        }
        process_debuffs(&Nikra->cur_health, Nikra->effects, &Nikra->cur_stamina);
        render_hud(game, Nikra, level_test);
        render_map(level_test, Nikra);
        printf("wasd (q — выход): ");
        if (!fgets(line, sizeof line, stdin)) break;   // EOF / ошибка ввода

        if (line[0] == 'q') {
            if (check_stairs(level_test, Nikra, game) == 2) {
                printf("Вы вышли из башни на 1-м этаже. \n");
                free_map(level_test);
                return;
            }
            continue;
        }

        Direction dir = dir_from_key(line[0]);
        if (dir == DIR_NONE) continue;

        if (move_check(level_test, dir, Nikra))
            Nikra->cur_stamina--;
            check_stairs(level_test, Nikra, game);
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
