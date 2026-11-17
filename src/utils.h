#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#define MAX_ITEM_NAME_LEN 64    //Длина названия предмета
#include <stdio.h>
#define MAX_ITEMS_INGAME 10     //количество предметов в проекте
#define MAX_INV_SIZE 10         //размер инвентаря
#define MAX_USER_NAME_LEN 64    //Длина имени пользователя

//Основная структура данных игры
typedef struct GameState{
    int current_hour;
    int current_day;
    int gameState;
    int activeCall;
    int inventory[MAX_INV_SIZE];
} GameState;


//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;

extern Item item_names[MAX_ITEMS_INGAME];
extern char user_name[MAX_USER_NAME_LEN];
extern int inventory[MAX_INV_SIZE];

int randRange(int min, int max);
int failsafe_sys_numbers(const char *message_text);
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes);
void time_showcase(GameState game);
void time_advance(GameState *game);
void item_catalog_reader(void);
void diary_write(GameState *game);

#endif