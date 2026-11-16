#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#define MAX_ITEM_NAME_LEN 64    //Длина названия предмета
#include <stdio.h>
#define MAX_ITEMS_INGAME 10     //количество предметов в проекте
#define MAX_INV_SIZE 10         //размер инвентаря
#define MAX_USER_NAME_LEN 64    //Длина имени пользователя

int current_day = 1;
int current_hour = 8;

//Основная структура данных игры
typedef struct {
    int current_hour;
    int current_day;
    int gameState;
    int activeCall;
    char user_name[MAX_USER_NAME_LEN];
    int inventory[MAX_INV_SIZE];
} GameState;


    
GameState game = {
    .current_hour = 8,
    .current_day = 1,
    .gameState = 0,
    .user_name = "Незнакомец",
    .inventory = {0}
};

//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;

//Список предметов
Item item_names[MAX_ITEMS_INGAME];

char user_name[MAX_USER_NAME_LEN];
int inventory[MAX_INV_SIZE] = {0};

#endif