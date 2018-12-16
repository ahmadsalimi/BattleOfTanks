//
// Created by nik on 12/15/2018.
//

#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

void setting();

void show_window();

void read_map();

void draw_map();

void draw_tank(const double *x, const double *y, const int *angle);

void draw_shot();

void drawing(double *x, double *y, int *angle);

void Quit();

void make_shot(double x, double y, int angle);

extern int max_boxes_x;
extern int max_boxes_y;
extern int vertical_walls[10][10];
extern int horizontal_walls[10][10];

#endif //PROJECT_VIEW_H

