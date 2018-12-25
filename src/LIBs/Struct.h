#include "Constants.h"
#include <stdbool.h>
#include <SDL.h>

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

typedef struct {
    int time;
    double x;
    double y;
    int angle;
} SHOT;

typedef struct {
    bool life;
    double x;
    double y;
    int angle;
    int RGBA_color[3]; // RGB
    int shooting_key;
    int directions[4]; //up, down, right, left
    SHOT shot[MAX_BALLS];
    int score;
} TANK;

typedef struct {
    short int x;
    short int y;
} POINT;

typedef struct {
    int x, y;
    void *parent;
    bool wall;
    int dirs; //Directions that still haven't been explored
} MAP;

typedef struct {
    int number;
    int lives;
    TANK tank[3];
} PLAYERS;

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} IMAGE;

#endif //PROJECT_STRUCT_H
