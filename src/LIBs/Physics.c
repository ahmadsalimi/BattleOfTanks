//
// Created by nik on 12/15/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include "Physics.h"
#include "View.h"
#include "Constants.h"
#include "Struct.h"
#include "MapGenerate.h"


int keys[401];
int shooting_flag = 0;


void shoot(SHOT *shot) {
    if (shot->x >= FINISH_MAP_X - SHOT_RADIUS || ((shot->x) / BOX_WIDTH < (int) (shot->x + 2 * SPEED * cos((shot->angle) * PI / 180)) / BOX_WIDTH && vertical_walls[(int) (shot->y + 2 * SPEED * sin((shot->angle) * PI / 180)) / BOX_WIDTH - 1][(int) (shot->x + 2 * SPEED * cos((shot->angle) * PI / 180)) / BOX_WIDTH - 1])) {
        shot->angle = 180 - shot->angle;
        (shot->x)--;
    } else if (shot->x <= START_MAP_X + SHOT_RADIUS || ((shot->x) / BOX_WIDTH > (int) (shot->x + 2 * SPEED * cos((shot->angle) * PI / 180)) / BOX_WIDTH && vertical_walls[(shot->y) / BOX_WIDTH - 1][(shot->x) / BOX_WIDTH - 1])) {
        shot->angle = 180 - shot->angle;
        (shot->x)++;
    }
    if (shot->y >= FINISH_MAP_Y - SHOT_RADIUS || ((shot->y) / BOX_WIDTH < (int) (shot->y + 2 * SPEED * sin((shot->angle) * PI / 180)) / BOX_WIDTH && horizontal_walls[(int) (shot->y + 2 * SPEED * sin((shot->angle) * PI / 180)) / BOX_WIDTH - 1][(int) (shot->x + 2 * SPEED * cos((shot->angle) * PI / 180)) / BOX_WIDTH - 1])) {
        shot->angle = -shot->angle;
        (shot->y)--;
    } else if (shot->y <= START_MAP_Y + SHOT_RADIUS || ((shot->y) / BOX_WIDTH > (int) (shot->y + 2 * SPEED * sin((shot->angle) * PI / 180)) / BOX_WIDTH && horizontal_walls[(shot->y) / BOX_WIDTH - 1][(shot->x) / BOX_WIDTH - 1])) {
        shot->angle = -shot->angle;
        (shot->y)++;
    }
    shot->angle %= 360;
    shot->x = (int) (shot->x + 2 * SPEED * cos((shot->angle) * PI / 180));
    shot->y = (int) (shot->y + 2 * SPEED * sin((shot->angle) * PI / 180));
    (shot->time)--;
}

