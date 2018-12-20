//
// Created by nik on 12/20/2018.
//

#ifndef PROJECT_MAPGENERATE_H
#define PROJECT_MAPGENERATE_H

#include "Struct.h"

void initialize();

MAP *connect_cells(MAP *n);

void render_walls();

int generate_map();

extern int max_boxes_x;
extern int max_boxes_y;
extern int vertical_walls[10][10];
extern int horizontal_walls[10][10];

#endif //PROJECT_MAPGENERATE_H
