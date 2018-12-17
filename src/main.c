#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "LIBs/View.h"
#include "LIBs/Struct.h"
#include "LIBs/Physics.h"
#include "LIBs/Constants.h"


#ifdef main
#undef main
#endif /*main*/

int main(int argc, char *argv[]) {
    setting();
    show_window();
    int start_ticks = SDL_GetTicks();
    struct TANK tank1 = {1, 100, 100, 0};
    while (1) {
        if (!move(&tank1.x, &tank1.y, &tank1.angle)) break;
        drawing(&tank1.x, &tank1.y, &tank1.angle);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
    Quit();
    return 0;
}