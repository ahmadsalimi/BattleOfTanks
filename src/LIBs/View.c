#include <SDL2_gfxPrimitives.h>
#include "Physics.h"
#include "Audio.h"
#include "Storage.h"


SDL_Window *window;
SDL_Renderer *renderer;
IMAGE player_points[3];
IMAGE logo;
IMAGE multiplayer[2];
IMAGE multiplayer_hover[2];
IMAGE key_images[3];
IMAGE menu_images[3];
IMAGE menu_images_hover[3];
SDL_Surface *icon;
PLAYERS players;
MENU_STATES menu_state;
MENU_BUTTON_STATE menu_button_state;
bool menu_playtime;
Sint8 multiplayer_state;
Sint16 finish_point = 1;
Sint32 winner;

void load_icon() {
    icon = SDL_LoadBMP("icon.bmp");
    if (icon == NULL) {
        SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
    }
}

void show_window() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Battle of Tanks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    load_icon();
    SDL_SetWindowIcon(window, icon);
}

void read_images() {
    // read scoreboard images
    for (Sint8 i = 0; i < 3; i++) {
        char filename[15];
        sprintf(filename, "player-%d.bmp", i + 1);
        player_points[i].surface = SDL_LoadBMP(filename);
        if (player_points[i].surface == NULL) {
            SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
        }
        player_points[i].texture = SDL_CreateTextureFromSurface(renderer, player_points[i].surface);
        if (player_points[i].texture == NULL) {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_QueryTexture(player_points[i].texture, NULL, NULL, &player_points[i].rect.w, &player_points[i].rect.h);
        player_points[i].rect.w /= 6;
        player_points[i].rect.h /= 6;
        player_points[i].rect.x = 200 + i * 250;
        player_points[i].rect.y = SCREEN_HEIGHT - player_points[i].rect.h * 4 / 3;
    }
    //read logo image
    logo.surface = SDL_LoadBMP("logo.bmp");
    if (logo.surface == NULL) {
        SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
    }
    logo.texture = SDL_CreateTextureFromSurface(renderer, logo.surface);
    if (logo.texture == NULL) {
        SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
    }
    SDL_QueryTexture(logo.texture, NULL, NULL, &logo.rect.w, &logo.rect.h);
    logo.rect.w = logo.rect.w * 2 / 3;
    logo.rect.h = logo.rect.h * 2 / 3;
    logo.rect.x = (SCREEN_WIDTH - logo.rect.w) / 2;
    logo.rect.y = (SCREEN_HEIGHT - 2 * logo.rect.h) / 2;
    //read menu images
    for (Sint8 i = 0; i < 3; ++i) {
        char address[10], hover_address[10];
        sprintf(address, "%d.bmp", i + 10);
        sprintf(hover_address, "%d-hover.bmp", i + 10);
        menu_images[i].surface = SDL_LoadBMP(address);
        menu_images_hover[i].surface = SDL_LoadBMP(hover_address);
        if (menu_images[i].surface == NULL || menu_images_hover[i].surface == NULL) {
            SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
        }
        menu_images[i].texture = SDL_CreateTextureFromSurface(renderer, menu_images[i].surface);
        menu_images_hover[i].texture = SDL_CreateTextureFromSurface(renderer, menu_images_hover[i].surface);
        if (menu_images[i].texture == NULL || menu_images_hover[i].texture == NULL) {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_QueryTexture(menu_images[i].texture, NULL, NULL, &menu_images[i].rect.w, &menu_images[i].rect.h);
        SDL_QueryTexture(menu_images_hover[i].texture, NULL, NULL, &menu_images_hover[i].rect.w, &menu_images_hover[i].rect.h);
        menu_images[i].rect.w = menu_images[i].rect.w / 5;
        menu_images[i].rect.h = menu_images[i].rect.h / 5;
        menu_images[i].rect.x = (SCREEN_WIDTH - menu_images[i].rect.w) / 2;
        menu_images[i].rect.y = SCREEN_HEIGHT / 2 + 30 + (menu_images[i].rect.h + 10) * i;
        menu_images_hover[i].rect.w = menu_images_hover[i].rect.w * 9 / 40;
        menu_images_hover[i].rect.h = menu_images_hover[i].rect.h * 9 / 40;
        menu_images_hover[i].rect.x = (SCREEN_WIDTH - menu_images_hover[i].rect.w) / 2;
        menu_images_hover[i].rect.y = SCREEN_HEIGHT / 2 + 27 + (menu_images_hover[i].rect.h) * i;
    }
    //read multiplayer buttons
    for (Sint8 i = 0; i < 2; ++i) {
        char address[10], hover_address[10];
        sprintf(address, "%d.bmp", i + 2);
        sprintf(hover_address, "%dh.bmp", i + 2);
        multiplayer[i].surface = SDL_LoadBMP(address);
        multiplayer_hover[i].surface = SDL_LoadBMP(hover_address);
        if (multiplayer[i].surface == NULL || multiplayer_hover[i].surface == NULL) {
            SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
        }
        multiplayer[i].texture = SDL_CreateTextureFromSurface(renderer, multiplayer[i].surface);
        multiplayer_hover[i].texture = SDL_CreateTextureFromSurface(renderer, multiplayer_hover[i].surface);
        if (multiplayer[i].texture == NULL || multiplayer_hover[i].texture == NULL) {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_QueryTexture(multiplayer[i].texture, NULL, NULL, &multiplayer[i].rect.w, &multiplayer[i].rect.h);
        SDL_QueryTexture(multiplayer_hover[i].texture, NULL, NULL, &multiplayer_hover[i].rect.w, &multiplayer_hover[i].rect.h);
        multiplayer[i].rect.w = multiplayer[i].rect.w / 5;
        multiplayer[i].rect.h = multiplayer[i].rect.h / 5;
        multiplayer[i].rect.x = SCREEN_WIDTH / 2 + (i - 1) * multiplayer[i].rect.w + 40 * (i + 2) - 100;
        multiplayer[i].rect.y = SCREEN_HEIGHT / 2 + 30;
        multiplayer_hover[i].rect.w = multiplayer_hover[i].rect.w * 9 / 40;
        multiplayer_hover[i].rect.h = multiplayer_hover[i].rect.h * 9 / 40;
        multiplayer_hover[i].rect.x = SCREEN_WIDTH / 2 + (i - 1) * multiplayer_hover[i].rect.w + 40 * (i + 2) - 100;
        multiplayer_hover[i].rect.y = SCREEN_HEIGHT / 2 + 30;
    }
    // key images
    for (Sint8 i = 0; i < 3; i++) {
        char filename[15] = {0};
        sprintf(filename, "%dkey.bmp", i + 1);
        key_images[i].surface = SDL_LoadBMP(filename);
        if (key_images[i].surface == NULL) {
            SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
        }
        key_images[i].texture = SDL_CreateTextureFromSurface(renderer, key_images[i].surface);
        if (key_images[i].texture == NULL) {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_QueryTexture(key_images[i].texture, NULL, NULL, &key_images[i].rect.w, &key_images[i].rect.h);
        key_images[i].rect.w = key_images[i].rect.w * 2 / 3;
        key_images[i].rect.h = key_images[i].rect.h * 2 / 3;
        key_images[i].rect.x = (SCREEN_WIDTH - key_images[i].rect.w) / 2 + (i - 1) * (key_images[i].rect.w + 20);
        key_images[i].rect.y = (SCREEN_HEIGHT - key_images[i].rect.h) / 2;
    }
}

void show_starting_menu() {
    pause_background();
    multiplayer_state = 2;
    while (players.state == P_MENU) {
        handle_sounds();
        if (menu_events() == -1) {
            not_closed = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 236, 213, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, logo.texture, NULL, &logo.rect);
        if (menu_state == M_OPENING) {
            if (menu_playtime) {
                SDL_RenderSetScale(renderer, 1.5, 1.75);
                stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 200) / 1.5), (Sint16) ((logo.rect.y - 20) / 1.75), "Press \"ESC\" to resume the game...", 0, 0, 0, (Uint8) (255 * pow(sin((double) SDL_GetTicks() / 500), 2)));
                SDL_RenderSetScale(renderer, 1, 1);
            }
            for (Sint8 i = 0; i < 3; i++) {
                i == menu_button_state ? SDL_RenderCopy(renderer, menu_images_hover[i].texture, NULL, &menu_images_hover[i].rect) : SDL_RenderCopy(renderer, menu_images[i].texture, NULL, &menu_images[i].rect);
            }
        } else if (menu_state == M_NEW_GAME) {
            for (Sint8 i = 0; i < 2; i++) {
                i + 2 == multiplayer_state ? SDL_RenderCopy(renderer, multiplayer_hover[i].texture, NULL, &multiplayer_hover[i].rect) : SDL_RenderCopy(renderer, multiplayer[i].texture, NULL, &multiplayer[i].rect);
            }
        } else { // menu state = 2, Load
            if (!file_checked) {
                check_storage();
            }
            SDL_RenderSetScale(renderer, 1.5, 1.75);
            if (!(last_number - 1)) {
                stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 210) / 1.5), (Sint16) ((logo.rect.y + logo.rect.h + 20) / 1.75), "There is no saved game! Press \"ESC\".", 0, 0, 0, 255);
            } else {
                char loading_str[50];
                sprintf(loading_str, "Set the number of saved game you want to play: %d.", load_number);
                char help[] = "Change the number using keys \"up\" or \"down\". Press \"ENTER\" to load.";
                stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 280) / 1.5), (Sint16) ((logo.rect.y + logo.rect.h + 20) / 1.75), loading_str, 0, 0, 0, 255);
                stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 380) / 1.5), (Sint16) ((logo.rect.y + logo.rect.h + 60) / 1.75), help, 0, 0, 0, 255);
            }
            SDL_RenderSetScale(renderer, 1, 1);
        }
        SDL_RenderPresent(renderer);
    }
    menu_playtime = 1;
    if (players.number == 2) {
        player_points[1].rect.x = player_points[2].rect.x;
    }
}

