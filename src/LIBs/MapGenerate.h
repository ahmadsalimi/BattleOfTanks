#include <stdbool.h>
#include <SDL.h>
#ifndef PROJECT_MAPGENERATE_H
#define PROJECT_MAPGENERATE_H

int generate_map();

extern Sint8 max_boxes_x;
extern Sint8 max_boxes_y;
extern bool vertical_walls[10][10];
extern bool horizontal_walls[10][10];

#endif //PROJECT_MAPGENERATE_H
