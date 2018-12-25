#include "View.h"

#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H


#define FPS 70
#define BOX_WIDTH 90
#define SCREEN_WIDTH 11 * BOX_WIDTH
#define SCREEN_HEIGHT 9 * BOX_WIDTH
#define START_MAP_X (SCREEN_WIDTH - BOX_WIDTH * (max_boxes_x - 1)) / 2
#define START_MAP_Y (SCREEN_HEIGHT - BOX_WIDTH * (max_boxes_y - 1)) / 2
#define FINISH_MAP_X START_MAP_X + BOX_WIDTH * (max_boxes_x - 1)
#define FINISH_MAP_Y START_MAP_Y + BOX_WIDTH * (max_boxes_y - 1)
#define SPEED (double) FPS * 3.6 / BOX_WIDTH
#define SPEED_OF_SHOT (double) SPEED * 1.2
#define OMEGA SPEED * 1.7
#define PI 3.141592653589
#define TANK_RADIUS (double) BOX_WIDTH * 0.2
#define SHOT_RADIUS (double) BOX_WIDTH * 0.04
#define LENGTH (double) BOX_WIDTH * 0.28
#define MAX_BALLS 5
#define LIFE_OF_SHOT 5 * FPS


#endif //PROJECT_CONSTANTS_H
