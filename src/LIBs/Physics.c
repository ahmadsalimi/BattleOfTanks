//
// Created by nik on 12/15/2018.
//

#include "Physics.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

int keys[100];
const int SPEED = 3;
const double PI = 3.141592653589;

int move(double *x, double *y, int *angle) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym % 100] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym % 100] = 0;
                break;
        }
    }
    *x += SPEED * cos(*angle * PI / 180) * (keys[SDLK_UP % 100] - keys[SDLK_DOWN % 100]);
    *y += SPEED * sin(*angle * PI / 180) * (keys[SDLK_UP % 100] - keys[SDLK_DOWN % 100]);
    *angle -= SPEED * (keys[SDLK_LEFT % 100] - keys[SDLK_RIGHT % 100]);
    if (*angle <= 360) {
        *angle -= 360;
    }
    if (*angle >= -360) {
        *angle += 360;
    }
}