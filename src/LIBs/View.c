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

PLAYERS players;

void setting() {
    srand((unsigned int) time(NULL));
    generate_map();
    players.number = 3;
    for (int i = 0; i < players.number; i++){
        players.tank[i].life = 1;
        players.tank[i].x = START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        players.tank[i].y = START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        players.tank[i].angle = rand() % 360;
        for (int j = 0; j < MAX_BALLS; j++) {
            players.tank[i].shot[j].time = 0;
        }
    }
    // PLAYER 0
    players.tank[0].RGBA_color[0] = 239; players.tank[0].RGBA_color[1] = 0; players.tank[0].RGBA_color[2] = 0; //red
    players.tank[0].shooting_key = SDLK_m;
    players.tank[0].directions[0] = SDLK_UP; players.tank[0].directions[1] = SDLK_DOWN; players.tank[0].directions[2] = SDLK_RIGHT; players.tank[0].directions[3] = SDLK_LEFT; //up, down, right, left
    // PLAYER 1
    players.tank[1].RGBA_color[0] = 0; players.tank[1].RGBA_color[1] = 132; players.tank[1].RGBA_color[2] = 0; //green
    players.tank[1].shooting_key = SDLK_q;
    players.tank[1].directions[0] = SDLK_e; players.tank[1].directions[1] = SDLK_d; players.tank[1].directions[2] = SDLK_f; players.tank[1].directions[3] = SDLK_s; //up, down, right, left
    // PLAYER 2
    players.tank[2].RGBA_color[0] = 0; players.tank[2].RGBA_color[1] = 84; players.tank[2].RGBA_color[2] = 204; //blue
    players.tank[2].shooting_key = SDLK_y;
    players.tank[2].directions[0] = SDLK_i; players.tank[2].directions[1] = SDLK_k; players.tank[2].directions[2] = SDLK_l; players.tank[2].directions[3] = SDLK_j; //up, down, right, left
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
                thickLineRGBA(renderer, (Sint16) ((n + 1) * BOX_WIDTH - 1), (Sint16) ((m + 1) * BOX_WIDTH), (Sint16) ((n + 2) * BOX_WIDTH + 1), (Sint16) ((m + 1) * BOX_WIDTH), 3, 111, 63, 46, 255);
            }
            if (vertical_walls[m][n]) {
                thickLineRGBA(renderer, (Sint16) ((n + 1) * BOX_WIDTH), (Sint16) ((m + 1) * BOX_WIDTH - 1), (Sint16) ((n + 1) * BOX_WIDTH), (Sint16) ((m + 2) * BOX_WIDTH + 1), 3, 111, 63, 46, 255);
            }
        }
    }
}

void draw_tank(PLAYERS *players) {
    for(int i = 0; i < players->number; i++){
        if(players->tank[i].life){
            filledCircleRGBA(renderer, (Sint16) players->tank[i].x, (Sint16) players->tank[i].y, (Sint16) (TANK_RADIUS), (Uint8) players->tank[i].RGBA_color[0], (Uint8) players->tank[i].RGBA_color[1], (Uint8) players->tank[i].RGBA_color[2], 255);
            thickLineRGBA(renderer, (Sint16) players->tank[i].x, (Sint16) players->tank[i].y, (Sint16) (players->tank[i].x + LENGTH * cos((players->tank[i].angle) * PI / 180)), (Sint16) (players->tank[i].y + LENGTH * sin((players->tank[i].angle) * PI / 180)), 4, 70, 70, 70, 255);
            filledCircleRGBA(renderer, (Sint16) players->tank[i].x, (Sint16) players->tank[i].y, (Sint16) (TANK_RADIUS / 2), 232, 232, 232, 255);
            for (int j = -3; j < 3; j++) {
                filledCircleRGBA(renderer, (Sint16) (players->tank[i].x + cos((players->tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (players->tank[i].y + sin((players->tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (TANK_RADIUS / 8), 232, 232, 232, 255);
            }
        }
    }
}

void draw_shot(PLAYERS *players) {
    for (int i = 0; i < players->number; i++) {
        for (int j = 0; j < MAX_BALLS; j++){
            if (players->tank[i].shot[j].time > 0) {
                shoot(&(players->tank[i].shot[j]));
                filledCircleRGBA(renderer, (Sint16) players->tank[i].shot[j].x, (Sint16) players->tank[i].shot[j].y, (Sint16) (SHOT_RADIUS), 0, 0, 0, 255);
            }
        }
    }
}

void drawing(PLAYERS *players) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_shot(players);
    draw_tank(players);
    SDL_RenderPresent(renderer);
}

void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void make_shot(PLAYERS *players, int i) {
    for (int j = 0; j < MAX_BALLS; j++) {
        if (players->tank[i].shot[j].time <= 0) {
            players->tank[i].shot[j].time = LIFE_OF_SHOT;
            players->tank[i].shot[j].x = players->tank[i].x + (TANK_RADIUS + SHOT_RADIUS) * cos(players->tank[i].angle * PI / 180);
            players->tank[i].shot[j].y = players->tank[i].y + (TANK_RADIUS + SHOT_RADIUS) * sin(players->tank[i].angle * PI / 180);;
            players->tank[i].shot[j].angle = players->tank[i].angle;
            break;
        }
    }
}