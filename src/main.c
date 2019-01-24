#include "LIBs/View.h"
#include "LIBs/Physics.h"
#include "LIBs/Play.h"
#include "LIBs/Audio.h"


int main(int argc, char *argv[]) {
    players.state = P_MENU;
    tank_presetting();
    show_window();
    read_images();
    load_audio();
    play_sound(OPENING_S);
    while (not_closed) {
        show_starting_menu();
        waiting_for_start();
        play_game();
        game_over();
    }
    Quit();
    return 0;
}