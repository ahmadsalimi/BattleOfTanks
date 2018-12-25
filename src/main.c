#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <stdbool.h>
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
    bool flag = 1;
    show_window();
    read_images();
    while (flag) {
        int one_left_delay, one_left_counter = 0;
        setting();
        int start_ticks = SDL_GetTicks();
        while (players.lives >= 1) {
            if (!events()) {
                flag = 0;
                break;
            }
            death_check();
            drawing();
            if (players.lives == 1 && !one_left_counter) {
                one_left_delay = SDL_GetTicks();
                one_left_counter++;
            }
            if (players.lives == 1 && SDL_GetTicks() - one_left_delay >= 3000) {
                break;
            }
            while (SDL_GetTicks() - start_ticks < 1000 / FPS);
        }
        set_score();
        if (flag && !players.lives) SDL_Delay(3000);
    }
    Quit();
    return 0;
}