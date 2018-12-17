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

SDL_Window *window;
SDL_Renderer *renderer;

struct SHOT shot[MAX_BALLS];

int max_boxes_x = 0;
int max_boxes_y = 0;
int vertical_walls[10][10];
int horizontal_walls[10][10];

void setting() {
    read_map();
    for (int i = 0; i < MAX_BALLS; i++) {
        shot[i].time = 0;
    }
}

void show_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("AlterTank", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void read_map() {
    srand(time(NULL));
    int r = rand() % 3;
    int i, j, n, x1, y1, x2, y2;
    char line[30];
    FILE *gmaps;
    gmaps = fopen("C:\\Users\\nik\\Documents\\GitHub\\97106013\\src\\LIBs\\Generated_Maps\\gmaps.txt", "r");
    for (j = 0; j < r;) {
        fgets(line, 30, gmaps);
        if (line[0] == 10) {
            j++;
        }
    }
    fscanf(gmaps, "%d", &n);
    for (int k = 0; k < n; k++) {
        fscanf(gmaps, "%d %d %d %d", &x1, &y1, &x2, &y2);
        if (x1 == x2) {
            for (j = y1; j < y2; j++) {
                vertical_walls[j][x1] = 1;
            }
        } else {
            for (i = x1; i < x2; i++) {
                horizontal_walls[y1][i] = 1;
            }
        }
        if (x2 > max_boxes_x) {
            max_boxes_x = x2;
        }
        if (y2 > max_boxes_y) {
            max_boxes_y = y2;
        }
    }
    for (i = 0; i <= max_boxes_y; i++) {
        for (j = 0; j <= max_boxes_x; j++) {
            printf("%d ", horizontal_walls[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i <= max_boxes_y; i++) {
        for (j = 0; j <= max_boxes_x; j++) {
            printf("%d ", vertical_walls[i][j]);
        }
        printf("\n");
    }
}

void draw_map() {
    for (int m = 0; m <= max_boxes_y; m++) {
        for (int n = 0; n <= max_boxes_x; n++) {
            if (horizontal_walls[m][n]) {
                lineRGBA(renderer, n * BOX_WIDTH, m * BOX_WIDTH, (n + 1) * BOX_WIDTH, m * BOX_WIDTH, 0, 0, 0, 255);
            }
            if (vertical_walls[m][n]) {
                lineRGBA(renderer, n * BOX_WIDTH, m * BOX_WIDTH, n * BOX_WIDTH, (m + 1) * BOX_WIDTH, 0, 0, 0, 255);
            }
        }
    }
}

void draw_tank(const double *x, const double *y, const int *angle) {
    for (int i = -10; i <= 10; i++) {
        lineRGBA(renderer, *x, *y, *x + LENGTH * cos((*angle + i / 2.0) * PI / 180),
                 *y + LENGTH * sin((*angle + i / 2.0) * PI / 180), 0, 0, 0, 255);
    }
    filledCircleRGBA(renderer, *x, *y, TANK_RADIUS, 232, 170, 0, 255);
}

void draw_shot() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (shot[i].time > 0) {
            shoot(&shot[i].x, &shot[i].y, &shot[i].angle, &shot[i].time);
            filledCircleRGBA(renderer, shot[i].x, shot[i].y, SHOT_RADIUS, 0, 0, 0, 255);
        }
    }
}

void drawing(double *x, double *y, int *angle) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_tank(x, y, angle);
    draw_shot();
    SDL_RenderPresent(renderer);
}

void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void make_shot(double x, double y, int angle) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (shot[i].time <= 0) {
            shot[i].time = LIFE_OF_SHOT;
            shot[i].x = x + LENGTH * cos(angle * PI / 180);
            shot[i].y = y + LENGTH * sin(angle * PI / 180);
            shot[i].angle = angle;
            break;
        }
    }
}