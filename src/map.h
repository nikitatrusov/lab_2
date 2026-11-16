#ifndef MAP_H
#define MAP_H


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

static const int DIR_DX[DIR_COUNT] = { 0, 0, 0, -1, 1 };
static const int DIR_DY[DIR_COUNT] = { 0, -1, 1, 0, 0 };

int generate_map(Level *level_test, Player *dodikjan);
void render_map(Level *level_test, Player *dodikjan);
int move_check(Level *level_test, Direction dir, Player *dodikjan);
int check_stairs(Level *level, Player *dodikjan);
int fan_direction(char c, int *dx, int *dy);
int apply_fan(Level *level, Player *dodikjan);
void begin_journey(Level *level_test, Player *dodikjan);
void read_map(void);
int map_decoder(const char *encoded, char *decoded, int cap);


#define T_chance 5
#define C_chance 5
#define rand_gen_max 100
#define rand_gen_min 0
#define MAX_FLOORS 10           //Количество этажей    
#define fly_range 4
#define max_flight_range 100

#endif