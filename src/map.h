#ifndef MAP.H
#define MAP.H

#endif

typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_COUNT
} Direction;


Direction dir_from_key(char c) {
    switch (c) {
        case 'w': return DIR_UP;
        case 's': return DIR_DOWN;
        case 'd': return DIR_RIGHT;
        case 'a': return DIR_LEFT;
        default: return DIR_NONE;
    }
}

Direction dir_from_fan(char c) {
    switch (c) {
        case '^': return DIR_UP;
        case 'v': return DIR_DOWN;
        case '>': return DIR_RIGHT;
        case '<': return DIR_LEFT;
        default: return DIR_NONE;
    }
}

typedef struct {
    char **mapLocation;
    int width, height;
    int floor_number;
} Level;

Level level_test = {
    .width = 10,
    .height = 10,
    .floor_number = 1
};

//Информация о карте
typedef struct {
    int size_x;
    int size_y;
    int size_z;
    char map_name[100]; //Временно для тестов!!!!!
} MapInfo;
