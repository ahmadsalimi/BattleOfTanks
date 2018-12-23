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
int shooting_flag[3] = {0};

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
    shot->x = shot->x + SPEED_OF_SHOT * cos((shot->angle) * PI / 180);
    shot->y = shot->y + SPEED_OF_SHOT * sin((shot->angle) * PI / 180);
    (shot->time)--;
}

void wall_heads(PLAYERS *game, int i) {
    POINT box[4] = {
            {((int) (game->tank[i].x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (game->tank[i].y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH},
            {((int) (game->tank[i].x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (game->tank[i].y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (game->tank[i].x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (game->tank[i].y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (game->tank[i].x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (game->tank[i].y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH}
    };
    for (int j = 0; j < 4; j++) {
        if (pow(box[j].x - (game->tank[i].x - START_MAP_X), 2) + pow(box[j].y - (game->tank[i].y - START_MAP_Y), 2) <= TANK_RADIUS * TANK_RADIUS || (pow(box[j].x - ((game->tank[i].x - START_MAP_X) + LENGTH * cos(game->tank[i].angle * PI / 180)), 2) + pow(box[j].y - ((game->tank[i].y - START_MAP_Y) + LENGTH * sin(game->tank[i].angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS) / 8 || (pow(box[j].x - ((game->tank[i].x - START_MAP_X) + TANK_RADIUS * cos(game->tank[i].angle * PI / 180)), 2) + pow(box[j].y - ((game->tank[i].y - START_MAP_Y) + TANK_RADIUS * sin(game->tank[i].angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS)) {
            if ((box[j].x + 1 > BOX_WIDTH && horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH - 1]) ^ (horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH]) && !((box[j].y > BOX_WIDTH && vertical_walls[box[j].y / BOX_WIDTH - 1][box[j].x / BOX_WIDTH]) && (vertical_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH]))) {

                game->tank[i].x -= SPEED * cos(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]) + horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH] - horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH - 1];
                game->tank[i].y -= SPEED * sin(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]) - 1;
            } else if ((box[j].y + 1 > BOX_WIDTH && vertical_walls[box[j].y / BOX_WIDTH - 1][box[j].x / BOX_WIDTH]) ^ (vertical_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH]) && !((box[j].x > BOX_WIDTH && horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH - 1]) && (horizontal_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH]))) {
                game->tank[i].x -= SPEED * cos(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]) - 1;
                game->tank[i].y -= SPEED * sin(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]) + vertical_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH] - vertical_walls[box[j].y / BOX_WIDTH - 1][box[j].x / BOX_WIDTH];
            }
        }
    }

}

void wall_confluence(PLAYERS *game, int i) {
    //checking confluence for vertical walls
    double max_x_left = game->tank[i].x + TANK_RADIUS + 2, max_x_right = game->tank[i].x - TANK_RADIUS - 2, Y = game->tank[i].y;
    if (cos((game->tank[i].angle) * PI / 180) > 0) {
        if (!keys[game->tank[i].directions[1] % 401] && (keys[game->tank[i].directions[0] % 401] || keys[game->tank[i].directions[2] % 401] || keys[game->tank[i].directions[3] % 401])) {
            if (LENGTH * cos((game->tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_x_left = game->tank[i].x + LENGTH * cos((game->tank[i].angle) * PI / 180) + 2;
                Y = game->tank[i].y + LENGTH * sin((game->tank[i].angle) * PI / 180);
            }
            if (max_x_left >= FINISH_MAP_X || ((int) game->tank[i].x / BOX_WIDTH < (int) max_x_left / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) max_x_left / BOX_WIDTH - 1])) {
                game->tank[i].x -= SPEED * cos(game->tank[i].angle * PI / 180) + 1;
            }
        } else {
            if (max_x_right <= START_MAP_X || ((int) game->tank[i].x / BOX_WIDTH > (int) max_x_right / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) game->tank[i].x / BOX_WIDTH - 1])) {
                game->tank[i].x += SPEED * cos(game->tank[i].angle * PI / 180) + 1;
            }
        }
    } else {
        if (!keys[game->tank[i].directions[1] % 401] && (keys[game->tank[i].directions[0] % 401] || keys[game->tank[i].directions[2] % 401] || keys[game->tank[i].directions[3] % 401])) {
            if (-LENGTH * cos((game->tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_x_right = game->tank[i].x + LENGTH * cos((game->tank[i].angle) * PI / 180) - 2;
                Y = game->tank[i].y + LENGTH * sin((game->tank[i].angle) * PI / 180);
            }
            if (max_x_right <= START_MAP_X || ((int) game->tank[i].x / BOX_WIDTH > (int) max_x_right / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) game->tank[i].x / BOX_WIDTH - 1])) {
                (game->tank[i].x) -= SPEED * cos(game->tank[i].angle * PI / 180) - 1;
            }
        } else {
            if (max_x_left >= FINISH_MAP_X || ((int) game->tank[i].x / BOX_WIDTH < (int) max_x_left / BOX_WIDTH && vertical_walls[(int) Y / BOX_WIDTH - 1][(int) max_x_left / BOX_WIDTH - 1])) {
                (game->tank[i].x) += SPEED * cos(game->tank[i].angle * PI / 180) - 1;
            }
        }
    }
    //checking confluence for horizontal walls
    double max_y_up = game->tank[i].y - TANK_RADIUS - 2, max_y_down = game->tank[i].y + TANK_RADIUS + 2, X = game->tank[i].x;
    if (sin((game->tank[i].angle) * PI / 180) < 0) {
        if (!keys[game->tank[i].directions[1] % 401] && (keys[game->tank[i].directions[0] % 401] || keys[game->tank[i].directions[2] % 401] || keys[game->tank[i].directions[3] % 401])) {
            if (-LENGTH * sin((game->tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_y_up = game->tank[i].y + LENGTH * sin((game->tank[i].angle) * PI / 180) - 2;
                X = game->tank[i].x + LENGTH * cos((game->tank[i].angle) * PI / 180);
            }
            if (max_y_up <= START_MAP_Y || ((int) game->tank[i].y / BOX_WIDTH > (int) max_y_up / BOX_WIDTH && horizontal_walls[(int) max_y_up / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (game->tank[i].y) -= SPEED * sin(game->tank[i].angle * PI / 180) - 1;
            }
        } else {
            if (max_y_down >= FINISH_MAP_Y || ((int) game->tank[i].y / BOX_WIDTH < (int) max_y_down / BOX_WIDTH && horizontal_walls[(int) game->tank[i].y / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (game->tank[i].y) += SPEED * sin(game->tank[i].angle * PI / 180) - 1;
            }
        }
    } else {
        if (!keys[game->tank[i].directions[1] % 401] && (keys[game->tank[i].directions[0] % 401] || keys[game->tank[i].directions[2] % 401] || keys[game->tank[i].directions[3] % 401])) {
            if (LENGTH * sin((game->tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_y_down = game->tank[i].y + LENGTH * sin((game->tank[i].angle) * PI / 180) + 2;
                X = game->tank[i].x + LENGTH * cos((game->tank[i].angle) * PI / 180);
            }
            if (max_y_down >= FINISH_MAP_Y || ((int) game->tank[i].y / BOX_WIDTH < (int) max_y_down / BOX_WIDTH && horizontal_walls[(int) game->tank[i].y / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (game->tank[i].y) -= SPEED * sin(game->tank[i].angle * PI / 180) + 1;
            }
        } else {
            if (max_y_up <= START_MAP_Y || ((int) game->tank[i].y / BOX_WIDTH > (int) max_y_up / BOX_WIDTH && horizontal_walls[(int) max_y_up / BOX_WIDTH][(int) X / BOX_WIDTH - 1])) {
                (game->tank[i].y) += SPEED * sin(game->tank[i].angle * PI / 180) + 1;
            }
        }
    }
}

void tank_motion(PLAYERS *game, int i) {
    //moving
    game->tank[i].x += SPEED * cos(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]);
    game->tank[i].y += SPEED * sin(game->tank[i].angle * PI / 180) * (keys[game->tank[i].directions[0] % 401] - keys[game->tank[i].directions[1] % 401]);
    wall_heads(game, i); //checking confluence between tank and wall heads
    wall_confluence(game, i);
}

void tank_rotation(PLAYERS *game, int i) {
    game->tank[i].angle -= (int) (OMEGA * (keys[game->tank[i].directions[3] % 401] - keys[game->tank[i].directions[2] % 401])) % 360;
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
                for (int i = 0; i < game.number; i++){
                    if (event.key.keysym.sym == game.tank[i].shooting_key) {
                        printf("tank %d pressed shot!\n", i);
                        shooting_flag[i] = 0;
                    }
                }
                break;
            default:
                break;
        }
    }
}

int events(PLAYERS *game) {
    for (int i = 0; i < game->number; i++){
        if (get_keys() == -1) {
            return 0;
        }
        if (keys[game->tank[i].shooting_key % 401]) {
            if (!shooting_flag[i]) {
                make_shot(game, i);
            }
            shooting_flag[i] = 1;
        }
        tank_motion(game, i);
        tank_rotation(game, i);
    }
}