void waiting_for_start() {
    Sint16 angle[3] = {(Sint16) (rand() % 360), (Sint16) (rand() % 360), (Sint16) (rand() % 360)};
    if (players.number == 2) {
        key_images[0].rect.x = (SCREEN_WIDTH) / 2 - (key_images[0].rect.w + 20);
        key_images[1].rect.x = (SCREEN_WIDTH) / 2 + 20;
        player_points[1].rect.x = player_points[2].rect.x;
    } else {
        for (Sint8 i = 0; i < 3; i++) {
            key_images[i].rect.x = (SCREEN_WIDTH - key_images[i].rect.w) / 2 + (i - 1) * (key_images[i].rect.w + 20);
            player_points[i].rect.x = 200 + i * 250;
        }
    }
    while (players.state == P_WAITING) {
        handle_sounds();
        if (waiting_events() == -1) {
            not_closed = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 236, 213, 255);
        SDL_RenderClear(renderer);
        char finish[80];
        sprintf(finish, "Please set the finish score of the game by keys \"up\" and \"down\": %d", finish_point);
        SDL_RenderSetScale(renderer, 1.5, 1.75);
        stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 390) / 1.5), (Sint16) ((key_images[0].rect.y - 80) / 1.75), finish, 0, 0, 0, 255);
        stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 195) / 1.5), (Sint16) ((key_images[0].rect.y - 40) / 1.75), "Press ENTER to start the game...", 0, 0, 0, (Uint8) (255 * pow(sin((double) SDL_GetTicks() / 500), 2)));
        SDL_RenderSetScale(renderer, 1, 1);
        for (Sint8 i = 0; i < players.number; i++) {
            SDL_RenderCopy(renderer, key_images[i].texture, NULL, &key_images[i].rect);
            filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) (TANK_RADIUS * 2), (Uint8) players.tank[i].RGBA_color[0], (Uint8) players.tank[i].RGBA_color[1], (Uint8) players.tank[i].RGBA_color[2], 255);
            thickLineRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) ((key_images[i].rect.x + key_images[i].rect.w / 2) + LENGTH * 2 * cos(angle[i] * PI / 180)), (Sint16) ((key_images[i].rect.y + key_images[i].rect.h + 60) + LENGTH * 2 * sin(angle[i] * PI / 180)), 4, 70, 70, 70, 255);
            filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) (TANK_RADIUS), 232, 232, 232, 255);
            for (Sint8 j = -3; j < 3; j++) {
                filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2 + cos((angle[i] + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60 + sin((angle[i] + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 2), (Sint16) (TANK_RADIUS / 4), 232, 232, 232, 255);
            }
        }
        SDL_RenderPresent(renderer);
    }
}

