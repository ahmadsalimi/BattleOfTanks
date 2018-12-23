#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include "Struct.h"


void setting();

void show_window();

void drawing(PLAYERS *players);

void Quit();

void make_shot(PLAYERS *players, int i);

extern PLAYERS players;

#endif //PROJECT_VIEW_H

