#ifndef PLAYER_H
#define PlAYER_H


typedef struct {
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[64];
} Player;

Player dodikjan = {
    .max_health = 100,
    .cur_health = 100,
    .max_stamina = 50,
    .cur_stamina = 50,
    .gold = 0,
    .playerX = 5,
    .playerY = 5
};

#endif