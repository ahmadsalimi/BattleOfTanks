//
// Created by nik on 12/15/2018.
//

#include "View.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Physics.h"
#include "Struct.h"

SDL_Window *window;
SDL_Renderer *renderer;

const int RADIUS = 25;
const int SHOT_RADIUS = 5;
const int LENGTH = 35;
const double FPS = 50;

struct SHOT shot[5];

void show_window(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("AlterTank", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    for(int i = 0; i < 5; i++){
        shot[i].time = 0;
    }
}
void drawing(double* x, double* y, int* angle){
    const double PI = 3.141592653589;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    filledCircleRGBA(renderer, *x, *y, RADIUS, 232, 170, 0, 255);
    for(int i = -10; i <= 10; i++){
        SDL_RenderDrawLine(renderer, *x, *y, *x + LENGTH * cos((*angle + i/2.0) * PI / 180), *y + LENGTH * sin((*angle + i/2.0) * PI / 180));
    }
    for (int i = 0; i < 5; i++){
        if (shot[i].time > 0){
            shoot(&shot[i].x, &shot[i].y, &shot[i].angle, &shot[i].time);
            filledCircleRGBA(renderer, shot[i].x, shot[i].y, SHOT_RADIUS, 0, 0, 0, 255);
        }
    }
    SDL_RenderPresent(renderer);
}
void Quit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void make_shot(double x, double y, int angle){
    const double PI = 3.141592653589;
    for (int i = 0; i < 5; i++){
        if (shot[i].time <= 0){
            shot[i].time = 5 * FPS;
            shot[i].x = x + LENGTH * cos(angle * PI / 180);
            shot[i].y = y + LENGTH * sin(angle * PI / 180);
            shot[i].angle = angle;
            break;
        }
    }
}