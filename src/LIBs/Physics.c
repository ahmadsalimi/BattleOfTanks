//
// Created by nik on 12/15/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Physics.h"
#include "View.h"
#include "Constants.h"


int keys[501];
int shooting_flag = 0;

void shoot(double *x, double *y, int *angle, int *time) {
    if (*x >= SCREEN_WIDTH - SHOT_RADIUS || *x <= SHOT_RADIUS) {
        *angle = 180 - *angle;
    }
    if (*y >= SCREEN_HEIGHT - SHOT_RADIUS || *y <= SHOT_RADIUS) {
        *angle = -*angle;
    }
    *x = *x + 2 * SPEED * cos((*angle) * PI / 180);
    *y = *y + 2 * SPEED * sin((*angle) * PI / 180);
    (*time)--;
}

int move(double *x, double *y, int *angle) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym % 501] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym % 501] = 0;
                if (event.key.keysym.sym == SDLK_m) {
                    shooting_flag = 0;
                }
                break;
        }
    }
    if (keys[SDLK_m % 501]) {
        if (!shooting_flag) {
            make_shot(*x, *y, *angle);
        }
        shooting_flag = 1;
    }
    if (*x < SCREEN_WIDTH - TANK_RADIUS && *x > TANK_RADIUS) {
        *x += SPEED * cos(*angle * PI / 180) * (keys[SDLK_UP % 501] - keys[SDLK_DOWN % 501]);
    } else if (*x >= SCREEN_WIDTH - TANK_RADIUS) {
        (*x)--;
    } else {
        (*x)++;
    }
    if (*y < SCREEN_HEIGHT - TANK_RADIUS && *y > TANK_RADIUS) {
        *y += SPEED * sin(*angle * PI / 180) * (keys[SDLK_UP % 501] - keys[SDLK_DOWN % 501]);
    } else if (*y >= SCREEN_HEIGHT - TANK_RADIUS) {
        (*y)--;
    } else {
        (*y)++;
    }
    *angle -= OMEGA * (keys[SDLK_LEFT % 501] - keys[SDLK_RIGHT % 501]);
    if (*angle <= 360) {
        *angle -= 360;
    }
    if (*angle >= -360) {
        *angle += 360;
    }
}

