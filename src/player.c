#include "player.h"
#include "utils.h"

Player *Nikra = NULL;

void init_player(void) {
    Nikra = (Player*)malloc(sizeof(Player));

    if (Nikra == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }

    Nikra->max_health = 100;
    Nikra->cur_health = 100;
    Nikra->max_stamina = 50;
    Nikra->cur_stamina = 50;
    Nikra->gold = 0;
    Nikra->playerX = 5;
    Nikra->playerY = 5;

}


//Вывод инвентаря на экран
void inventory_showcase(GameState game) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, game.inventory[i], item_names[game.inventory[i]].name);
    }
}

//Вывод индексов предметов на экран
void items_index_showcase() {
    printf("\n--- ПРЕДМЕТЫ ---\n");
    for (int i = 0; i < MAX_ITEMS_INGAME; i++) {
        printf("ID = %d | Название: ( %s )\n", item_names[i].id, item_names[i].name);
    }
}

//Положить предмет в инвентарь
void inventory_item_add(GameState *game) {
    int slot_id;
    int item_id;
    slot_id=failsafe_sys_numbers("Введите индекс слота (0-9): ");

    //проверка существования слота
    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        items_index_showcase();
        item_id=failsafe_sys_numbers("\nВведите индекс предмета: ");
        
        //проверка существования предмета
        if (item_id > -1 && item_id < MAX_ITEMS_INGAME) {
            game->inventory[slot_id] = item_names[item_id].id;
            printf("Вы положили %s в слот %2d", item_names[item_id].name, slot_id); 
            } 
        else {
            printf("Такого предмета не существует");
        }
        
    }
    else {
        printf("Такого слота не существует");
    }
}

//Очистка слота инвентаря
void inventory_slot_clean(GameState *game) {
    
    int slot_id;
    inventory_showcase(*game);
    slot_id = failsafe_sys_numbers("Введите индекс слота, который хотите очистить: ");

    if (slot_id > -1 && slot_id < MAX_INV_SIZE) {
        game->inventory[slot_id] = 0;
    } else {
        printf("Такого слота не существует");
    }
}

void inventory_item_search_via_name(GameState game) {
    int amount_of_items = 0;

    char found_item_slots[MAX_INV_SIZE] = {0};
    char searched_item[MAX_ITEM_NAME_LEN * 2];

    failsafe_sys_russian("Введите название предмета, который хочешь найти: ", searched_item, MAX_ITEM_NAME_LEN * 2);

    for (int i = 0; i < MAX_INV_SIZE; i++) {
        int item_id = game.inventory[i];

        if (item_id >= 0 && item_id < MAX_ITEMS_INGAME) {
            if (strcmp(searched_item, item_names[item_id].name) == 0) {
                found_item_slots[amount_of_items] = i;
                amount_of_items ++;
            }
        }
    }
        
    if (amount_of_items > 0) {
        printf("Да, объект %s найден, он находится в слоте/ах: ", searched_item);
        for (int i = 0; i < amount_of_items; i++) {
                printf("%d", found_item_slots[i]);
                if (i < amount_of_items - 1) {
                    printf(", "); 
                } 
        }
        printf("\n");
    } else {
        printf("Нет, предмета %s, у тебя нет\n", searched_item);
    }
}

//Функция показа любимого предмета
void inventory_favorite_item_showcase(GameState game) {
    int count[10] = {0};
                
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (game.inventory[i] != 0) {
            count[game.inventory[i]]++;
        }
    }

    int max_count = 0;
    int fav_item = 0;
    for (int i = 0; i < MAX_INV_SIZE; i++) {
        if (count[i] > max_count) {
            max_count = count[i];
            fav_item = i;
        }
    }
    printf("Любимый предмет: %s, он встречается %d раз\n", item_names[fav_item].name, max_count);
}