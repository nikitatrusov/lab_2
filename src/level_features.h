#ifndef LEVEL_FEATURES_H
#define LEVEL_FEATURES_H


typedef struct GameState GameState;
typedef struct Player Player; 
typedef struct Level Level;


int check_stairs(Level *level, Player *Nikra, GameState *game);
int fan_direction(char c, int *dx, int *dy);
int apply_fan(Level *level, Player *Nikra);


#endif