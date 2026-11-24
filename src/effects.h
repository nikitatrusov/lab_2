#ifndef EFFECTS_H
#define EFFECTS_H


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


#endif