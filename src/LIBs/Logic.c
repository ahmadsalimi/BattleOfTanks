#include "Struct.h"
#include "Audio.h"

void kill(Sint8 i) {
    players.tank[i].life = false;
    if (players.tank[i].shot_type != T_MINE || players.tank[i].power.mine.mode == M_CARRYING) {
        players.tank[i].shot_type = T_NORMAL;
    }
    (players.lives)--;
}

void death_check() {
    for (Sint8 i = 0; i < players.number; i++) {
        for (Sint8 j = 0; players.tank[i].life && j < players.number; j++) {
            for (Sint8 k = 0; k < MAX_BALLS; k++) {
                if (players.tank[j].shot[k].time > 0 && pow(players.tank[i].x - players.tank[j].shot[k].x, 2) + pow(players.tank[i].y - players.tank[j].shot[k].y, 2) < pow(TANK_RADIUS, 2)) {
                    play_sound(EXPLOSION_S);
                    players.tank[j].shot[k].time = 0;
                    kill(i);
                }
            }
        }
    }
}

void laser_kill(Sint8 i) {
    if (players.tank[i].power.laser.target_counter) {
        kill(players.tank[i].power.laser.target);
    }
    players.tank[i].shot_type = T_NORMAL;
    players.tank[i].power.laser.kill_time = LASER_KILL_TIME;
    players.tank[i].power.laser.target_counter = 0;
    players.tank[i].power.laser.target = -1;
    players.tank[i].power.laser.time = 0;
    players.tank[i].power.laser.enable = false;
    players.tank[i].power.laser.targets[0] = -1;
    players.tank[i].power.laser.targets[1] = -1;
}

void mine_kill(Sint8 i) {
    play_sound(EXPLOSION_S);
    kill(players.tank[i].power.mine.target);
    players.tank[i].power.mine.mode = M_KILLING;
    players.tank[i].power.mine.kill_time = MINE_KILL_TIME;
}

void set_score() {
    for (Sint8 i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            (players.tank[i].score)++;
        }
        if (players.tank[i].score >= finish_point) {
            pause_background();
            play_sound(FINISH_S);
            play_sound(GAME_OVER_S);
            players.state = P_GAME_OVER;
        }
    }
}