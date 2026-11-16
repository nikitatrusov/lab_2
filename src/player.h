#ifndef PLAYER.H
#define PlAYER.H

#endif

Player dodikjan = {
    .max_health = 100,
    .cur_health = 100,
    .max_stamina = 50,
    .cur_stamina = 50,
    .gold = 0,
    .playerX = 5,
    .playerY = 5
};


typedef struct {
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[64];
} Player;


//Список предметов
Item item_names[MAX_ITEMS_INGAME];

//Инициализация типа данных Item, в дальнейшем необходимо для хранения списка предметов
typedef struct {
    int id;
    char name[MAX_ITEM_NAME_LEN];
} Item;
