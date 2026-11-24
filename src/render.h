#ifndef RENDER_H
#define RENDER_H


typedef struct GameState GameState;
typedef struct Player Player; // (Assuming Player is named)
typedef struct Level Level;


void render_hud(GameState *game, Player *Nikra, Level *level_test);
void render_map(Level *level_test, Player *Nikra);
void render_time_menu(GameState *game);
void render_inventory_menu();


#endif