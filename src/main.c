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
    TANK tank1 = {1, rand() % (max_boxes_x - 2) * BOX_WIDTH + (int) (BOX_WIDTH * 3 / 2), rand() % (max_boxes_y - 2) * BOX_WIDTH + (int) (BOX_WIDTH * 3 / 2), rand() % 360};
    while (1) {
        if (!events(&tank1)) break;
        drawing(&tank1);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
    Quit();
    return 0;
}