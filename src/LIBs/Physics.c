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


int keys[401];
int shooting_flag = 0;

void shoot(int *x, int *y, int *angle, int *time) {
    if (*x >= SCREEN_WIDTH - SHOT_RADIUS ||
        ((*x) / BOX_WIDTH < (int) (*x + 2 * SPEED * cos((*angle) * PI / 180)) / BOX_WIDTH &&
         vertical_walls[(int) (*y + 2 * SPEED * sin((*angle) * PI / 180)) / BOX_WIDTH][
                 (int) (*x + 2 * SPEED * cos((*angle) * PI / 180)) / BOX_WIDTH])) {
        *angle = 180 - *angle;
        (*x)--;
    } else if (*x <= SHOT_RADIUS || ((*x) / BOX_WIDTH > (int) (*x + 2 * SPEED * cos((*angle) * PI / 180)) / BOX_WIDTH &&
                                     vertical_walls[(*y) / BOX_WIDTH][(*x) / BOX_WIDTH])) {
        *angle = 180 - *angle;
        (*x)++;
    }
    if (*y >= SCREEN_HEIGHT - SHOT_RADIUS ||
        ((*y) / BOX_WIDTH < (int) (*y + 2 * SPEED * sin((*angle) * PI / 180)) / BOX_WIDTH &&
         horizontal_walls[(int) (*y + 2 * SPEED * sin((*angle) * PI / 180)) / BOX_WIDTH][
                 (int) (*x + 2 * SPEED * cos((*angle) * PI / 180)) / BOX_WIDTH])) {
        *angle = -*angle;
        (*y)--;
    } else if (*y <= SHOT_RADIUS || ((*y) / BOX_WIDTH > (int) (*y + 2 * SPEED * sin((*angle) * PI / 180)) / BOX_WIDTH &&
                                     horizontal_walls[(*y) / BOX_WIDTH][(*x) / BOX_WIDTH])) {
        *angle = -*angle;
        (*y)++;
    }
    *x = (int) (*x + 2 * SPEED * cos((*angle) * PI / 180));
    *y = (int) (*y + 2 * SPEED * sin((*angle) * PI / 180));
    (*time)--;
}

void tank_motion(double *x, double *y, int *angle) {
    if (*x < SCREEN_WIDTH - TANK_RADIUS && *x > TANK_RADIUS) {
        *x += SPEED * cos(*angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);
    } else if (*x >= SCREEN_WIDTH - TANK_RADIUS) {
        (*x)--;
    } else {
        (*x)++;
    }
    if (*y < SCREEN_HEIGHT - TANK_RADIUS && *y > TANK_RADIUS) {
        *y += SPEED * sin(*angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);
    } else if (*y >= SCREEN_HEIGHT - TANK_RADIUS) {
        (*y)--;
    } else {
        (*y)++;
    }
}

void tank_rotation(int *angle) {
    *angle -= OMEGA * (keys[SDLK_LEFT % 401] - keys[SDLK_RIGHT % 401]);
    if (*angle <= 360) {
        *angle -= 360;
    }
    if (*angle >= -360) {
        *angle += 360;
    }
}

int move(double *x, double *y, int *angle) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym % 401] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym % 401] = 0;
                if (event.key.keysym.sym == SDLK_m) {
                    shooting_flag = 0;
                }
                break;
        }
    }
    if (keys[SDLK_m % 401]) {
        if (!shooting_flag) {
            make_shot(*x, *y, *angle);
        }
        shooting_flag = 1;
    }
    tank_motion(x, y, angle);
    tank_rotation(angle);
}