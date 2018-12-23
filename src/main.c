#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include "LIBs/View.h"
#include "LIBs/Struct.h"
#include "LIBs/Physics.h"
#include "LIBs/Constants.h"
#include "LIBs/MapGenerate.h"

#ifdef main
#undef main
#endif /*main*/

int main(int argc, char *argv[]) {
    setting();
    show_window();
    int start_ticks = SDL_GetTicks();
    while (1) {
        if (!events(&game)) break;
        drawing(&game);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
    Quit();
    return 0;
}