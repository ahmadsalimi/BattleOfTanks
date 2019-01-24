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
    enum {
      M_CARRYING, M_MINING, M_HIDDEN, M_KILLING
    } mode;
    POINT position;
    Sint8 target;
    Sint16 carrying_time;
    Sint16 show_time;
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
    enum {
        T_NORMAL, T_LASER, T_MINE
    } shot_type;
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
    enum {
     P_MENU, P_WAITING, P_PLAYING, P_GAME_OVER
    } state;
} PLAYERS;

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} IMAGE;

typedef enum {
    BACKGROUND_S, CLICK_S, TOGGLE_S, EXPLOSION_S, FINISH_S, OPENING_S, GAME_OVER_S, LASER_SHOOT_S, MINING_S, POWER_ACHIEVE_S, POWER_APPEAR_S, REGENERATION_S, SHOOT_S
} SOUNDS;

typedef enum {
    M_OPENING, M_NEW_GAME, M_LOAD
} MENU_STATES;

typedef enum{
    B_NEW_GAME, B_LOAD, B_EXIT
} MENU_BUTTON_STATE;

extern PLAYERS players;
extern LASER_BOX laser_box[2];
extern MINE_BOX mine_box[2];
extern MENU_STATES menu_state;
extern MENU_BUTTON_STATE menu_button_state;
extern bool menu_playtime;
extern Sint8 multiplayer_state;
extern Sint16 finish_point;
extern int power_make_time;
extern int play_time;
extern bool save_mode;
extern int last_number;
extern int load_number;
extern int keys[501];
extern bool file_checked;
extern Sint8 max_boxes_x;
extern Sint8 max_boxes_y;
extern bool vertical_walls[10][10];
extern bool horizontal_walls[10][10];
extern bool not_closed;

#endif //PROJECT_STRUCT_H