void show_players_points() {
    for (Sint8 i = 0; i < players.number; i++) {
        SDL_RenderCopy(renderer, player_points[i].texture, NULL, &player_points[i].rect);
        char point[4];
        sprintf(point, "%d", players.tank[i].score);
        SDL_RenderSetScale(renderer, 1.5, 1.75);
        stringRGBA(renderer, (Sint16) ((player_points[i].rect.x + player_points[i].rect.w + 15) / 1.5), (Sint16) ((player_points[i].rect.y + player_points[i].rect.h / 2) / 1.75), point, 0, 0, 0, 255);
        SDL_RenderSetScale(renderer, 1, 1);
    }
}

void draw_map() {
    for (Sint8 n = 0; n <= max_boxes_x; n++) {
        for (Sint8 m = 0; m <= max_boxes_y; m++) {
            if (horizontal_walls[m][n]) {
                thickLineRGBA(renderer, (Sint16) (START_MAP_X + n * BOX_WIDTH - 1), (Sint16) (START_MAP_Y + m * BOX_WIDTH), (Sint16) (START_MAP_X + (n + 1) * BOX_WIDTH + 1), (Sint16) (START_MAP_Y + m * BOX_WIDTH), 5, 111, 63, 46, 255);
            }
            if (vertical_walls[m][n]) {
                thickLineRGBA(renderer, (Sint16) (START_MAP_X + n * BOX_WIDTH), (Sint16) (START_MAP_Y + m * BOX_WIDTH - 1), (Sint16) (START_MAP_X + n * BOX_WIDTH), (Sint16) (START_MAP_Y + (m + 1) * BOX_WIDTH + 1), 5, 111, 63, 46, 255);
            }
        }
    }
}

