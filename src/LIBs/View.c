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

PLAYERS game;

void setting() {
    srand((unsigned int) time(NULL));
    generate_map();
    game.number = 3;
    for (int i = 0; i < game.number; i++){
        game.tank[i].life = 1;
        game.tank[i].x = START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        game.tank[i].y = START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        game.tank[i].angle = rand() % 360;
        for (int j = 0; j < MAX_BALLS; j++) {
            game.tank[i].shot[j].time = 0;
        }
    }
    // PLAYER 0
    game.tank[0].RGBA_color[0] = 239; game.tank[0].RGBA_color[1] = 0; game.tank[0].RGBA_color[2] = 0;
    game.tank[0].shooting_key = SDLK_m;
    game.tank[0].directions[0] = SDLK_UP; game.tank[0].directions[1] = SDLK_DOWN; game.tank[0].directions[2] = SDLK_RIGHT; game.tank[0].directions[3] = SDLK_LEFT;
    // PLAYER 1
    game.tank[1].RGBA_color[0] = 0; game.tank[1].RGBA_color[1] = 132; game.tank[1].RGBA_color[2] = 0;
    game.tank[1].shooting_key = SDLK_q;
    game.tank[1].directions[0] = SDLK_e; game.tank[1].directions[1] = SDLK_d; game.tank[1].directions[2] = SDLK_f; game.tank[1].directions[3] = SDLK_s;
    // PLAYER 2
    game.tank[2].RGBA_color[0] = 0; game.tank[2].RGBA_color[1] = 168; game.tank[2].RGBA_color[2] = 234;
    game.tank[2].shooting_key = SDLK_y;
    game.tank[2].directions[0] = SDLK_i; game.tank[2].directions[1] = SDLK_k; game.tank[2].directions[2] = SDLK_l; game.tank[2].directions[3] = SDLK_j;
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

void draw_tank(PLAYERS *game) {
    for(int i = 0; i < game->number; i++){
        filledCircleRGBA(renderer, (Sint16) game->tank[i].x, (Sint16) game->tank[i].y, (Sint16) (TANK_RADIUS), (Uint8) game->tank[i].RGBA_color[0], (Uint8) game->tank[i].RGBA_color[1], (Uint8) game->tank[i].RGBA_color[2], 255);
        thickLineRGBA(renderer, (Sint16) game->tank[i].x, (Sint16) game->tank[i].y, (Sint16) (game->tank[i].x + LENGTH * cos((game->tank[i].angle) * PI / 180)), (Sint16) (game->tank[i].y + LENGTH * sin((game->tank[i].angle) * PI / 180)), 4, 70, 70, 70, 255);
        filledCircleRGBA(renderer, (Sint16) game->tank[i].x, (Sint16) game->tank[i].y, (Sint16) (TANK_RADIUS / 2), 232, 232, 232, 255);
        for (int j = -3; j < 3; j++) {
            filledCircleRGBA(renderer, (Sint16) (game->tank[i].x + cos((game->tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (game->tank[i].y + sin((game->tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (TANK_RADIUS / 8), 232, 232, 232, 255);
        }
    }
}

void draw_shot(PLAYERS *game) {
    for (int i = 0; i < game->number; i++) {
        for (int j = 0; j < MAX_BALLS; j++){
            if (game->tank[i].shot[j].time > 0) {
                shoot(&(game->tank[i].shot[j]));
                filledCircleRGBA(renderer, (Sint16) game->tank[i].shot[j].x, (Sint16) game->tank[i].shot[j].y, (Sint16) (SHOT_RADIUS), 0, 0, 0, 255);
            }
        }
    }
}

void drawing(PLAYERS *game) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_shot(game);
    draw_tank(game);
    SDL_RenderPresent(renderer);
}

void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void make_shot(PLAYERS *game, int i) {
    for (int j = 0; j < MAX_BALLS; j++) {
        if (game->tank[i].shot[j].time <= 0) {
            game->tank[i].shot[j].time = LIFE_OF_SHOT;
            game->tank[i].shot[j].x = game->tank[i].x + TANK_RADIUS / 2 * cos(game->tank[i].angle * PI / 180);
            game->tank[i].shot[j].y = game->tank[i].y + TANK_RADIUS / 2 * sin(game->tank[i].angle * PI / 180);;
            game->tank[i].shot[j].angle = game->tank[i].angle;
            break;
        }
    }
}