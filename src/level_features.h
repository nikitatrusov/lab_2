#ifndef LEVEL_FEATURES_H
#define LEVEL_FEATURES_H


typedef struct GameState GameState;
typedef struct Player Player; 
typedef struct Level Level;


typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_COUNT
} Direction;


int check_stairs(Level *level, Player *Nikra, GameState *game);
int fan_direction(char c, int *dx, int *dy);
int apply_fan(Level *level, Player *Nikra);


#endif