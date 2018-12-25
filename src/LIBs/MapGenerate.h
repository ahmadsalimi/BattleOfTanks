#include <stdbool.h>

#ifndef PROJECT_MAPGENERATE_H
#define PROJECT_MAPGENERATE_H

int generate_map();

extern int max_boxes_x;
extern int max_boxes_y;
extern bool vertical_walls[10][10];
extern bool horizontal_walls[10][10];

#endif //PROJECT_MAPGENERATE_H
