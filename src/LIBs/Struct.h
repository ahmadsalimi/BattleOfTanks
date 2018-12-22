#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

typedef struct {
    int life;
    double x;
    double y;
    int angle;
} TANK;

typedef struct {
    int time;
    double x;
    double y;
    int angle;
} SHOT;

typedef struct {
    int x;
    int y;
} POINT;

typedef struct {
    int x, y;
    void *parent;
    int wall;
    int dirs; //Directions that still haven't been explored
} MAP;

#endif //PROJECT_STRUCT_H