void tank_motion(TANK *tank) {
    //moving
    tank->x += SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);
    tank->y += SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);

    struct POINT box[4] = {
            {((int) tank->x / BOX_WIDTH) * BOX_WIDTH,     ((int) tank->y / BOX_WIDTH) * BOX_WIDTH},
            {((int) tank->x / BOX_WIDTH) * BOX_WIDTH,     ((int) tank->y / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) tank->x / BOX_WIDTH + 1) * BOX_WIDTH, ((int) tank->y / BOX_WIDTH) * BOX_WIDTH},
            {((int) tank->x / BOX_WIDTH + 1) * BOX_WIDTH, ((int) tank->y / BOX_WIDTH + 1) * BOX_WIDTH}
    };

    //checking confluence for heads of walls

    for (int i = 0; i < 4; i++) {
        if (pow(box[i].x - tank->x, 2) + pow(box[i].y - tank->y, 2) <= TANK_RADIUS * TANK_RADIUS || (pow(box[i].x - (tank->x + LENGTH * cos(tank->angle * PI / 180)), 2) + pow(box[i].y - (tank->y + LENGTH * sin(tank->angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS) / 8 ||
            (pow(box[i].x - (tank->x + TANK_RADIUS * cos(tank->angle * PI / 180)), 2) + pow(box[i].y - (tank->y + TANK_RADIUS * sin(tank->angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS)) {
            if ((box[i].x + 1 > 2 * BOX_WIDTH && horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 2]) ^ (horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1]) && !((box[i].y > 2 * BOX_WIDTH && vertical_walls[box[i].y / BOX_WIDTH - 2][box[i].x / BOX_WIDTH - 1]) && (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1]))) {
                tank->x -= SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) + horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1] - horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 2];
                tank->y -= SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) - 1;
            } else if ((box[i].y + 1 > 2 * BOX_WIDTH && vertical_walls[box[i].y / BOX_WIDTH - 2][box[i].x / BOX_WIDTH - 1]) ^ (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1]) && !((box[i].x > 2 * BOX_WIDTH && horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 2]) && (horizontal_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1]))) {
                tank->x -= SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) - 1;
                tank->y -= SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) + vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH - 1] - vertical_walls[box[i].y / BOX_WIDTH - 2][box[i].x / BOX_WIDTH - 1];
            }
        }
    }


    //checking confluence for vertical walls
    double max_x_left = tank->x + TANK_RADIUS, max_x_right = tank->x - TANK_RADIUS, Y = tank->y;
    if (cos((tank->angle) * PI / 180) > 0) {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (LENGTH * cos((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_x_left = tank->x + LENGTH * cos((tank->angle) * PI / 180);
                Y = tank->y + LENGTH * sin((tank->angle) * PI / 180);
            }
            if (max_x_left >= FINISH_MAP_X || ((int) tank->x / BOX_WIDTH < (int) max_x_left / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) max_x_left / BOX_WIDTH - 1])) {
                tank->x -= SPEED * cos(tank->angle * PI / 180) + 1;
            }
        } else {
            if (max_x_right <= START_MAP_X || ((int) tank->x / BOX_WIDTH > (int) max_x_right / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) tank->x / BOX_WIDTH - 1])) {
                tank->x += SPEED * cos(tank->angle * PI / 180) + 1;
            }
        }
    } else {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (-LENGTH * cos((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_x_right = tank->x + LENGTH * cos((tank->angle) * PI / 180);
                Y = tank->y + LENGTH * sin((tank->angle) * PI / 180);
            }
            if (max_x_right <= START_MAP_X || ((int) tank->x / BOX_WIDTH > (int) max_x_right / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) tank->x / BOX_WIDTH - 1])) {
                (tank->x) -= SPEED * cos(tank->angle * PI / 180) - 1;
            }
        } else {
            if (max_x_left >= FINISH_MAP_X || ((int) tank->x / BOX_WIDTH < (int) max_x_left / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH][(int) max_x_left / BOX_WIDTH])) {
                (tank->x) += SPEED * cos(tank->angle * PI / 180) - 1;
            }
        }
    }
    //checking confluence for horizontal walls
    double max_y_up = tank->y - TANK_RADIUS, max_y_down = tank->y + TANK_RADIUS, X = tank->x;
    if (sin((tank->angle) * PI / 180) < 0) {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (-LENGTH * sin((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_y_up = tank->y + LENGTH * sin((tank->angle) * PI / 180);
                X = tank->x + LENGTH * cos((tank->angle) * PI / 180);
            }
            if (max_y_up <= START_MAP_Y || ((int) tank->y / BOX_WIDTH > (int) max_y_up / BOX_WIDTH && horizontal_walls[(int) max_y_up / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (tank->y) -= SPEED * sin(tank->angle * PI / 180) - 1;
            }
        } else {
            if (max_y_down >= FINISH_MAP_Y || ((int) tank->y / BOX_WIDTH < (int) max_y_down / BOX_WIDTH && horizontal_walls[(int) tank->y / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (tank->y) += SPEED * sin(tank->angle * PI / 180) - 1;
            }
        }
    } else {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (LENGTH * sin((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_y_down = tank->y + LENGTH * sin((tank->angle) * PI / 180);
                X = tank->x + LENGTH * cos((tank->angle) * PI / 180);
            }
            if (max_y_down >= FINISH_MAP_Y || ((int) tank->y / BOX_WIDTH < (int) max_y_down / BOX_WIDTH && horizontal_walls[(int) tank->y / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (tank->y) -= SPEED * sin(tank->angle * PI / 180) + 1;
            }
        } else {
            if (max_y_up <= START_MAP_Y || ((int) tank->y / BOX_WIDTH > (int) max_y_up / BOX_WIDTH && horizontal_walls[(int) max_y_up / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (tank->y) += SPEED * sin(tank->angle * PI / 180) + 1;
            }
        }
    }
}

void tank_rotation(TANK *tank) {
    tank->angle -= (int) (OMEGA * (keys[SDLK_LEFT % 401] - keys[SDLK_RIGHT % 401]));
}

int move(TANK *tank) {
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
            default:
                break;
        }
    }
    if (keys[SDLK_m % 401]) {
        if (!shooting_flag) {
            make_shot(tank);
        }
        shooting_flag = 1;
    }
    tank_motion(tank);
    tank_rotation(tank);
}