#include "player.h"
#include "utils.h"

Player *Nikra = NULL;


void character_creation() {
    if (Nikra != NULL) {

        char answer[10];
        failsafe_sys_russian("Вы хотите перезаписать персонажа? Старый персонаж будет удален\n", answer, sizeof(answer));
            if (strcmp(answer, "да") == 0 || strcmp(answer, "yes") == 0) {

                printf("Освобождаем память по адресу: %p\n", (void*)Nikra);
                free(Nikra);
                Nikra = NULL; 
            }
            else {
                printf("Перезапись персонажа отменена\n");
                return;
            }
        }
    if (Nikra == NULL) {
        Nikra = (Player*)malloc(sizeof(Player));

        if (Nikra == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(1);
        }
    }
    printf("Память успешно выделена\n");

    failsafe_sys_russian("Приветствую тебя, фермер. Как тебя зовут?\n", Nikra->player_name, sizeof(Nikra->player_name));
    Nikra->max_health = 100;
    Nikra->cur_health = 100;
    Nikra->max_stamina = 50;
    Nikra->cur_stamina = 50;
    Nikra->gold = 0;
    Nikra->playerX = 5;
    Nikra->playerY = 5;
}


void character_showcase() {
    if (Nikra != NULL) {
        printf("\n==================================\n");
        printf("         КАРТОЧКА ПЕРСОНАЖА       \n");
        printf("==================================\n");
        printf(" Имя:       %s\n", Nikra->player_name);
        printf(" Здоровье:  %d / %d\n", Nikra->cur_health, Nikra->max_health);
        printf(" Вынослив.: %d / %d\n", Nikra->cur_stamina, Nikra->max_stamina);
        printf(" Золото:    %d\n", Nikra->gold);
        time_showcase(game);
        inventory_showcase(game);
        printf("==================================\n\n");
    }
}


