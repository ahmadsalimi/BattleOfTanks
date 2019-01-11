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
    bool life;
    double x;
    double y;
    double angle;
    Sint16 RGBA_color[3]; // RGB
    int shooting_key;
    int directions[4]; //up, down, right, left
    SHOT shot[MAX_BALLS];
    Sint16 score;
} TANK;

typedef struct {
    Sint16 x;
    Sint16 y;
} POINT;

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

#endif //PROJECT_STRUCT_H
