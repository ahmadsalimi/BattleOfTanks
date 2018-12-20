//
// Created by nik on 12/15/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "View.h"
#include "Physics.h"
#include "Struct.h"
#include "Constants.h"
#include "MapGenerate.h"

SDL_Window *window;
SDL_Renderer *renderer;

SHOT shot[MAX_BALLS];


void setting() {
    srand((unsigned int) time(NULL));
    generate_map();
    for (int i = 0; i < MAX_BALLS; i++) {
        shot[i].time = 0;
    }
}

void show_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("AlterTank", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}


void draw_map() {
    for (int n = 0; n <= max_boxes_x; n++) {
        for (int m = 0; m <= max_boxes_y; m++) {
            if (horizontal_walls[m][n]) {
                lineRGBA(renderer, (Sint16) ((n + 1) * BOX_WIDTH), (Sint16) ((m + 1) * BOX_WIDTH), (Sint16) ((n + 2) * BOX_WIDTH),
                         (Sint16) ((m + 1) * BOX_WIDTH), 0, 0, 0, 255);
            }
            if (vertical_walls[m][n]) {
                lineRGBA(renderer, (Sint16) ((n + 1) * BOX_WIDTH), (Sint16) ((m + 1) * BOX_WIDTH), (Sint16) ((n + 1) * BOX_WIDTH),
                         (Sint16) ((m + 2) * BOX_WIDTH), 0, 0, 0, 255);
            }
        }
    }
}

void draw_tank(TANK *tank) {
    for (int i = -10; i <= 10; i++) {
        lineRGBA(renderer, (Sint16) tank->x, (Sint16) tank->y,
                 (Sint16) (tank->x + LENGTH * cos((tank->angle + i / 2.0) * PI / 180)),
                 (Sint16) (tank->y + LENGTH * sin((tank->angle + i / 2.0) * PI / 180)), 0, 0, 0, 255);
    }
    filledCircleRGBA(renderer, (Sint16) tank->x, (Sint16) tank->y, (Sint16) (TANK_RADIUS), 232, 170, 0, 255);
}

void draw_shot() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (shot[i].time > 0) {
            shoot(&shot[i]);
            filledCircleRGBA(renderer, (Sint16) shot[i].x, (Sint16) shot[i].y, (Sint16) (SHOT_RADIUS), 0, 0, 0, 255);
        }
    }
}

void drawing(TANK *tank) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_tank(tank);
    draw_shot();
    SDL_RenderPresent(renderer);
}

void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void make_shot(TANK *tank) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (shot[i].time <= 0) {
            shot[i].time = LIFE_OF_SHOT;
            shot[i].x = (int) (tank->x + (TANK_RADIUS + 2 * SHOT_RADIUS) * cos(tank->angle * PI / 180));
            shot[i].y = (int) (tank->y + (TANK_RADIUS + 2 * SHOT_RADIUS) * sin(tank->angle * PI / 180));
            shot[i].angle = tank->angle;
            break;
        }
    }
}