#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "Physics.h"
#include "View.h"
#include "Constants.h"
#include "Struct.h"
#include "MapGenerate.h"
#include "Play.h"
#include "Logic.h"


int keys[501];
bool shooting_flag[3] = {0};

SDL_Event event;

double absolute(double x) {
    return x > 0 ? x : -x;
}

void reflect_shot(SHOT *shot) {
    bool flag = 0;
    POINT box[4] = {
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (shot->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (shot->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (shot->y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (shot->x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (shot->y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH}
    };
    if (shot->x >= FINISH_MAP_X - SHOT_RADIUS || ((int) ((shot->x - START_MAP_X) / BOX_WIDTH) < (int) (((shot->x - START_MAP_X) + 2 * SPEED * cos((shot->angle) * PI / 180) + SHOT_RADIUS) / BOX_WIDTH) && vertical_walls[box[3].y / BOX_WIDTH][box[3].x / BOX_WIDTH])) {
        shot->angle = (Sint16) (180 - shot->angle);
        (shot->x)--;
        flag = 1;
    } else if (shot->x <= START_MAP_X + SHOT_RADIUS || ((int) ((shot->x - START_MAP_X) / BOX_WIDTH) > (int) (((shot->x - START_MAP_X) + 2 * SPEED * cos((shot->angle) * PI / 180) - SHOT_RADIUS) / BOX_WIDTH) && vertical_walls[box[3].y / BOX_WIDTH][box[3].x / BOX_WIDTH - 1])) {
        shot->angle = (Sint16) (180 - shot->angle);
        (shot->x)++;
        flag = 1;
    }
    if (shot->y >= FINISH_MAP_Y - SHOT_RADIUS || ((int) ((shot->y - START_MAP_Y) / BOX_WIDTH) < (int) (((shot->y - START_MAP_Y) + 2 * SPEED * sin((shot->angle) * PI / 180) + SHOT_RADIUS) / BOX_WIDTH) && horizontal_walls[box[1].y / BOX_WIDTH][box[1].x / BOX_WIDTH])) {
        shot->angle = -shot->angle;
        (shot->y)--;
        flag = 1;
    } else if (shot->y <= START_MAP_Y + SHOT_RADIUS || ((int) ((shot->y - START_MAP_Y) / BOX_WIDTH) > (int) (((shot->y - START_MAP_Y) + 2 * SPEED * sin((shot->angle) * PI / 180) - SHOT_RADIUS) / BOX_WIDTH) && horizontal_walls[box[1].y / BOX_WIDTH - 1][box[1].x / BOX_WIDTH])) {
        shot->angle = -shot->angle;
        (shot->y)++;
        flag = 1;
    }
    if (flag) {
        for (int i = 0; i < 4; i++) {
            if (pow(box[i].x - (shot->x - START_MAP_X), 2) + pow(box[i].y - (shot->y - START_MAP_Y), 2) <= pow(SHOT_RADIUS + 1, 2) + 16) {
                if (cos(shot->angle * PI / 180) * sin(shot->angle * PI / 180) > 0 && box[i].y + 1 > BOX_WIDTH && box[i].x + 1 > BOX_WIDTH && (((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) && (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH])) || ((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH])))) { // _| or |`` walls
                    shot->angle = (Sint16) ((shot->angle + 180) % 360);
                }
                if (cos(shot->angle * PI / 180) * sin(shot->angle * PI / 180) < 0 && box[i].y + 1 > BOX_WIDTH && box[i].x + 1 > BOX_WIDTH && (((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH - 1]) && (vertical_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH])) || ((horizontal_walls[box[i].y / BOX_WIDTH][box[i].x / BOX_WIDTH]) && (vertical_walls[box[i].y / BOX_WIDTH - 1][box[i].x / BOX_WIDTH])))) { // |_ or ``| walls
                    shot->angle = (Sint16) ((shot->angle + 180) % 360);
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

void wall_heads(int i) {
    POINT box[4] = {
            {((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH},
            {((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH) * BOX_WIDTH,     ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH + 1) * BOX_WIDTH},
            {((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH + 1) * BOX_WIDTH, ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH) * BOX_WIDTH}
    };
    for (int j = 0; j < 4; j++) {
        if (pow(box[j].x - (players.tank[i].x - START_MAP_X), 2) + pow(box[j].y - (players.tank[i].y - START_MAP_Y), 2) <= TANK_RADIUS * TANK_RADIUS || (pow(box[j].x - ((players.tank[i].x - START_MAP_X) + LENGTH * cos(players.tank[i].angle * PI / 180)), 2) + pow(box[j].y - ((players.tank[i].y - START_MAP_Y) + LENGTH * sin(players.tank[i].angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS) / 8 || (pow(box[j].x - ((players.tank[i].x - START_MAP_X) + TANK_RADIUS * cos(players.tank[i].angle * PI / 180)), 2) + pow(box[j].y - ((players.tank[i].y - START_MAP_Y) + TANK_RADIUS * sin(players.tank[i].angle * PI / 180)), 2)) <= (LENGTH - TANK_RADIUS) * (LENGTH - TANK_RADIUS)) {
            if ((box[j].x > 0 && horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH - 1]) ^ (horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH]) && !((box[j].y > 0 && vertical_walls[box[j].y / BOX_WIDTH - 1][box[j].x / BOX_WIDTH]) && (vertical_walls[box[j].y / BOX_WIDTH][box[j].x / BOX_WIDTH]))) { // if there are one of horizontal walls and at least one of vertical walls
                players.tank[i].x -= SPEED * cos(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) + horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH] - horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH - 1];
                players.tank[i].y -= SPEED * sin(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) - 1;
                if (players.tank[i].y - START_MAP_Y < box[j].y) {
                    players.tank[i].y -= 2 * absolute(SPEED * sin(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) - 1);
                }
            } else if ((box[j].y > 0 && vertical_walls[(box[j].y + 1) / BOX_WIDTH - 1][(box[j].x + 1) / BOX_WIDTH]) ^ (vertical_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH]) && !((box[j].x > 0 && horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH - 1]) && (horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH]))) {// if there are one of vertical walls and at least one of horizontal walls
                players.tank[i].x -= SPEED * cos(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) - 1;
                players.tank[i].y -= SPEED * sin(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) + vertical_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH] - vertical_walls[(box[j].y + 1) / BOX_WIDTH - 1][(box[j].x + 1) / BOX_WIDTH];
                if (players.tank[i].x - START_MAP_X < box[j].x) {
                    players.tank[i].x -= 2 * absolute(SPEED * cos(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]) - 1);
                }
            } else if ((box[j].x > 0 && horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH - 1]) ^ (horizontal_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH]) && (box[j].y > 0 && vertical_walls[(box[j].y + 1) / BOX_WIDTH - 1][(box[j].x + 1) / BOX_WIDTH]) ^ (vertical_walls[(box[j].y + 1) / BOX_WIDTH][(box[j].x + 1) / BOX_WIDTH])) {//if there are one of horizontal and one of vertical walls
                players.tank[i].x += (absolute(SPEED * cos(players.tank[i].angle * PI / 180)) + 1) * (players.tank[i].x - START_MAP_X > box[j].x - players.tank[i].x - START_MAP_X < box[j].x);
                players.tank[i].y += (absolute(SPEED * sin(players.tank[i].angle * PI / 180)) + 1) * (players.tank[i].y - START_MAP_Y > box[j].y - players.tank[i].y - START_MAP_Y < box[j].y);
            }
        }
    }
}

