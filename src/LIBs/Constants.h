#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H


#define FPS 60
#define BOX_WIDTH 100
#define SCREEN_WIDTH (11 * BOX_WIDTH)
#define SCREEN_HEIGHT (10 * BOX_WIDTH)
#define START_MAP_X ((SCREEN_WIDTH - BOX_WIDTH * (max_boxes_x - 1)) / 2)
#define START_MAP_Y ((SCREEN_HEIGHT - BOX_WIDTH * (max_boxes_y - 1)) / 2)
#define FINISH_MAP_X ((int) (START_MAP_X + BOX_WIDTH * (max_boxes_x - 1)))
#define FINISH_MAP_Y ((int) (START_MAP_Y + BOX_WIDTH * (max_boxes_y - 1)))
#define SPEED ((double) FPS * 3.6 / BOX_WIDTH)
#define SPEED_OF_SHOT ((double) SPEED * 1.2)
#define OMEGA SPEED * 1.7
#define PI 3.141592653589
#define TANK_RADIUS ((double) BOX_WIDTH * 0.2)
#define SHOT_RADIUS ((double) BOX_WIDTH * 0.04)
#define LENGTH ((double) BOX_WIDTH * 0.28)
#define MAX_BALLS 5
#define LIFE_OF_SHOT (10 * FPS)
#define POWER_RADIUS ((double) BOX_WIDTH * 0.18)
#define POWER_NUMBER 2
#define POWER_MAKE_TIME (15 * FPS)
#define POWER_BOX_TIME (25 * FPS)
#define POWER_TIME (12 * FPS)
#define LASER_KILL_TIME (FPS / 5)
#define MINE_SHOW_TIME (3 * FPS)
#define MINE_HIDE_TIME (14 * FPS)
#define MINE_KILL_TIME (FPS)


#endif //PROJECT_CONSTANTS_H