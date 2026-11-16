#ifndef PLAYER_H
#define PlAYER_H


typedef struct Player{
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[64];
} Player;

#endif