void wall_confluence(int i) {
    //checking confluence for vertical walls
    double max_x_left = players.tank[i].x + TANK_RADIUS + 2, max_x_right = players.tank[i].x - TANK_RADIUS - 2, Y = players.tank[i].y;
    if (cos((players.tank[i].angle) * PI / 180) > 0) {
        if (!keys[players.tank[i].directions[1] % 501] && (keys[players.tank[i].directions[0] % 501] || keys[players.tank[i].directions[2] % 501] || keys[players.tank[i].directions[3] % 501])) {
            if (LENGTH * cos((players.tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_x_left = players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180) + 2;
                Y = players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180);
            }
            if (max_x_left >= FINISH_MAP_X || ((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH < (int) (max_x_left - START_MAP_X) / BOX_WIDTH && vertical_walls[(int) (Y - START_MAP_Y) / BOX_WIDTH][(int) (max_x_left - START_MAP_X) / BOX_WIDTH])) {
                players.tank[i].x -= SPEED * cos(players.tank[i].angle * PI / 180) + 1;
            }
        } else {
            if (max_x_right <= START_MAP_X || ((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH > (int) (max_x_right - START_MAP_X) / BOX_WIDTH && vertical_walls[(int) (Y - START_MAP_Y) / BOX_WIDTH][(int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH])) {
                players.tank[i].x += SPEED * cos(players.tank[i].angle * PI / 180) + 1;
            }
        }
    } else {
        if (!keys[players.tank[i].directions[1] % 501] && (keys[players.tank[i].directions[0] % 501] || keys[players.tank[i].directions[2] % 501] || keys[players.tank[i].directions[3] % 501])) {
            if (-LENGTH * cos((players.tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_x_right = players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180) - 2;
                Y = players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180);
            }
            if (max_x_right <= START_MAP_X || ((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH > (int) (max_x_right - START_MAP_X) / BOX_WIDTH && vertical_walls[(int) (Y - START_MAP_Y) / BOX_WIDTH][(int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].x) -= SPEED * cos(players.tank[i].angle * PI / 180) - 1;
            }
        } else {
            if (max_x_left >= FINISH_MAP_X || ((int) (players.tank[i].x - START_MAP_X) / BOX_WIDTH < (int) (max_x_left - START_MAP_X) / BOX_WIDTH && vertical_walls[(int) (Y - START_MAP_Y) / BOX_WIDTH][(int) (max_x_left - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].x) += SPEED * cos(players.tank[i].angle * PI / 180) - 1;
            }
        }
    }
    //checking confluence for horizontal walls
    double max_y_up = players.tank[i].y - TANK_RADIUS - 2, max_y_down = players.tank[i].y + TANK_RADIUS + 2, X = players.tank[i].x;
    if (sin((players.tank[i].angle) * PI / 180) < 0) {
        if (!keys[players.tank[i].directions[1] % 501] && (keys[players.tank[i].directions[0] % 501] || keys[players.tank[i].directions[2] % 501] || keys[players.tank[i].directions[3] % 501])) {
            if (-LENGTH * sin((players.tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_y_up = players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180) - 2;
                X = players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180);
            }
            if (max_y_up <= START_MAP_Y || ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH > (int) (max_y_up - START_MAP_Y) / BOX_WIDTH && horizontal_walls[(int) (max_y_up - START_MAP_Y) / BOX_WIDTH + 1][(int) (X - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].y) -= SPEED * sin(players.tank[i].angle * PI / 180) - 1;
            }
        } else {
            if (max_y_down >= FINISH_MAP_Y || ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH < (int) (max_y_down - START_MAP_Y) / BOX_WIDTH && horizontal_walls[(int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH + 1][(int) (X - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].y) += SPEED * sin(players.tank[i].angle * PI / 180) - 1;
            }
        }
    } else {
        if (!keys[players.tank[i].directions[1] % 501] && (keys[players.tank[i].directions[0] % 501] || keys[players.tank[i].directions[2] % 501] || keys[players.tank[i].directions[3] % 501])) {
            if (LENGTH * sin((players.tank[i].angle) * PI / 180) > TANK_RADIUS) {
                max_y_down = players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180) + 2;
                X = players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180);
            }
            if (max_y_down >= FINISH_MAP_Y || ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH < (int) (max_y_down - START_MAP_Y) / BOX_WIDTH && horizontal_walls[(int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH + 1][(int) (X - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].y) -= SPEED * sin(players.tank[i].angle * PI / 180) + 1;
            }
        } else {
            if (max_y_up <= START_MAP_Y || ((int) (players.tank[i].y - START_MAP_Y) / BOX_WIDTH > (int) (max_y_up - START_MAP_Y) / BOX_WIDTH && horizontal_walls[(int) (max_y_up - START_MAP_Y) / BOX_WIDTH + 1][(int) (X - START_MAP_X) / BOX_WIDTH])) {
                (players.tank[i].y) += SPEED * sin(players.tank[i].angle * PI / 180) + 1;
            }
        }
    }
}

void mine_confluence(int i) {
    for (int j = 0; j < players.number; j++) {
        if (i != j && players.tank[j].shot_type == 2 && players.tank[j].power.mine.mode) {
            if (pow(players.tank[i].x - players.tank[j].power.mine.position.x, 2) + pow(players.tank[i].y - players.tank[j].power.mine.position.y, 2) < pow(TANK_RADIUS + POWER_RADIUS, 2)) {
                players.tank[j].power.mine.target = (Sint8) i;
                mine_kill(j);
            }
        }
    }
}

void tank_motion(int i) {
    //moving
    players.tank[i].x += SPEED * cos(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]);
    players.tank[i].y += SPEED * sin(players.tank[i].angle * PI / 180) * (keys[players.tank[i].directions[0] % 501] - keys[players.tank[i].directions[1] % 501]);
    wall_confluence(i);
    wall_heads(i); //checking confluence between tank and wall heads
    mine_confluence(i);
}

void tank_rotation(int i) {
    players.tank[i].angle -= (OMEGA * (keys[players.tank[i].directions[3] % 501] - keys[players.tank[i].directions[2] % 501]));
}

void check_power(int i) {
    if (laser_box->enable && !players.tank[i].shot_type) {
        if (pow(players.tank[i].x - laser_box->center.x, 2) + pow(players.tank[i].y - laser_box->center.y, 2) < pow(TANK_RADIUS + POWER_RADIUS, 2)) {
            players.tank[i].shot_type = 1;
            players.tank[i].power.laser.enable = 1;
            players.tank[i].power.laser.time = POWER_TIME;
            laser_box->enable = 0;
            laser_box->time = 0;
        }
    }
    if (mine_box->enable && !players.tank[i].shot_type) {
        if (pow(players.tank[i].x - mine_box->center.x, 2) + pow(players.tank[i].y - mine_box->center.y, 2) < pow(TANK_RADIUS + POWER_RADIUS, 2)) {
            players.tank[i].shot_type = 2;
            players.tank[i].power.mine.enable = 1;
            players.tank[i].power.mine.carrying_time = POWER_TIME;
            players.tank[i].power.mine.mode = 0;
            players.tank[i].power.mine.position.x = (Sint16) players.tank[i].x;
            players.tank[i].power.mine.position.y = (Sint16) players.tank[i].y;
            mine_box->enable = 0;
            mine_box->time = 0;
        }
    }
}

int get_keys() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym % 501] = 1;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym % 501] = 0;
                for (int i = 0; i < players.number; i++) {
                    if (event.key.keysym.sym == players.tank[i].shooting_key) {
                        shooting_flag[i] = 0;
                    }
                }
                break;
            default:
                break;
        }
    }
}

