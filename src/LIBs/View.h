//
// Created by nik on 12/15/2018.
//

#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include "Struct.h"

void setting();

void show_window();

void read_map();

void draw_map();

void draw_tank(TANK *tank);

void draw_shot();

void drawing(TANK *tank);

void Quit();

void make_shot(TANK *tank);

extern int max_boxes_x;
extern int max_boxes_y;
extern int vertical_walls[10][10];
extern int horizontal_walls[10][10];

#endif //PROJECT_VIEW_H

