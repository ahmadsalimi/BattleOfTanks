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
#include "LIBs/Play.h"


#ifdef main
#undef main
#endif /*main*/

int main(int argc, char *argv[]) {
    players.state = 0;
    tank_presetting();
    show_window();
    read_images();
    show_starting_menu();
    waiting_for_start();
    play_game();
    Quit();
    return 0;
}