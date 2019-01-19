#include <math.h>
#include "Logic.h"
#include "Struct.h"
#include "View.h"
#include "Constants.h"

void death_check() {
    for (int i = 0; i < players.number; i++) {
        for (int j = 0; players.tank[i].life && j < players.number; j++) {
            for (int k = 0; k < MAX_BALLS; k++) {
                if (players.tank[j].shot[k].time > 0 && pow(players.tank[i].x - players.tank[j].shot[k].x, 2) + pow(players.tank[i].y - players.tank[j].shot[k].y, 2) < pow(TANK_RADIUS, 2)) {
                    players.tank[j].shot[k].time = 0;
                    players.tank[i].life = 0;
                    (players.lives)--;
                }
            }
        }
    }
}

void set_score() {
    for (int i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            (players.tank[i].score)++;
        }
        if (players.tank[i].score >= finish_point) {
            players.state = 3;
        }
    }
}