#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include "Struct.h"

void setting();

void show_window();

void drawing(PLAYERS *game);

void Quit();

void make_shot(PLAYERS *game, int i);

extern PLAYERS game;

#endif //PROJECT_VIEW_H