void draw_tank() {
    for (Sint8 i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            filledCircleRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (TANK_RADIUS), (Uint8) players.tank[i].RGBA_color[0], (Uint8) players.tank[i].RGBA_color[1], (Uint8) players.tank[i].RGBA_color[2], 255);
            thickLineRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180)), (Sint16) (players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180)), 4, 70, 70, 70, 255);
            if (players.tank[i].shot_type != T_MINE || players.tank[i].power.mine.mode != M_CARRYING) {
                filledCircleRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (TANK_RADIUS / 2), 232, 232, 232, 255);
                for (Sint8 j = -3; j < 3; j++) {
                    filledCircleRGBA(renderer, (Sint16) (players.tank[i].x + cos((players.tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (players.tank[i].y + sin((players.tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (TANK_RADIUS / 8), 232, 232, 232, 255);
                }
            } else if (players.tank[i].shot_type == T_MINE && players.tank[i].power.mine.mode == M_CARRYING) {
                for (Sint8 j = 0; j < 6; j++) {
                    thickLineRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (players.tank[i].power.mine.position.x + (TANK_RADIUS * 2 / 3) * cos(players.tank[i].angle * PI / 180 + 2 * j * PI / 6)), (Sint16) (players.tank[i].power.mine.position.y + (TANK_RADIUS * 2 / 3) * sin(players.tank[i].angle * PI / 180 + 2 * j * PI / 6)), 3, 232, 232, 232, 255);
                }
                filledCircleRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (TANK_RADIUS * 2 / 5), 232, 232, 232, 255);
            }
        }
    }
}

void draw_shot() {
    for (Sint8 i = 0; i < players.number; i++) {
        for (Sint8 j = 0; j < MAX_BALLS; j++) {
            if (players.tank[i].shot[j].time > 0) {
                if (!save_mode) {
                    shoot(&(players.tank[i].shot[j]));
                }
                filledCircleRGBA(renderer, (Sint16) players.tank[i].shot[j].x, (Sint16) players.tank[i].shot[j].y, (Sint16) (SHOT_RADIUS), 0, 0, 0, 255);
            }
        }
    }
}

