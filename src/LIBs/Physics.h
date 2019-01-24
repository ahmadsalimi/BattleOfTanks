#ifndef PROJECT_PHYSICS_H
#define PROJECT_PHYSICS_H

#include "Struct.h"

double absolute(double x);

void shoot(SHOT *shot);

Sint8 menu_events();

Sint8 waiting_events();

Sint8 game_over_events();

Sint8 events();

#endif //PROJECT_PHYSICS_H