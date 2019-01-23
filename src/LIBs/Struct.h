#include "Constants.h"
#include <stdbool.h>
#include <SDL.h>

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

typedef struct {
    int time;
    double x;
    double y;
    Sint16 angle;
} SHOT;

typedef struct {
    Sint16 x;
    Sint16 y;
} POINT;

typedef struct {
    bool enable;
    Sint16 time;
    POINT center;
} LASER_BOX;

typedef struct {
    bool enable;
    POINT start;
    POINT finish;
    Sint8 target;
    Sint16 time;
    Sint8 target_counter;
    Sint8 targets[2];
    bool kill;
    Sint8 kill_time;
} LASER;

typedef struct {
    bool enable;
    Sint16 time;
    POINT center;
} MINE_BOX;

typedef struct {
    bool enable;
    Sint8 mode; //0: carrying, 1: laid out and showing, 2: hide, 3: killing
    POINT position;
    Sint8 target;
    Sint16 carrying_time;
    Sint8 show_time;
    Sint16 hide_time;
    Sint8 kill_time;
} MINE;

typedef struct {
    LASER laser;
    MINE mine;
} POWER;

typedef struct {
    bool life;
    double x;
    double y;
    double angle;
    Sint16 RGBA_color[3]; // RGB
    int shooting_key;
    int directions[4]; //up, down, right, left
    SHOT shot[MAX_BALLS];
    Sint16 score;
    Sint8 shot_type; // 0: normal, 1: laser
    POWER power;
} TANK;


typedef struct {
    Sint8 x, y;
    void *parent;
    bool wall;
    Sint8 dirs; //Directions that still haven't been explored
} MAP;

typedef struct {
    Sint8 number;
    Sint8 lives;
    TANK tank[3];
    Sint8 state;
} PLAYERS;

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} IMAGE;

typedef enum {
    BACKGROUND, CLICK, TOGGLE, EXPLOSION, FINISH, OPENING, GAME_OVER, LASER_SHOOT, MINING, POWER_ACHIEVE, POWER_APPEAR, REGENERATION, SHOOT
} SOUNDS;

extern PLAYERS players;
extern LASER_BOX *laser_box;
extern MINE_BOX *mine_box;
extern Sint8 menu_state;
extern Sint8 menu_button_state;
extern bool menu_playtime;
extern Sint8 multiplayer_state;
extern Sint16 finish_point;
extern int power_make_time;
extern int play_time;
extern bool save_mode;
extern int last_number;
extern int load_number;
extern int keys[501];

#endif //PROJECT_STRUCT_H