void draw_power_box() {
    for (Sint8 i = 0; i < 2; i++) {
        if (laser_box[i].enable) {
            filledCircleRGBA(renderer, laser_box[i].center.x, laser_box[i].center.y, (Sint16) POWER_RADIUS, 230, 230, 230, 255);
            thickLineRGBA(renderer, (Sint16) (laser_box[i].center.x - POWER_RADIUS * cos(-PI / 6)), (Sint16) (laser_box[i].center.y - POWER_RADIUS * sin(-PI / 6)), (Sint16) (laser_box[i].center.x - POWER_RADIUS * cos(PI / 3) / 2), (Sint16) (laser_box[i].center.y - POWER_RADIUS * sin(PI / 3) / 2), 4, 255, 42, 0, 255);
            thickLineRGBA(renderer, (Sint16) (laser_box[i].center.x + POWER_RADIUS * cos(-PI / 6)), (Sint16) (laser_box[i].center.y + POWER_RADIUS * sin(-PI / 6)), (Sint16) (laser_box[i].center.x + POWER_RADIUS * cos(PI / 3) / 2), (Sint16) (laser_box[i].center.y + POWER_RADIUS * sin(PI / 3) / 2), 4, 255, 42, 0, 255);
            thickLineRGBA(renderer, (Sint16) (laser_box[i].center.x - POWER_RADIUS * cos(PI / 3) / 2), (Sint16) (laser_box[i].center.y - POWER_RADIUS * sin(PI / 3) / 2), (Sint16) (laser_box[i].center.x + POWER_RADIUS * cos(PI / 3) / 2), (Sint16) (laser_box[i].center.y + POWER_RADIUS * sin(PI / 3) / 2), 4, 255, 42, 0, 255);
            circleRGBA(renderer, laser_box[i].center.x, laser_box[i].center.y, (Sint16) POWER_RADIUS, 20, 20, 20, 255);
            if (!save_mode) {
                laser_box[i].time--;
            }
        }
        if (mine_box[i].enable) {
            filledCircleRGBA(renderer, mine_box[i].center.x, mine_box[i].center.y, (Sint16) POWER_RADIUS, 230, 230, 230, 255);
            for (Sint8 j = 0; j < 6; j++) {
                thickLineRGBA(renderer, mine_box[i].center.x, mine_box[i].center.y, (Sint16) (mine_box[i].center.x + (POWER_RADIUS * 2 / 3) * cos(2 * j * PI / 6)), (Sint16) (mine_box[i].center.y + (POWER_RADIUS * 2 / 3) * sin(2 * j * PI / 6)), 2, 255, 42, 0, 255);
            }
            filledCircleRGBA(renderer, mine_box[i].center.x, mine_box[i].center.y, (Sint16) (POWER_RADIUS * 2 / 5), 255, 42, 0, 255);
            circleRGBA(renderer, mine_box[i].center.x, mine_box[i].center.y, (Sint16) POWER_RADIUS, 20, 20, 20, 255);
            if (!save_mode) {
                mine_box[i].time--;
            }
        }
    }
}

void show_laser_killing(Sint8 i) {
    if (players.tank[i].power.laser.kill_time > 0) {
        thickLineRGBA(renderer, players.tank[i].power.laser.start.x, players.tank[i].power.laser.start.y, players.tank[i].power.laser.finish.x, players.tank[i].power.laser.finish.y, 3, 255, 42, 0, 255);
        if (!save_mode) {
            players.tank[i].power.laser.kill_time--;
        }
    }
}

