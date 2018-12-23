#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include "LIBs/View.h"
#include "LIBs/Struct.h"
#include "LIBs/Physics.h"
#include "LIBs/Constants.h"
#include "LIBs/MapGenerate.h"
#include "LIBs/Logic.h"

#ifdef main
#undef main
#endif /*main*/

int main(int argc, char *argv[]) {
    show_window();
    setting();
    int start_ticks = SDL_GetTicks();
    while (1) {
        if (!events(&players)) break;
        drawing(&players);
        death_check(&players);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
    Quit();
    return 0;
}