int menu_events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (menu_state == 0) {
        if (keys[SDLK_ESCAPE % 501] && menu_playtime) {
            players.state = 2;
            keys[SDLK_ESCAPE % 501] = 0;
        }
        if (keys[SDLK_DOWN % 501] && menu_button_state != 2) {
            menu_button_state++;
            keys[SDLK_DOWN % 501] = 0;
        }
        if (keys[SDLK_UP % 501] && menu_button_state != 0) {
            menu_button_state--;
            keys[SDLK_UP % 501] = 0;
        }
        if (keys[SDLK_RETURN % 501]) {
            if (menu_button_state == 0) { // New game
                menu_state = 1;
                play_time = 0;
            } else if (menu_button_state == 1) { // load
                menu_state = 2;
            } else { //exit
                return -1;
            }
            keys[SDLK_RETURN % 501] = 0;
        }
    } else if (menu_state == 1) {
        if (keys[SDLK_RIGHT % 501] && multiplayer_state == 2) {
            multiplayer_state = 3;
        }
        if (keys[SDLK_LEFT % 501] && multiplayer_state == 3) {
            multiplayer_state = 2;
        }
        if (keys[SDLK_RETURN % 501]) {
            players.number = multiplayer_state;
            players.state = 1;
            setting();
            keys[SDLK_RETURN % 501] = 0;
        }
    } else {

    }

}

