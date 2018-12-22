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

void reflect_shot(SHOT *shot) {
    int flag = 0;
    POINT box[4] = {
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (shot->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (shot->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (shot->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (shot->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH}
    };
    if (shot->x >= FINISH_MAP_X - SHOT_RADIUS || ((int) (shot->x / BOX_WIDTH) < (int) ((shot->x + 2 * SPEED * cos((shot->angle) * PI / 180) + SHOT_RADIUS) / BOX_WIDTH) && vertical_walls[box[3].y / BOX_WIDTH][box[3].x / BOX_WIDTH])) {
        shot->angle = 180 - shot->angle;
        (shot->x)--;
        flag = 1;
    } else if (shot->x <= START_MAP_X + SHOT_RADIUS || ((int) (shot->x / BOX_WIDTH) > (int) ((shot->x + 2 * SPEED * cos((shot->angle) * PI / 180) - SHOT_RADIUS) / BOX_WIDTH) && vertical_walls[box[3].y / BOX_WIDTH][box[3].x / BOX_WIDTH - 1])) {
        shot->angle = 180 - shot->angle;
        (shot->x)++;
        flag = 1;
    }
    if (shot->y >= FINISH_MAP_Y - SHOT_RADIUS || ((int) (shot->y / BOX_WIDTH) < (int) ((shot->y + 2 * SPEED * sin((shot->angle) * PI / 180) + SHOT_RADIUS) / BOX_WIDTH) && horizontal_walls[box[1].y / BOX_WIDTH][box[1].x / BOX_WIDTH])) {
        shot->angle = -shot->angle;
        (shot->y)--;
        flag = 1;
    } else if (shot->y <= START_MAP_Y + SHOT_RADIUS || ((int) (shot->y) / BOX_WIDTH > (int) ((shot->y + 2 * SPEED * sin((shot->angle) * PI / 180) - SHOT_RADIUS) / BOX_WIDTH) && horizontal_walls[box[1].y / BOX_WIDTH - 1][box[1].x / BOX_WIDTH])) {
        shot->angle = -shot->angle;
        (shot->y)++;
        flag = 1;
    }
    if (flag){
        for (int i = 0; i < 4; i++) {
            if (pow(box[i].x - (shot->x - START_MAP_X), 2) + pow(box[i].y - (shot->y - START_MAP_Y), 2) <= pow(SHOT_RADIUS + 1, 2) + 16) {
                if (cos(shot->angle * PI / 180) * sin(shot->angle * PI / 180) > 0 && box[i].y + 1 > BOX_WIDTH && box[i].x + 1 > BOX_WIDTH && (((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) && (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH])) || ((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH])))) { // _| or |`` walls
                    shot->angle = (shot->angle + 180) % 360;
                }
                if (cos(shot->angle * PI / 180) * sin(shot->angle * PI / 180) < 0 && box[i].y + 1 > BOX_WIDTH && box[i].x + 1 > BOX_WIDTH && (((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) && (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH])) || ((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH])))) { // |_ or ``| walls
                    shot->angle = (shot->angle + 180) % 360;
                }
            }
        }
    }


}

void shoot(SHOT *shot) {
    reflect_shot(shot);
    shot->angle %= 360;
    shot->x = shot->x + 2 * SPEED * cos((shot->angle) * PI / 180);
    shot->y = shot->y + 2 * SPEED * sin((shot->angle) * PI / 180);
    (shot->time)--;
}

void wall_heads(TANK *tank) {
    POINT box[4] = {
            {((int) (tank->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (tank->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH},
            {((int) (tank->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (tank->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (tank->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (tank->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (tank->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (tank->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH}
    };
    for (int i = 0; i < 4; i++) {
        if (pow(box[i].x - (tank->x - START_MAP_X), 2) + pow(box[i].y - (tank->y - START_MAP_Y), 2) <= TANK_RADIUS * TANK_RADIUS || (pow(box[i].x - ((tank->x - START_MAP_X) + LENGTH * cos(tank->angle * PI / 180)), 2) + pow(box[i].y - ((tank->y - START_MAP_Y) + LENGTH * sin(tank->angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS) / 8 || (pow(box[i].x - ((tank->x - START_MAP_X) + TANK_RADIUS * cos(tank->angle * PI / 180)), 2) + pow(box[i].y - ((tank->y - START_MAP_Y) + TANK_RADIUS * sin(tank->angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS)) {
            if ((box[i].x + 1 > BOX_WIDTH && horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) ^ (horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && !((box[i].y > BOX_WIDTH && vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH]) && (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]))) {

                tank->x -= SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) + horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH] - horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1];
                tank->y -= SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) - 1;
            } else if ((box[i].y + 1 > BOX_WIDTH && vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH]) ^ (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && !((box[i].x > BOX_WIDTH && horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) && (horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]))) {
                tank->x -= SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) - 1;
                tank->y -= SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]) + vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH] - vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH];
            }
        }
    }

}

void wall_confluence(TANK *tank) {
    //checking confluence for vertical walls
    double max_x_left = tank->x + TANK_RADIUS + 2, max_x_right = tank->x - TANK_RADIUS - 2, Y = tank->y;
    if (cos((tank->angle) * PI / 180) > 0) {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (LENGTH * cos((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_x_left = tank->x + LENGTH * cos((tank->angle) * PI / 180) + 2;
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
                max_x_right = tank->x + LENGTH * cos((tank->angle) * PI / 180) - 2;
                Y = tank->y + LENGTH * sin((tank->angle) * PI / 180);
            }
            if (max_x_right <= START_MAP_X || ((int) tank->x / BOX_WIDTH > (int) max_x_right / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) tank->x / BOX_WIDTH - 1])) {
                (tank->x) -= SPEED * cos(tank->angle * PI / 180) - 1;
            }
        } else {
            if (max_x_left >= FINISH_MAP_X || ((int) tank->x / BOX_WIDTH < (int) max_x_left / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) max_x_left / BOX_WIDTH - 1])) {
                (tank->x) += SPEED * cos(tank->angle * PI / 180) - 1;
            }
        }
    }
    //checking confluence for horizontal walls
    double max_y_up = tank->y - TANK_RADIUS - 2, max_y_down = tank->y + TANK_RADIUS + 2, X = tank->x;
    if (sin((tank->angle) * PI / 180) < 0) {
        if (!keys[SDLK_DOWN % 401] && (keys[SDLK_UP % 401] || keys[SDLK_RIGHT % 401] || keys[SDLK_LEFT % 401])) {
            if (-LENGTH * sin((tank->angle) * PI / 180) > TANK_RADIUS) {
                max_y_up = tank->y + LENGTH * sin((tank->angle) * PI / 180) - 2;
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
                max_y_down = tank->y + LENGTH * sin((tank->angle) * PI / 180) + 2;
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

void tank_motion(TANK *tank) {
    //moving
    tank->x += SPEED * cos(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);
    tank->y += SPEED * sin(tank->angle * PI / 180) * (keys[SDLK_UP % 401] - keys[SDLK_DOWN % 401]);
    wall_heads(tank); //checking confluence between tank and wall heads
    wall_confluence(tank);
}

void tank_rotation(TANK *tank) {
    tank->angle -= (int) (OMEGA * (keys[SDLK_LEFT % 401] - keys[SDLK_RIGHT % 401])) % 360;
}

int get_keys() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return -1;
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
}

int events(TANK *tank) {
    if (get_keys() == -1) {
        return 0;
    }
    if (keys[SDLK_m % 401]) {
        if (!shooting_flag) {
            make_shot(tank);
        }
        //shooting_flag = 1;
    }
    tank_motion(tank);
    tank_rotation(tank);
}