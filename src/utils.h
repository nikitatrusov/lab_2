#ifndef UTILS_H
#define UTILS_H


#include "config.h"


#include <stddef.h>


typedef struct Player Player;
typedef struct Item Item; 
//Основная структура данных игры

typedef struct GameState{
    int current_hour;
    int current_day;
    int gameState;
    int activeCall;
    int inventory[MAX_INV_SIZE];
} GameState;


extern char user_name[MAX_USER_NAME_LEN];


int randRange(int min, int max);
int failsafe_sys_numbers(const char *message_text);
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes);
void time_showcase(GameState game);
void time_advance(GameState *game);
void item_catalog_reader(void);
void diary_write(GameState *game);
void time_advance_formap(GameState *game);
void render_time_menu(GameState *game);
void render_inventory_menu();
int ask_state_menu(char *request);
void time_sleep(GameState *game, Player *Nikra);

#endif