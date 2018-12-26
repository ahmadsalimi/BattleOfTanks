#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include "Struct.h"

void tank_presetting();

void setting();

void show_window();

void read_images();

void show_starting_menu();

void waiting_for_start();

void drawing();

void Quit();

void destroy_starting();

void make_shot(int i);

extern PLAYERS players;
extern Sint8 multiplayer_state;

#endif //PROJECT_VIEW_H