void set_laser_coordinates(Sint8 i) {
    players.tank[i].power.laser.start.x = (Sint16) players.tank[i].x;
    players.tank[i].power.laser.start.y = (Sint16) players.tank[i].y;
    double a = -tan(players.tank[i].angle * PI / 180), b = 1.0, c = tan(players.tank[i].angle * PI / 180) * players.tank[i].x - players.tank[i].y;
    for (Sint8 j = 0; j < players.number; j++) {
        if (i != j && players.tank[j].life && absolute(a * players.tank[j].x + b * players.tank[j].y + c) / sqrt(pow(a, 2) + pow(b, 2)) < TANK_RADIUS) {
            if (((sin(players.tank[i].angle * PI / 180) > 0 && players.tank[j].y + TANK_RADIUS > players.tank[i].y) || (sin(players.tank[i].angle * PI / 180) <= 0 && players.tank[j].y - TANK_RADIUS < players.tank[i].y)) && ((cos(players.tank[i].angle * PI / 180) > 0 && players.tank[j].x + TANK_RADIUS > players.tank[i].x) || (cos(players.tank[i].angle * PI / 180) <= 0 && players.tank[j].x - TANK_RADIUS < players.tank[i].x)))
                players.tank[i].power.laser.targets[players.tank[i].power.laser.target_counter++] = j;
        }
    }
    if (players.tank[i].power.laser.target_counter == 0) { // wall
        players.tank[i].power.laser.target = -1;
        if (sin(players.tank[i].angle * PI / 180) >= 0 && cos(players.tank[i].angle * PI / 180) >= 0) {
            double end_x = -(b * FINISH_MAP_Y + c) / a, end_y = -(a * FINISH_MAP_X + c) / b;
            if (end_x < (double) FINISH_MAP_X) {
                players.tank[i].power.laser.finish.x = (Sint16) end_x;
                players.tank[i].power.laser.finish.y = (Sint16) (FINISH_MAP_Y);
            } else {
                players.tank[i].power.laser.finish.x = (Sint16) (FINISH_MAP_X);
                players.tank[i].power.laser.finish.y = (Sint16) end_y;
            }
        } else if (sin(players.tank[i].angle * PI / 180) >= 0 && cos(players.tank[i].angle * PI / 180) < 0) {
            double end_x = -(b * FINISH_MAP_Y + c) / a, end_y = -(a * START_MAP_X + c) / b;
            if (end_x > (double) START_MAP_X) {
                players.tank[i].power.laser.finish.x = (Sint16) end_x;
                players.tank[i].power.laser.finish.y = (Sint16) (FINISH_MAP_Y);
            } else {
                players.tank[i].power.laser.finish.x = (Sint16) (START_MAP_X);
                players.tank[i].power.laser.finish.y = (Sint16) end_y;
            }
        } else if (sin(players.tank[i].angle * PI / 180) < 0 && cos(players.tank[i].angle * PI / 180) >= 0) {
            double end_x = -(b * START_MAP_Y + c) / a, end_y = -(a * FINISH_MAP_X + c) / b;
            if (end_x < (double) FINISH_MAP_X) {
                players.tank[i].power.laser.finish.x = (Sint16) end_x;
                players.tank[i].power.laser.finish.y = (Sint16) (START_MAP_Y);
            } else {
                players.tank[i].power.laser.finish.x = (Sint16) (FINISH_MAP_X);
                players.tank[i].power.laser.finish.y = (Sint16) end_y;
            }
        } else {
            double end_x = -(b * START_MAP_Y + c) / a, end_y = -(a * START_MAP_X + c) / b;
            if (end_x > (double) START_MAP_X) {
                players.tank[i].power.laser.finish.x = (Sint16) end_x;
                players.tank[i].power.laser.finish.y = (Sint16) (START_MAP_Y);
            } else {
                players.tank[i].power.laser.finish.x = (Sint16) (START_MAP_X);
                players.tank[i].power.laser.finish.y = (Sint16) end_y;
            }
        }
    } else if (players.tank[i].power.laser.target_counter == 1) {
        double d = sqrt(pow(players.tank[i].x - players.tank[players.tank[i].power.laser.targets[0]].x, 2) + pow(players.tank[i].y - players.tank[players.tank[i].power.laser.targets[0]].y, 2));
        players.tank[i].power.laser.finish.x = (Sint16) (players.tank[i].power.laser.start.x + d * cos(players.tank[i].angle * PI / 180));
        players.tank[i].power.laser.finish.y = (Sint16) (players.tank[i].power.laser.start.y + d * sin(players.tank[i].angle * PI / 180));
        players.tank[i].power.laser.target = players.tank[i].power.laser.targets[0];
    } else if (players.tank[i].power.laser.target_counter == 2) {
        double d0 = sqrt(pow(players.tank[i].x - players.tank[players.tank[i].power.laser.targets[0]].x, 2) + pow(players.tank[i].y - players.tank[players.tank[i].power.laser.targets[0]].y, 2));
        double d1 = sqrt(pow(players.tank[i].x - players.tank[players.tank[i].power.laser.targets[1]].x, 2) + pow(players.tank[i].y - players.tank[players.tank[i].power.laser.targets[1]].y, 2));
        if (d0 < d1) {
            players.tank[i].power.laser.finish.x = (Sint16) (players.tank[i].power.laser.start.x + d0 * cos(players.tank[i].angle * PI / 180));
            players.tank[i].power.laser.finish.y = (Sint16) (players.tank[i].power.laser.start.y + d0 * sin(players.tank[i].angle * PI / 180));
            players.tank[i].power.laser.target = players.tank[i].power.laser.targets[0];
        } else {
            players.tank[i].power.laser.finish.x = (Sint16) (players.tank[i].power.laser.start.x + d1 * cos(players.tank[i].angle * PI / 180));
            players.tank[i].power.laser.finish.y = (Sint16) (players.tank[i].power.laser.start.y + d1 * sin(players.tank[i].angle * PI / 180));
            players.tank[i].power.laser.target = players.tank[i].power.laser.targets[1];
        }
    }
}

void destroy_mine_power(Sint8 i) {
    players.tank[i].power.mine.enable = false;
    players.tank[i].power.mine.kill_time = 0;
    players.tank[i].power.mine.carrying_time = 0;
    players.tank[i].power.mine.mode = M_CARRYING;
    players.tank[i].power.mine.hide_time = 0;
    players.tank[i].power.mine.show_time = 0;
    players.tank[i].power.mine.target = -1;
    players.tank[i].shot_type = T_NORMAL;
}

