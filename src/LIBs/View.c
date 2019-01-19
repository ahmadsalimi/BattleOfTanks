#include <stdio.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "View.h"
#include "Physics.h"
#include "Struct.h"
#include "Constants.h"
#include "MapGenerate.h"
#include "Play.h"


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
Sint8 menu_state;
Sint8 menu_button_state;
bool menu_playtime;
Sint8 multiplayer_state;
Sint16 finish_point = 1;

void load_icon() {
    icon = SDL_LoadBMP("icon.bmp");
    if (icon == NULL) {
        SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
    }
}

void show_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Battle of Tanks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    load_icon();
    SDL_SetWindowIcon(window, icon);
}

void read_images() {
    // read scoreboard images
    for (int i = 0; i < 3; i++) {
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
    for (int i = 0; i < 3; ++i) {
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
    //2 player
    multiplayer[0].surface = SDL_LoadBMP("2.bmp");
    multiplayer_hover[0].surface = SDL_LoadBMP("2h.bmp");
    if (multiplayer[0].surface == NULL || multiplayer_hover[0].surface == NULL) {
        SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
    }
    multiplayer[0].texture = SDL_CreateTextureFromSurface(renderer, multiplayer[0].surface);
    multiplayer_hover[0].texture = SDL_CreateTextureFromSurface(renderer, multiplayer_hover[0].surface);
    if (multiplayer[0].texture == NULL || multiplayer_hover[0].texture == NULL) {
        SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
    }
    SDL_QueryTexture(multiplayer[0].texture, NULL, NULL, &multiplayer[0].rect.w, &multiplayer[0].rect.h);
    SDL_QueryTexture(multiplayer_hover[0].texture, NULL, NULL, &multiplayer_hover[0].rect.w, &multiplayer_hover[0].rect.h);
    multiplayer[0].rect.w = multiplayer[0].rect.w / 5;
    multiplayer[0].rect.h = multiplayer[0].rect.h / 5;
    multiplayer[0].rect.x = SCREEN_WIDTH / 2 - multiplayer[0].rect.w - 20;
    multiplayer[0].rect.y = SCREEN_HEIGHT / 2 + 30;
    multiplayer_hover[0].rect.w = multiplayer_hover[0].rect.w * 9 / 40;
    multiplayer_hover[0].rect.h = multiplayer_hover[0].rect.h * 9 / 40;
    multiplayer_hover[0].rect.x = SCREEN_WIDTH / 2 - multiplayer_hover[0].rect.w - 20;
    multiplayer_hover[0].rect.y = SCREEN_HEIGHT / 2 + 30;
    //3 player
    multiplayer[1].surface = SDL_LoadBMP("3.bmp");
    multiplayer_hover[1].surface = SDL_LoadBMP("3h.bmp");
    if (multiplayer[1].surface == NULL || multiplayer_hover[1].surface == NULL) {
        SDL_ShowSimpleMessageBox(0, "Image init error", SDL_GetError(), window);
    }
    multiplayer[1].texture = SDL_CreateTextureFromSurface(renderer, multiplayer[1].surface);
    multiplayer_hover[1].texture = SDL_CreateTextureFromSurface(renderer, multiplayer_hover[1].surface);
    if (multiplayer[1].texture == NULL || multiplayer_hover[1].texture == NULL) {
        SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
    }
    SDL_QueryTexture(multiplayer[1].texture, NULL, NULL, &multiplayer[1].rect.w, &multiplayer[1].rect.h);
    SDL_QueryTexture(multiplayer_hover[1].texture, NULL, NULL, &multiplayer_hover[1].rect.w, &multiplayer_hover[1].rect.h);
    multiplayer[1].rect.w = multiplayer[1].rect.w / 5;
    multiplayer[1].rect.h = multiplayer[1].rect.h / 5;
    multiplayer[1].rect.x = SCREEN_WIDTH / 2 + 20;
    multiplayer[1].rect.y = SCREEN_HEIGHT / 2 + 30;
    multiplayer_hover[1].rect.w = multiplayer_hover[1].rect.w * 9 / 40;
    multiplayer_hover[1].rect.h = multiplayer_hover[1].rect.h * 9 / 40;
    multiplayer_hover[1].rect.x = SCREEN_WIDTH / 2 + 20;
    multiplayer_hover[1].rect.y = SCREEN_HEIGHT / 2 + 30;
    // key images
    for (int i = 0; i < 3; i++) {
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
    multiplayer_state = 2;
    while (players.state == 0) {
        if (menu_events() == -1) {
            flag = 0;
            break;
        }
        if (menu_state == 0) {
            SDL_SetRenderDrawColor(renderer, 255, 236, 213, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, logo.texture, NULL, &logo.rect);
            if (menu_playtime) {
                stringRGBA(renderer, SCREEN_WIDTH / 2 - 130, (Sint16) (logo.rect.y - 20), "Press \"ESC\" to resume the game...", 0, 0, 0, (Uint8) (255 * pow(sin((double) SDL_GetTicks() / 500), 2)));
            }
            if (menu_button_state == 0) { // New Game
                SDL_RenderCopy(renderer, menu_images_hover[0].texture, NULL, &menu_images_hover[0].rect);
                SDL_RenderCopy(renderer, menu_images[1].texture, NULL, &menu_images[1].rect);
                SDL_RenderCopy(renderer, menu_images[2].texture, NULL, &menu_images[2].rect);
            } else if (menu_button_state == 1) { // Load
                SDL_RenderCopy(renderer, menu_images[0].texture, NULL, &menu_images[0].rect);
                SDL_RenderCopy(renderer, menu_images_hover[1].texture, NULL, &menu_images_hover[1].rect);
                SDL_RenderCopy(renderer, menu_images[2].texture, NULL, &menu_images[2].rect);
            } else if (menu_button_state == 2) { // Exit
                SDL_RenderCopy(renderer, menu_images[0].texture, NULL, &menu_images[0].rect);
                SDL_RenderCopy(renderer, menu_images[1].texture, NULL, &menu_images[1].rect);
                SDL_RenderCopy(renderer, menu_images_hover[2].texture, NULL, &menu_images_hover[2].rect);
            }
            SDL_RenderPresent(renderer);
        } else if (menu_state == 1) { // New game
            SDL_SetRenderDrawColor(renderer, 255, 236, 213, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, logo.texture, NULL, &logo.rect);
            if (multiplayer_state == 2) {
                SDL_RenderCopy(renderer, multiplayer[1].texture, NULL, &multiplayer[1].rect);
                SDL_RenderCopy(renderer, multiplayer_hover[0].texture, NULL, &multiplayer_hover[0].rect);
            } else if (multiplayer_state == 3) {
                SDL_RenderCopy(renderer, multiplayer[0].texture, NULL, &multiplayer[0].rect);
                SDL_RenderCopy(renderer, multiplayer_hover[1].texture, NULL, &multiplayer_hover[1].rect);
            }
            SDL_RenderPresent(renderer);
        } else { // Load

        }
    }
    menu_playtime = 1;
    if (players.number == 2) {
        player_points[1].rect.x = player_points[2].rect.x;
    }
}

void waiting_for_start() {
    int angle[3] = {rand() % 360, rand() % 360, rand() % 360};
    if (players.number == 2) {
        key_images[0].rect.x = (SCREEN_WIDTH) / 2 - (key_images[0].rect.w + 20);
        key_images[1].rect.x = (SCREEN_WIDTH) / 2 + 20;
        player_points[1].rect.x = player_points[2].rect.x;
    } else {
        for (int i = 0; i < 3; i++) {
            key_images[i].rect.x = (SCREEN_WIDTH - key_images[i].rect.w) / 2 + (i - 1) * (key_images[i].rect.w + 20);
            player_points[i].rect.x = 200 + i * 250;
        }
    }
    while (players.state == 1) {
        if (waiting_events() == -1) {
            flag = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 236, 213, 255);
        SDL_RenderClear(renderer);
        char finish[80];
        sprintf(finish, "Please set the finish score of the game by keys \"up\" and \"down\": %d", finish_point);
        stringRGBA(renderer, SCREEN_WIDTH / 2 - 260, (Sint16) (key_images[0].rect.y - 40), finish, 0, 0, 0, 255);
        stringRGBA(renderer, SCREEN_WIDTH / 2 - 130, (Sint16) (key_images[0].rect.y - 20), "Press ENTER to start the game...", 0, 0, 0, (Uint8) (255 * pow(sin((double) SDL_GetTicks() / 500), 2)));
        for (int i = 0; i < players.number; i++) {
            SDL_RenderCopy(renderer, key_images[i].texture, NULL, &key_images[i].rect);
            filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) (TANK_RADIUS * 2), (Uint8) players.tank[i].RGBA_color[0], (Uint8) players.tank[i].RGBA_color[1], (Uint8) players.tank[i].RGBA_color[2], 255);
            thickLineRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) ((key_images[i].rect.x + key_images[i].rect.w / 2) + LENGTH * 2 * cos(angle[i] * PI / 180)), (Sint16) ((key_images[i].rect.y + key_images[i].rect.h + 60) + LENGTH * 2 * sin(angle[i] * PI / 180)), 4, 70, 70, 70, 255);
            filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60), (Sint16) (TANK_RADIUS), 232, 232, 232, 255);
            for (int j = -3; j < 3; j++) {
                filledCircleRGBA(renderer, (Sint16) (key_images[i].rect.x + key_images[i].rect.w / 2 + cos((angle[i] + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 2), (Sint16) (key_images[i].rect.y + key_images[i].rect.h + 60 + sin((angle[i] + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 2), (Sint16) (TANK_RADIUS / 4), 232, 232, 232, 255);
            }
        }
        SDL_RenderPresent(renderer);
    }
}

void show_players_points() {
    for (int i = 0; i < players.number; i++) {
        SDL_RenderCopy(renderer, player_points[i].texture, NULL, &player_points[i].rect);
        char point[4];
        sprintf(point, "%d", players.tank[i].score);
        stringRGBA(renderer, (Sint16) (player_points[i].rect.x + player_points[i].rect.w + 10), (Sint16) (player_points[i].rect.y + player_points[i].rect.h / 2), point, 0, 0, 0, 255);
    }
}

void draw_map() {
    for (int n = 0; n <= max_boxes_x; n++) {
        for (int m = 0; m <= max_boxes_y; m++) {
            if (horizontal_walls[m][n]) {
                thickLineRGBA(renderer, (Sint16) (START_MAP_X + n * BOX_WIDTH - 1), (Sint16) (START_MAP_Y + m * BOX_WIDTH), (Sint16) (START_MAP_X + (n + 1) * BOX_WIDTH + 1), (Sint16) (START_MAP_Y + m * BOX_WIDTH), 3, 111, 63, 46, 255);
            }
            if (vertical_walls[m][n]) {
                thickLineRGBA(renderer, (Sint16) (START_MAP_X + n * BOX_WIDTH), (Sint16) (START_MAP_Y + m * BOX_WIDTH - 1), (Sint16) (START_MAP_X + n * BOX_WIDTH), (Sint16) (START_MAP_Y + (m + 1) * BOX_WIDTH - 1), 3, 111, 63, 46, 255);
            }
        }
    }
}

void draw_tank() {
    for (int i = 0; i < players.number; i++) {
        if (players.tank[i].life) {
            filledCircleRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (TANK_RADIUS), (Uint8) players.tank[i].RGBA_color[0], (Uint8) players.tank[i].RGBA_color[1], (Uint8) players.tank[i].RGBA_color[2], 255);
            thickLineRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (players.tank[i].x + LENGTH * cos((players.tank[i].angle) * PI / 180)), (Sint16) (players.tank[i].y + LENGTH * sin((players.tank[i].angle) * PI / 180)), 4, 70, 70, 70, 255);
            filledCircleRGBA(renderer, (Sint16) players.tank[i].x, (Sint16) players.tank[i].y, (Sint16) (TANK_RADIUS / 2), 232, 232, 232, 255);
            for (int j = -3; j < 3; j++) {
                filledCircleRGBA(renderer, (Sint16) (players.tank[i].x + cos((players.tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (players.tank[i].y + sin((players.tank[i].angle + 30 + j * 60) * PI / 180) * TANK_RADIUS * 3 / 4), (Sint16) (TANK_RADIUS / 8), 232, 232, 232, 255);
            }
        }
    }
}

void draw_shot() {
    for (int i = 0; i < players.number; i++) {
        for (int j = 0; j < MAX_BALLS; j++) {
            if (players.tank[i].shot[j].time > 0) {
                shoot(&(players.tank[i].shot[j]));
                filledCircleRGBA(renderer, (Sint16) players.tank[i].shot[j].x, (Sint16) players.tank[i].shot[j].y, (Sint16) (SHOT_RADIUS), 0, 0, 0, 255);
            }
        }
    }
}

void drawing() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_map();
    draw_shot();
    draw_tank();
    show_players_points();
    SDL_RenderPresent(renderer);
}

void show_winner() {
    char colors[][6] = {"red", "green", "blue"};
    int winner = 0, best_point = players.tank[0].score;
    for (int i = 1; i < players.number; i++) {
        if (players.tank[i].score > best_point) {
            best_point = players.tank[i].score;
            winner = i;
        }
    }
    char winner_str[50];
    sprintf(winner_str, "Game Over!! the winner is \"%s\".", colors[winner]);
    stringRGBA(renderer, SCREEN_WIDTH / 2 - 140, (Sint16) (player_points[0].rect.y - 200), winner_str, 0, 0, 0, 255);
}

void show_game_over_points() {
    for (int i = 0; i < players.number; i++) {
        SDL_RenderCopy(renderer, player_points[i].texture, NULL, &player_points[i].rect);
        char point[4];
        sprintf(point, "%d", players.tank[i].score);
        stringRGBA(renderer, (Sint16) (player_points[i].rect.x + player_points[i].rect.w + 10), (Sint16) (player_points[i].rect.y + player_points[i].rect.h / 2), point, 0, 0, 0, 255);
    }
}

void game_over() {
    for (int i = 0; i < 3; i++) {
        player_points[i].rect.y = (SCREEN_HEIGHT - player_points[i].rect.h) / 2;
    }

    while (players.state == 3) {
        if (game_over_events() == -1) {
            flag = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        show_winner();
        show_game_over_points();
        SDL_RenderPresent(renderer);
    }
    for (int i = 0; i < 3; i++) {
        player_points[i].rect.y = SCREEN_HEIGHT - player_points[i].rect.h * 4 / 3;
    }
}

void Quit() {
    SDL_DestroyTexture(logo.texture);
    SDL_FreeSurface(logo.surface);
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(multiplayer[i].texture);
        SDL_FreeSurface(multiplayer[i].surface);
        SDL_DestroyTexture(multiplayer_hover[i].texture);
        SDL_FreeSurface(multiplayer_hover[i].surface);
    }
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(key_images[i].texture);
        SDL_FreeSurface(key_images[i].surface);
    }

    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(player_points[i].texture);
        SDL_FreeSurface(player_points[i].surface);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}