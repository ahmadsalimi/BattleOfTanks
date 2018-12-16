//
// Created by nik on 12/15/2018.
//
#include "View.h"
#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H


#define FPS 60
#define BOX_WIDTH 90
#define SCREEN_WIDTH max_boxes_x * BOX_WIDTH + 1
#define SCREEN_HEIGHT max_boxes_y * BOX_WIDTH + 1
#define SPEED BOX_WIDTH / FPS * 2.5
#define OMEGA SPEED * 2
#define PI 3.141592653589
#define TANK_RADIUS BOX_WIDTH * 0.2
#define SHOT_RADIUS BOX_WIDTH * 0.04
#define LENGTH BOX_WIDTH * 0.28
#define MAX_BALLS 30
#define LIFE_OF_SHOT 20 * FPS


#endif //PROJECT_CONSTANTS_H