void draw_tank_power() {
    for (Sint8 i = 0; i < players.number; i++) {
        show_laser_killing(i);
        if (players.tank[i].shot_type == T_LASER) {
            players.tank[i].power.laser.target_counter = 0;
            players.tank[i].power.laser.targets[0] = -1;
            players.tank[i].power.laser.targets[1] = -1;
            set_laser_coordinates(i);
            thickLineRGBA(renderer, players.tank[i].power.laser.start.x, players.tank[i].power.laser.start.y, players.tank[i].power.laser.finish.x, players.tank[i].power.laser.finish.y, 3, 101, 255, 101, 255);
            if (!save_mode) {
                players.tank[i].power.laser.time--;
            }
        }
        if (players.tank[i].shot_type == T_MINE) { // mine
            if (players.tank[i].power.mine.mode == M_CARRYING) {
                players.tank[i].power.mine.position.x = (Sint16) players.tank[i].x;
                players.tank[i].power.mine.position.y = (Sint16) players.tank[i].y;
                if (!save_mode) {
                    players.tank[i].power.mine.carrying_time--;
                }
                if (!players.tank[i].power.mine.carrying_time) {
                    destroy_mine_power(i);
                }
            } else if (players.tank[i].power.mine.mode == M_MINING) {
                for (Sint8 j = 0; j < 6; j++) {
                    thickLineRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (players.tank[i].power.mine.position.x + (POWER_RADIUS * 2 / 3) * cos(2 * j * PI / 6)), (Sint16) (players.tank[i].power.mine.position.y + (POWER_RADIUS * 2 / 3) * sin(2 * j * PI / 6)), 2, 0, 0, 0, (Uint8) (255 * ((double) players.tank[i].power.mine.show_time / MINE_SHOW_TIME)));
                }
                filledCircleRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (POWER_RADIUS * 2 / 5), 0, 0, 0, (Uint8) (255 * ((double) players.tank[i].power.mine.show_time / MINE_SHOW_TIME)));
                if (!save_mode) {
                    players.tank[i].power.mine.show_time--;
                }
                if (!players.tank[i].power.mine.show_time) {
                    players.tank[i].power.mine.mode = M_HIDDEN;
                    players.tank[i].power.mine.hide_time = MINE_HIDE_TIME;
                }
            } else if (players.tank[i].power.mine.mode == M_HIDDEN) { // hide
                if (!save_mode) {
                    players.tank[i].power.mine.hide_time--;
                }
                if (!players.tank[i].power.mine.hide_time) {
                    destroy_mine_power(i);
                }
            } else { //kill
                for (Sint8 j = 0; j < 6; j++) {
                    thickLineRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (players.tank[i].power.mine.position.x + (POWER_RADIUS * 2 / 3) * cos(2 * j * PI / 6)), (Sint16) (players.tank[i].power.mine.position.y + (POWER_RADIUS * 2 / 3) * sin(2 * j * PI / 6)), 2, 0, 0, 0, 255);
                }
                filledCircleRGBA(renderer, players.tank[i].power.mine.position.x, players.tank[i].power.mine.position.y, (Sint16) (POWER_RADIUS * 2 / 5), 0, 0, 0, 255);
                if (!save_mode) {
                    players.tank[i].power.mine.kill_time--;
                }
                if (!players.tank[i].power.mine.kill_time) {
                    destroy_mine_power(i);
                }
            }
        }
    }
}

void show_time(Sint8 i) {
    Sint8 h = (Sint8) ((play_time / FPS) / 3600);
    Sint8 m = (Sint8) ((play_time / FPS) / 60 - 60 * h);
    Sint8 s = (Sint8) ((play_time / FPS) - 3600 * h - 60 * m);
    char time[10];
    sprintf(time, "%02d:%02d:%02d", h, m, s);
    if (!i) {
        SDL_RenderSetScale(renderer, 1.2, 1.5);
        stringRGBA(renderer, (Sint16) ((FINISH_MAP_X - 90) / 1.2), (Sint16) ((FINISH_MAP_Y + 15) / 1.5), time, 0, 0, 0, 255);
    } else {
        SDL_RenderSetScale(renderer, 1.5, 1.75);
        stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 45) / 1.5), (Sint16) ((player_points[0].rect.y - 180) / 1.75), time, 0, 0, 0, 255);
    }
    SDL_RenderSetScale(renderer, 1, 1);
}

void show_finish_score() {
    char finish[20];
    sprintf(finish, "Finish score: %d", finish_point);
    SDL_RenderSetScale(renderer, 1.2, 1.5);
    stringRGBA(renderer, (Sint16) ((START_MAP_X + 12) / 1.2), (Sint16) ((FINISH_MAP_Y + 15) / 1.5), finish, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, 1, 1);
}

