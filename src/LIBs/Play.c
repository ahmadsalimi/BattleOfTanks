//
// Created by nik on 12/26/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include "Play.h"
#include "View.h"
#include "Physics.h"
#include "Logic.h"


bool flag = 1;

void play_game(){
    while (flag) {
        int one_left_delay = 0, one_left_counter = 0;
        setting();
        int start_ticks = SDL_GetTicks();
        while (players.lives >= 1) {
            if (events() == -1) {
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
}