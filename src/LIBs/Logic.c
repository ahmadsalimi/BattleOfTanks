#include <math.h>
#include "Logic.h"
#include "Struct.h"
#include "View.h"
#include "Constants.h"
#include "Audio.h"

void death_check() {
    for (int i = 0; i < players.number; i++) {
        for (int j = 0; players.tank[i].life && j < players.number; j++) {
            for (int k = 0; k < MAX_BALLS; k++) {
                if (players.tank[j].shot[k].time > 0 && pow(players.tank[i].x - players.tank[j].shot[k].x, 2) + pow(players.tank[i].y - players.tank[j].shot[k].y, 2) < pow(TANK_RADIUS, 2)) {
                    play_sound(EXPLOSION);
                    players.tank[j].shot[k].time = 0;
                    players.tank[i].life = 0;
                    players.tank[i].shot_type = 0;
                    (players.lives)--;
                }
            }
        }
    }
}

void laser_kill(int i) {
    if (players.tank[i].power.laser.target_counter) {
        players.tank[players.tank[i].power.laser.target].life = 0;
        players.tank[players.tank[i].power.laser.target].shot_type = 0;
        (players.lives)--;
    }
    players.tank[i].shot_type = 0;
    players.tank[i].power.laser.kill_time = LASER_KILL_TIME;
    players.tank[i].power.laser.target_counter = 0;
    players.tank[i].power.laser.target = -1;
    players.tank[i].power.laser.time = 0;
    players.tank[i].power.laser.enable = 0;
    players.tank[i].power.laser.targets[0] = -1;
    players.tank[i].power.laser.targets[1] = -1;
}

void mine_kill(int i) {
    play_sound(EXPLOSION);
    players.tank[players.tank[i].power.mine.target].life = 0;
    players.tank[players.tank[i].power.mine.target].shot_type = 0;
    (players.lives)--;
    players.tank[i].power.mine.mode = 3;
    players.tank[i].power.mine.kill_time = MINE_KILL_TIME;
}

void set_score() {
    for (int i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            (players.tank[i].score)++;
        }
        if (players.tank[i].score >= finish_point) {
            pause_background();
            play_sound(FINISH);
            players.state = 3;
        }
    }
}