int waiting_events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (keys[SDLK_RETURN % 501]) {
        players.state = 2;
        keys[SDLK_RETURN % 501] = 0;
        keys[SDLK_ESCAPE % 501] = 0;
    }
    if (keys[SDLK_UP % 501] && finish_point < 30000) {
        finish_point++;
        keys[SDLK_UP % 501] = 0;
    }
    if (keys[SDLK_DOWN % 501] && finish_point > 1) {
        finish_point--;
        keys[SDLK_DOWN % 501] = 0;
    }
}

int game_over_events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (keys[SDLK_ESCAPE % 501]) {
        players.state = 0;
        menu_state = 0;
        menu_playtime = 0;
        keys[SDLK_ESCAPE % 501] = 0;
        for (int i = 0; i < 3; i++) {
            players.tank[i].score = 0;
        }
    }
}

void leave_mine(int i) {
    players.tank[i].power.mine.show_time = MINE_SHOW_TIME;
    players.tank[i].power.mine.target = -1;
    players.tank[i].power.mine.mode = 1;
}

int events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (keys[SDLK_ESCAPE % 501]) {
        players.state = 0;
        menu_state = 0;
        keys[SDLK_ESCAPE % 501] = 0;
    }
    for (int i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            if (keys[players.tank[i].shooting_key % 501]) {
                if (!shooting_flag[i]) {
                    if (players.tank[i].shot_type == 0) {
                        make_shot(i);
                    } else if (players.tank[i].shot_type == 1) {
                        laser_kill(i);
                    } else if (players.tank[i].shot_type == 2) {
                        leave_mine(i);
                    }
                }
                shooting_flag[i] = 1;
            }
            tank_motion(i);
            tank_rotation(i);
            check_power(i);
        }
    }
}