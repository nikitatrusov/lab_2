#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"


typedef struct {
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[MAX_USER_NAME_LEN];
} Player;

extern Player *Nikra;
extern GameState game;

void time_sleep(GameState *game, Player *Nikra);

#endif