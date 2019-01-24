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
#include "Audio.h"

bool not_closed = 1;
LASER_BOX laser_box[2];
MINE_BOX mine_box[2];
int power_make_time = POWER_MAKE_TIME;
int play_time;
Sint16 colors[3][3] = {
        239, 0, 0,
        0, 132, 0,
        0, 84, 204
};
int directions[3][4] = {
        SDLK_UP, SDLK_DOWN, SDLK_RIGHT, SDLK_LEFT,
        SDLK_e, SDLK_d, SDLK_f, SDLK_s,
        SDLK_i, SDLK_k, SDLK_l, SDLK_j
};
int shooting_keys[] = {SDLK_RCTRL, SDLK_q, SDLK_y};


void tank_presetting() {
    for (Sint8 i = 0; i < 3; i++) {
        for (Sint8 j = 0; j < 3; j++) {
            players.tank[i].RGBA_color[j] = colors[i][j];
        }
        for (Sint8 j = 0; j < 4; j++) {
            players.tank[i].directions[j] = directions[i][j];
        }
        players.tank[i].shooting_key = shooting_keys[i];
    }
}

void setting() {
    srand(SDL_GetTicks());
    generate_map();
    players.lives = players.number;
    for (Sint8 i = 0; i < players.number; i++) {
        players.tank[i].shot_type = T_NORMAL;
        players.tank[i].life = true;
        players.tank[i].x = START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        players.tank[i].y = START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2);
        if ((i == 1 && players.tank[i - 1].x == players.tank[i].x && players.tank[i - 1].y == players.tank[i].y) || (i == 2 && ((players.tank[i - 1].x == players.tank[i].x && players.tank[i - 1].y == players.tank[i].y) || (players.tank[i - 2].x == players.tank[i].x && players.tank[i - 2].y == players.tank[i].y)))) {
            i--;
        }
        players.tank[i].angle = (Sint16) (rand() % 360);
        for (Sint8 j = 0; j < MAX_BALLS; j++) {
            players.tank[i].shot[j].time = 0;
        }
    }
    for (Sint8 i = 0; i < 2; i++) {
        laser_box[i].enable = false;
        mine_box[i].enable = false;
    }

    power_make_time = POWER_MAKE_TIME;
}

void make_shot(Sint8 i) {
    for (Sint8 j = 0; j < MAX_BALLS; j++) {
        if (players.tank[i].shot[j].time <= 0) {
            play_sound(SHOOT_S);
            players.tank[i].shot[j].time = LIFE_OF_SHOT;
            players.tank[i].shot[j].x = players.tank[i].x + (TANK_RADIUS + SHOT_RADIUS) * cos(players.tank[i].angle * PI / 180);
            players.tank[i].shot[j].y = players.tank[i].y + (TANK_RADIUS + SHOT_RADIUS) * sin(players.tank[i].angle * PI / 180);;
            players.tank[i].shot[j].angle = (Sint16) players.tank[i].angle;
            break;
        }
    }
}

void make_laser_box() {
    for (Sint8 i = 0; i < 2; i++) {
        if (!laser_box[i].enable) {
            laser_box[i].enable = true;
            laser_box[i].time = POWER_BOX_TIME;
            laser_box[i].center.x = (Sint16) (START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2));
            laser_box[i].center.y = (Sint16) (START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2));
            break;
        }
    }
}

void make_mine_box() {
    for (Sint8 i = 0; i < 2; i++) {
        if (!mine_box[i].enable) {
            mine_box[i].enable = true;
            mine_box[i].time = POWER_BOX_TIME;
            mine_box[i].center.x = (Sint16) (START_MAP_X + rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2));
            mine_box[i].center.y = (Sint16) (START_MAP_Y + rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH / 2));
            break;
        }
    }
}

void make_power() {
    if (rand() % 3) {
        play_sound(POWER_APPEAR_S);
        Sint8 n = (Sint8) (rand() % POWER_NUMBER);
        if (n == 0) { //LASER
            make_laser_box();
        } else if (n == 1) { //MINE
            make_mine_box();
        }
    }
}

void destroy_power() {
    for (Sint8 i = 0; i < players.number; i++) {
        if (players.tank[i].power.laser.enable && players.tank[i].power.laser.time <= 0) {
            players.tank[i].power.laser.enable = false;
            players.tank[i].power.laser.time = 0;
            players.tank[i].power.laser.target = -1;
            players.tank[i].shot_type = T_NORMAL;
        }
        if (players.tank[i].power.mine.enable && players.tank[i].power.mine.mode == M_CARRYING && players.tank[i].power.mine.carrying_time <= 0) {
            players.tank[i].power.mine.enable = false;
            players.tank[i].power.mine.carrying_time = 0;
            players.tank[i].shot_type = T_NORMAL;
        }
    }
}

void destroy_power_box() {
    for (Sint8 i = 0; i < 2; i++) {
        if (laser_box[i].enable && laser_box[i].time <= 0) {
            laser_box[i].enable = false;
            laser_box[i].time = 0;
        }
        if (mine_box[i].enable && mine_box[i].time <= 0) {
            mine_box[i].enable = false;
            mine_box[i].time = 0;
        }
    }
}

void power_box_handle() {
    if (!save_mode) {
        power_make_time--;
    }
    if (power_make_time <= 0) {
        power_make_time = POWER_MAKE_TIME;
        make_power();
    }
    destroy_power_box();
    destroy_power();
}

void play_game() {
    load_number = 1;
    menu_button_state = B_NEW_GAME;
    while (not_closed && players.state == P_PLAYING) {
        Uint32 one_left_delay = 0, one_left_counter = 0;
        if (!save_mode && players.lives <= 1) {
            play_sound(REGENERATION_S);
            setting();
        }
        keys[SDLK_ESCAPE % 501] = 0;
        while (players.lives >= 1 && players.state == P_PLAYING) {
            handle_sounds();
            play_sound(BACKGROUND_S);
            int start_ticks = SDL_GetTicks();
            if (events() == -1) {
                not_closed = 0;
                break;
            }
            death_check();
            power_box_handle();
            drawing();
            if (players.lives == 1 && !one_left_counter) {
                one_left_delay = SDL_GetTicks();
                one_left_counter++;
            }
            if (players.lives == 1 && SDL_GetTicks() - one_left_delay >= 3000) {
                break;
            }
            while (SDL_GetTicks() - start_ticks < 1000 / FPS);
            if (!save_mode) {
                play_time++;
            }
        }
        if (!save_mode && players.lives <= 1) {
            set_score();
        }
        if (not_closed && !players.lives) {
            SDL_Delay(3000);
            if (!save_mode) {
                play_time += 3 * FPS;
            }
        }
    }
}