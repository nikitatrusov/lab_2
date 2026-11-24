#ifndef INVENTORY_H
#define INVENTORY_H


#include "utils.h"
#include "config.h"


//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct{
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;


extern Item item_names[MAX_ITEMS_INGAME];
extern int inventory[MAX_INV_SIZE];

void inventory_showcase(GameState game);
void inventory_favorite_item_showcase(GameState game);
void inventory_item_search_via_name(GameState game);
void inventory_slot_clean(GameState *game);
void inventory_item_add(GameState *game);
void items_index_showcase();

#endif