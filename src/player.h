#ifndef PLAYER_H
#define PLAYER_H


#include "inventory.h"
#include "interactions.h"
#include "effects.h"


typedef struct Player{
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[MAX_USER_NAME_LEN];
    ActiveEffect effects[MAX_EFFECTS];
} Player;


extern Player *Nikra;
extern GameState game;


void character_creation();
void character_showcase();

#endif