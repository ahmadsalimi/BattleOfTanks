#include "LIBs/View.h"
#include "LIBs/Physics.h"
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