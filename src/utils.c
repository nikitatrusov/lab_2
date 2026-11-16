#include "utils.h"



//Функция проверки ввода для числовых значений
int failsafe_sys_numbers(const char *message_text) {

        int enter_data;
        int result;

        while (1) {
            printf("%s", message_text);   // переделать все через puts(str) !!!!
            // scanf дает 1, если число успешно считано
            result = scanf("%d", &enter_data);

            if (result == 1) {
                //Пользователь НЕ дурак, возвращаем число
                while (getchar() != '\n');
                return enter_data;
            } else {
                // Ошибка: введены не цифры (буквы, символы и т.д.)
                printf("Зачем буквы ввел Ара, не делай так дорогой, введи число\n");
                while (getchar() != '\n');
            }
        }
}

//Функция проверки ввода для русского языка
void failsafe_sys_russian(const char *message_text, char *out_message, size_t max_bytes) {
        HANDLE h_stdin = GetStdHandle(STD_INPUT_HANDLE); //Дескриптор стандартного ввода(ссылка на консоль), чтобы читать прям оттуда
        wchar_t wbuf[MAX_ITEM_NAME_LEN];                //буфер из широких символов, чтобы кириллица не ломалась
        DWORD charsRead = 0;                            //сколько в итоге символов прочитала функция

        fputs(message_text, stdout);                                    //Безопасно выводим текст
        fflush(stdout);                                                 //Принудительный сброс буфера
        ReadConsoleW(h_stdin, wbuf, max_bytes - 1, &charsRead, NULL);    //Читает строку из консоли. Откуда читать, куда ложить, макс символов
                                                                        //куда записать число прочитанных символов, и NULL
                                                                        //ReadConsoleW не ставит в конце \0, поэтому это делаем мы, чтобы wcscspn остановился
                                                                        //Ставим на место конца строки (\r\n) \0
        wbuf[charsRead] = L'\0';
        wbuf[wcscspn(wbuf, L"\r\n")] = L'\0';

        WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, out_message, (int)max_bytes, NULL, NULL); //WideChar в CP_UTF8, выводим в out_message
}


int randRange(int min, int max) {
    return min + rand() % (max - min +1);
}


//Функция показа времени
void time_showcase(GameState game) {
    printf("Текущее время: День %d, %02d:00\n", game.current_day, game.current_hour);   
}


//Функция изменения времени
void time_advance(GameState *game) {
    int hours;
    printf("Было %d:00 %2d-го дня, сколько часов ты проведешь за работой:", game->current_hour, game->current_day); //Подстава. так как мне впадлу переписать fail_sys
    hours = failsafe_sys_numbers("");                                                            //еще раз, есть этот принтф, который в свою очередь
    if (hours < 0) return;                                                               //сдвигает в случае неправильного ввода на след строку
    game->current_hour += hours;                                                                      //когда нибудь я этим займусь, честно честно
    game->current_day += game->current_hour / 24;
    game->current_hour = game->current_hour % 24;
}


//Чтение файла с списком предметов item_catalogue.txt
void item_catalog_reader(void) {

    FILE *file = fopen("item_catalogue.txt", "r");

    if (file == NULL) {
        perror("Не удалось найти файл");
    } else {
        printf("Файл прочитан успешно");

        //буферы для считывания данных с item_catalogue.txt
        int temp_id;
        char temp_name[MAX_ITEM_NAME_LEN];

        while (fscanf(file, "%d\t%63[^\n]", &temp_id, temp_name) == 2) {
            if (temp_id >= 0 && temp_id < MAX_ITEMS_INGAME) {
                item_names[temp_id].id = temp_id;
                strcpy(item_names[temp_id].name, temp_name);
            }
        }
        fclose(file);
    }
}

//Запись состояния игры в файл diary.txt
void diary_write(GameState *game) {

    FILE *file = fopen("diary.txt", "a");

    if (file == NULL) {
        perror("Не создать найти файл");
        fclose(file);
    } else {
        printf("Файл найден");
        fprintf(file, "Имя: %s, Сейчас %d часов %2d-го дня\n", game->user_name, game->current_hour, game->current_day);
        for (int i = 0; i < MAX_INV_SIZE; i++) {
            fprintf(file, "Слот %d: [%d] (%s)\n", i, game->inventory[i], item_names[game->inventory[i]].name);
            
        }
    }
    fclose(file);
}