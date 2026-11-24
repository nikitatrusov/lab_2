#ifndef PLAYER_H
#define PLAYER_H


#define MAX_EFFECTS 5



typedef enum {
    DEBUFF_NONE = 0,
    DEBUFF_POISON,
    DEBUFF_SPRAIN,
    BUFF_LIVELINESS
} EffectTypes;


typedef struct {
    EffectTypes type;
    int time_left;
    int power;
} ActiveEffect;


typedef struct {
    int max_health;
    int cur_health;
    int max_stamina;
    int cur_stamina;
    int gold;
    int playerX, playerY;
    char player_name[MAX_USER_NAME_LEN];
    ActiveEffect effects[MAX_EFFECTS];
} Player;


extern Player *Nikra;
extern GameState game;

void character_creation();
void character_showcase();

#endif