#include "interactions.h"


#include <stdio.h>


void add_effect(int power, int duration, ActiveEffect *effects, EffectTypes type) {
    for (int i = 0; i < MAX_EFFECTS; i++) {
        if (effects[i].type == type) {
            effects[i].time_left = duration; 
            return;
        }

        if (effects[i].type == DEBUFF_NONE) {
            effects[i].type = type;
            effects[i].time_left = duration;
            effects[i].power = power;
            return;
        }
    }
}


void process_debuffs(int *health, ActiveEffect *effects, int *stamina) {
    for (int i = 0; i < MAX_EFFECTS; i++) {
        if (effects[i].type == DEBUFF_NONE) continue;

        if (effects[i].type == DEBUFF_POISON) {
            *health -= effects[i].power;
            printf("Вы получили %d урона от яда!\n", effects[i].power);
        } 
        else if (effects[i].type == DEBUFF_SPRAIN) {
            *stamina -= effects[i].power;
            printf("Вам тяжело ходить из-за расстяжения связок!\n", effects[i].power);
        }
        else if (effects[i].type == BUFF_LIVELINESS) {
            *stamina += effects[i].power;
            printf("Вы чуствуете как жизненная энергия наполняет вас!\n", effects[i].power);
        }

        effects[i].time_left--;

        if (effects[i].time_left <= 0) {
            printf("Эффект закончился.\n");
            effects[i].type = DEBUFF_NONE; 
        }
    }
}