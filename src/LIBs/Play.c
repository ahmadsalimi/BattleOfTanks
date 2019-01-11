#include <SDL.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "Play.h"
#include "View.h"
#include "Physics.h"
#include "Logic.h"
#include "Constants.h"
#include "MapGenerate.h"

bool flag = 1;

void tank_presetting() {
    // PLAYER 0
    players.tank[0].RGBA_color[0] = 239;
    players.tank[0].RGBA_color[1] = 0;
    players.tank[0].RGBA_color[2] = 0; //red
    players.tank[0].shooting_key = SDLK_RCTRL; //shoot; Right ctrl
    players.tank[0].directions[0] = SDLK_UP; //up;
    players.tank[0].directions[1] = SDLK_DOWN; //down;
    players.tank[0].directions[2] = SDLK_RIGHT; //right;
    players.tank[0].directions[3] = SDLK_LEFT; //left;
    // PLAYER 1
    players.tank[1].RGBA_color[0] = 0;
    players.tank[1].RGBA_color[1] = 132;
    players.tank[1].RGBA_color[2] = 0; //green
    players.tank[1].shooting_key = SDLK_q; //shoot;
    players.tank[1].directions[0] = SDLK_e; //up;
    players.tank[1].directions[1] = SDLK_d; //down;
    players.tank[1].directions[2] = SDLK_f; //right;
    players.tank[1].directions[3] = SDLK_s; //left;
    // PLAYER 2
    players.tank[2].RGBA_color[0] = 0;
    players.tank[2].RGBA_color[1] = 84;
    players.tank[2].RGBA_color[2] = 204; //blue
    players.tank[2].shooting_key = SDLK_y; //shoot;
    players.tank[2].directions[0] = SDLK_i; //up;
    players.tank[2].directions[1] = SDLK_k; //down;
    players.tank[2].directions[2] = SDLK_l; //right;
    players.tank[2].directions[3] = SDLK_j; //left;
}

void setting() {
    srand((unsigned int) time(NULL));
    generate_map();
    players.lives = players.number;
    for (int i = 0; i < players.number; i++) {
        players.tank[i].life = 1;
        players.tank[i].x = START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        players.tank[i].y = START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        if ((i == 1 && players.tank[i - 1].x == players.tank[i].x && players.tank[i - 1].y == players.tank[i].y) || (i == 2 && ((players.tank[i - 1].x == players.tank[i].x && players.tank[i - 1].y == players.tank[i].y) || (players.tank[i - 2].x == players.tank[i].x && players.tank[i - 2].y == players.tank[i].y)))) {
            i--;
        }
        players.tank[i].angle = (Sint16) (rand() % 360);
        for (int j = 0; j < MAX_BALLS; j++) {
            players.tank[i].shot[j].time = 0;
        }
    }
}

void make_shot(int i) {
    for (int j = 0; j < MAX_BALLS; j++) {
        if (players.tank[i].shot[j].time <= 0) {
            players.tank[i].shot[j].time = LIFE_OF_SHOT;
            players.tank[i].shot[j].x = players.tank[i].x + (TANK_RADIUS + SHOT_RADIUS) * cos(players.tank[i].angle * PI / 180);
            players.tank[i].shot[j].y = players.tank[i].y + (TANK_RADIUS + SHOT_RADIUS) * sin(players.tank[i].angle * PI / 180);;
            players.tank[i].shot[j].angle = players.tank[i].angle;
            break;
        }
    }
}

void play_game() {
    while (flag) {
        int one_left_delay = 0, one_left_counter = 0;
        setting();
        while (players.lives >= 1) {
            int start_ticks = SDL_GetTicks();
            if (events() == -1) {
                flag = 0;
                break;
            }
            death_check();
            drawing();
            if (players.lives == 1 && !one_left_counter) {
                one_left_delay = SDL_GetTicks();
                one_left_counter++;
            }
            if (players.lives == 1 && SDL_GetTicks() - one_left_delay >= 3000) {
                break;
            }
            while (SDL_GetTicks() - start_ticks < 1000 / FPS);
        }
        set_score();
        if (flag && !players.lives) SDL_Delay(3000);
    }
}