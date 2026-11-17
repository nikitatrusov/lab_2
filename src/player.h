#ifndef PLAYER_H
#define PLAYER_H

#define MAX_USER_NAME_LEN 64    //Длина имени пользователя


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

#endif