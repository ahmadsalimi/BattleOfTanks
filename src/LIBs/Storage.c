#include "Storage.h"
#include "Struct.h"
#include "Logic.h"

bool save_mode;
bool file_checked;
FILE *storage;
int last_number;
int load_number = 1;

void check_storage() {
    storage = fopen("../storage.b", "r");
    char line[10000];
    last_number = 1;
    fgets(line, 10000, storage);
    while (1) {
        int num = 0;
        fscanf(storage, "%d", &num);
        if (num) {
            fgets(line, 10000, storage);
        } else {
            break;
        }
        last_number++;
    }
    file_checked = true;
    fclose(storage);
}

void load() {
    Sint8 x = 0, y = 0;
    srand(SDL_GetTicks());
    storage = fopen("../storage.b", "r");
    char line[10000];
    fgets(line, 10000, storage);
    int num = 0;
    while (num != load_number) {
        fscanf(storage, "%d ", &num);
        if (num == load_number) {
            fscanf(storage, "%d %d ", &(players.number), &(players.lives));
            for (Sint8 i = 0; i < players.number; i++) {
                fscanf(storage, "%d %lf %lf %lf %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", &players.tank[i].life, &players.tank[i].x, &players.tank[i].y, &players.tank[i].angle, &players.tank[i].score, &players.tank[i].shot_type, &players.tank[i].power.laser.enable, &players.tank[i].power.laser.start.x, &players.tank[i].power.laser.start.y, &players.tank[i].power.laser.finish.x, &players.tank[i].power.laser.finish.y, &players.tank[i].power.laser.target, &players.tank[i].power.laser.time, &players.tank[i].power.laser.target_counter, &players.tank[i].power.laser.targets[0], &players.tank[i].power.laser.targets[1], &players.tank[i].power.laser.kill, &players.tank[i].power.laser.kill_time, &players.tank[i].power.mine.enable, &players.tank[i].power.mine.mode, &players.tank[i].power.mine.position.x, &players.tank[i].power.mine.position.y, &players.tank[i].power.mine.target, &players.tank[i].power.mine.carrying_time, &players.tank[i].power.mine.show_time,
                       &players.tank[i].power.mine.hide_time, &players.tank[i].power.mine.kill_time);
                for (Sint8 j = 0; j < MAX_BALLS; j++) {
                    fscanf(storage, "%d %lf %lf %d ", &players.tank[i].shot[j].time, &players.tank[i].shot[j].x, &players.tank[i].shot[j].y, &players.tank[i].shot[j].angle);
                }
            }
            fscanf(storage, "%d %d ", &x, &y);
            for (Sint8 i = 0; i < 10; i++) {
                for (Sint8 j = 0; j < 10; j++) {
                    fscanf(storage, "%d ", &horizontal_walls[i][j]);
                }
            }
            for (Sint8 i = 0; i < 10; i++) {
                for (Sint8 j = 0; j < 10; j++) {
                    fscanf(storage, "%d ", &vertical_walls[i][j]);
                }
            }
            fscanf(storage, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", &laser_box[0].enable, &laser_box[0].time, &laser_box[0].center.x, &laser_box[0].center.y, &laser_box[1].enable, &laser_box[1].time, &laser_box[1].center.x, &laser_box[1].center.y, &mine_box[0].enable, &mine_box[0].time, &mine_box[0].center.x, &mine_box[0].center.y, &mine_box[1].enable, &mine_box[1].time, &mine_box[1].center.x, &mine_box[1].center.y, &power_make_time, &play_time, &finish_point);
            break;
        }
        if (num) {
            fgets(line, 10000, storage);
        } else {
            break;
        }
    }
    if (players.lives <= 1) {
        set_score();
    }
    fclose(storage);
    max_boxes_x = (Sint8) x;
    max_boxes_y = (Sint8) y;
}

void save() {
    storage = fopen("../storage.b", "a");
    fprintf(storage, "\n%d %d %d ", last_number, players.number, players.lives);
    for (Sint8 i = 0; i < players.number; i++) {
        fprintf(storage, "%d %lf %lf %lf %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", players.tank[i].life, players.tank[i].x, players.tank[i].y, players.tank[i].angle, players.tank[i].score, players.tank[i].shot_type, players.tank[i].power.laser.enable, players.tank[i].power.laser.start.x, players.tank[i].power.laser.start.y, players.tank[i].power.laser.finish.x, players.tank[i].power.laser.finish.y, players.tank[i].power.laser.target, players.tank[i].power.laser.time, players.tank[i].power.laser.target_counter, players.tank[i].power.laser.targets[0], players.tank[i].power.laser.targets[1], players.tank[i].power.laser.kill, players.tank[i].power.laser.kill_time, players.tank[i].power.mine.enable, players.tank[i].power.mine.mode, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, players.tank[i].power.mine.target, players.tank[i].power.mine.carrying_time, players.tank[i].power.mine.show_time, players.tank[i].power.mine.hide_time,
                players.tank[i].power.mine.kill_time);
        for (Sint8 j = 0; j < MAX_BALLS; j++) {
            fprintf(storage, "%d %lf %lf %d ", players.tank[i].shot[j].time, players.tank[i].shot[j].x, players.tank[i].shot[j].y, players.tank[i].shot[j].angle);
        }
    }
    fprintf(storage, "%d %d ", max_boxes_x, max_boxes_y);
    for (Sint8 i = 0; i < 10; i++) {
        for (Sint8 j = 0; j < 10; j++) {
            fprintf(storage, "%d ", horizontal_walls[i][j]);
        }
    }
    for (Sint8 i = 0; i < 10; i++) {
        for (Sint8 j = 0; j < 10; j++) {
            fprintf(storage, "%d ", vertical_walls[i][j]);
        }
    }
    fprintf(storage, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", laser_box[0].enable, laser_box[0].time, laser_box[0].center.x, laser_box[0].center.y, laser_box[1].enable, laser_box[1].time, laser_box[1].center.x, laser_box[1].center.y, mine_box[0].enable, mine_box[0].time, mine_box[0].center.x, mine_box[0].center.y, mine_box[1].enable, mine_box[1].time, mine_box[1].center.x, mine_box[1].center.y, power_make_time, play_time, finish_point);
    save_mode = false;
    last_number++;
    fclose(storage);
    file_checked = false;
}