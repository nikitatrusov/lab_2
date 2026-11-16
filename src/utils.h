#ifndef UTILS.H
#define UTILS.H

#endif

//Основная структура данных игры
typedef struct {
    int current_hour;
    int current_day;
    int gameState;
    int activeCall;
    char user_name[MAX_USER_NAME_LEN];
    int inventory[MAX_INV_SIZE];
} GameState;


