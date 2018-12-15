//
// Created by nik on 12/15/2018.
//

#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

typedef struct TANK {
    int life;
    double x;
    double y;
    int angle;
}TANK;

typedef struct SHOT {
    int time;
    double x;
    double y;
    int angle;
}SHOT;

#endif //PROJECT_STRUCT_H
