#include "LIBs/View.h"
#include "LIBs/Physics.h"
#include "LIBs/Play.h"


int main(int argc, char *argv[]) {
    players.state = 0;
    tank_presetting();
    show_window();
    read_images();
    while (flag) {
        show_starting_menu();
        waiting_for_start();
        play_game();
    }
    Quit();
    return 0;
}