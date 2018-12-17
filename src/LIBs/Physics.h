//
// Created by nik on 12/15/2018.
//

#ifndef PROJECT_PHYSICS_H
#define PROJECT_PHYSICS_H

void shoot(int *x, int *y, int *angle, int *time);

void tank_motion(double *x, double *y, int *angle);

void tank_rotation(int *angle);

int move(double *x, double *y, int *angle);

#endif //PROJECT_PHYSICS_H