void show_save_tool() {
    SDL_RenderSetScale(renderer, 1.2, 1.5);
    if (!save_mode) {
        stringRGBA(renderer, (Sint16) ((START_MAP_X + 12) / 1.2), (Sint16) ((START_MAP_Y - 20) / 1.5), "Press \"Left CTRL + S\" to save this game!", 0, 0, 0, 255);
    } else if (save_mode) {
        if (!file_checked) {
            check_storage();
        }
        char number[100];
        sprintf(number, "Save number will be %d.", last_number);
        stringRGBA(renderer, (Sint16) ((START_MAP_X + 12) / 1.2), (Sint16) ((START_MAP_Y - 50) / 1.5), number, 0, 0, 0, 255);
        stringRGBA(renderer, (Sint16) ((START_MAP_X + 12) / 1.2), (Sint16) ((START_MAP_Y - 20) / 1.5), "Press \"ESC\" to back to the game... Press \"ENTER\" to confirm saving.", 0, 0, 0, 255);
    }
    SDL_RenderSetScale(renderer, 1, 1);
}

void drawing() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_power_box();
    draw_shot();
    draw_tank_power();
    draw_tank();
    show_players_points();
    show_time(0);
    show_finish_score();
    show_save_tool();
    SDL_RenderPresent(renderer);
}

void show_winner() {
    char colors[][6] = {"red", "green", "blue"};
    char winner_str[50];
    sprintf(winner_str, "Game Over!! the winner is \"%s\".", colors[winner]);
    SDL_RenderSetScale(renderer, 1.5, 1.75);
    stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH / 2 - 190) / 1.5), (Sint16) ((player_points[0].rect.y - 210) / 1.75), winner_str, (Uint8) players.tank[winner].RGBA_color[0], (Uint8) players.tank[winner].RGBA_color[1], (Uint8) players.tank[winner].RGBA_color[2], 255);
    SDL_RenderSetScale(renderer, 1, 1);
}

void show_game_over_points() {
    for (Sint8 i = 0; i < players.number; i++) {
        SDL_RenderCopy(renderer, player_points[i].texture, NULL, &player_points[i].rect);
        char point[4];
        sprintf(point, "%d", players.tank[i].score);
        SDL_RenderSetScale(renderer, 1.5, 1.75);
        stringRGBA(renderer, (Sint16) ((player_points[i].rect.x + player_points[i].rect.w + 15) / 1.5), (Sint16) ((player_points[i].rect.y + player_points[i].rect.h / 2) / 1.75), point, 0, 0, 0, 255);
        SDL_RenderSetScale(renderer, 1, 1);
    }
}

void set_winner() {
    winner = 0;
    Sint32 best_point = players.tank[0].score;
    for (Sint8 i = 1; i < players.number; i++) {
        if (players.tank[i].score > best_point) {
            best_point = players.tank[i].score;
            winner = i;
        }
    }
}

void game_over() {
    for (Sint8 i = 0; i < 3; i++) {
        player_points[i].rect.x += 30;
        player_points[i].rect.y = (SCREEN_HEIGHT - player_points[i].rect.h) / 2;
    }
    set_winner();
    while (players.state == P_GAME_OVER) {
        if (game_over_events() == -1) {
            not_closed = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        show_winner();
        show_time(1);
        show_game_over_points();
        SDL_RenderPresent(renderer);
    }
    for (Sint8 i = 0; i < 3; i++) {
        player_points[i].rect.x = 200 + i * 250;
        player_points[i].rect.y = SCREEN_HEIGHT - player_points[i].rect.h * 4 / 3;
    }
}

void Quit() {
    SDL_DestroyTexture(logo.texture);
    SDL_FreeSurface(logo.surface);
    for (Sint8 i = 0; i < 2; i++) {
        SDL_DestroyTexture(multiplayer[i].texture);
        SDL_FreeSurface(multiplayer[i].surface);
        SDL_DestroyTexture(multiplayer_hover[i].texture);
        SDL_FreeSurface(multiplayer_hover[i].surface);
    }
    for (Sint8 i = 0; i < 3; i++) {
        SDL_DestroyTexture(key_images[i].texture);
        SDL_FreeSurface(key_images[i].surface);
    }

    for (Sint8 i = 0; i < 3; i++) {
        SDL_DestroyTexture(player_points[i].texture);
        SDL_FreeSurface(player_points[i].surface);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}