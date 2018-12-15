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
#include "View.h"
#include "Constants.h"


int keys[100];
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
                keys[event.key.keysym.sym % 100] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym % 100] = 0;
                if (event.key.keysym.sym == SDLK_m) {
                    shooting_flag = 0;
                }
                break;
        }
    }
    if (keys[SDLK_m % 100]) {
        if (!shooting_flag) {
            make_shot(*x, *y, *angle);
        }
        shooting_flag = 1;
    }
    if (*x < SCREEN_WIDTH - RADIUS && *x > RADIUS) {
        *x += SPEED * cos(*angle * PI / 180) * (keys[SDLK_UP % 100] - keys[SDLK_DOWN % 100]);
    } else if (*x >= SCREEN_WIDTH - RADIUS) {
        (*x)--;
    } else {
        (*x)++;
    }
    if (*y < SCREEN_HEIGHT - RADIUS && *y > RADIUS) {
        *y += SPEED * sin(*angle * PI / 180) * (keys[SDLK_UP % 100] - keys[SDLK_DOWN % 100]);
    } else if (*y >= SCREEN_HEIGHT - RADIUS) {
        (*y)--;
    } else {
        (*y)++;
    }
    *angle -= SPEED * (keys[SDLK_LEFT % 100] - keys[SDLK_RIGHT % 100]);
    if (*angle <= 360) {
        *angle -= 360;
    }
    if (*angle >= -360) {
        *angle += 360;
    }
}

