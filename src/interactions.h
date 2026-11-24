#ifndef INTERACTIONS_H
#define INTERACTIONS_H


#include "effects.h"


#define MAX_EFFECTS 5


void add_effect(int power, int duration, ActiveEffect *effects, EffectTypes type);
void process_debuffs(int *health, ActiveEffect *effects, int *stamina);


#endif
