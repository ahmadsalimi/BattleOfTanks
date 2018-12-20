//
// Created by nik on 12/20/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MapGenerate.h"
#include "Struct.h"

MAP cells[17][19];
int width, height;
int max_boxes_x;
int max_boxes_y;
int vertical_walls[10][10];
int horizontal_walls[10][10];
int cells_connection[8][9] = {1};

void initialize() {
    MAP *n;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            n = &cells[i][j];
            if (i * j % 2) {
                n->x = i;
                n->y = j;
                n->dirs = 15; //in binary: 1111. means all of the directions must be explored.
                n->wall = 0;
            } else {
                n->wall = 1; //Add walls everywhere.
            }
        }
    }
}

MAP *connect_cells(MAP *n) {
    int x = 0, y = 0;
    int dir;
    MAP *next_cell;
    while (n->dirs) {
        dir = (1 << (rand() % 4));
        if ((~n->dirs) & dir) {
            continue; //repetitive path
        }
        n->dirs &= ~dir;

        switch (dir) {
            case 1: //right
                if (n->x + 2 < width) {
                    x = n->x + 2;
                    y = n->y;
                } else continue;
                break;
            case 2: //down
                if (n->y + 2 < height) {
                    x = n->x;
                    y = n->y + 2;
                } else continue;
                break;
            case 4: //left
                if (n->x - 2 >= 0) {
                    x = n->x - 2;
                    y = n->y;
                } else continue;
                break;
            case 8: //up
                if (n->y - 2 >= 0) {
                    x = n->x;
                    y = n->y - 2;
                } else continue;
                break;
            default:
                break;
        }
        next_cell = &cells[x][y];
        if (next_cell->wall == 0) {
            if (next_cell->parent != NULL) continue; //this cell is already connected!
            next_cell->parent = n;
            cells[(x + n->x) / 2][(y + n->y) / 2].wall = 0; // remove wall.
            cells_connection[y / 2][x / 2] = 1;
            return next_cell;
        }
    }
    return n->parent; //any paths left.
}

void render_walls() {
    //separate the prepared walls into vertical and horizontal walls.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!(i % 2) && (j % 2) && (cells[j][i].wall) && (cells[j + 1][i].wall)) {
                horizontal_walls[i / 2][j / 2] = 1;
            }
            if ((i % 2) && !(j % 2) && (cells[j][i].wall) && (cells[j][i + 1].wall)) {
                vertical_walls[i / 2][j / 2] = 1;
            }
        }
    }
    //check if map has single cells to remove some walls.
    for (int i = 0; i < max_boxes_y - 1; i++) {
        for (int j = 0; j < max_boxes_x - 1; j++) {
            printf("%d ", cells_connection[i][j]);
            if (!cells_connection[i][j]) {
                int a = rand() % 2, b = rand() % 2;
                if (horizontal_walls[i + a][j] && i + a < max_boxes_y - 1 && j < max_boxes_x) {
                    horizontal_walls[i + a][j] = 0;
                }
                if (vertical_walls[i][j + b] && i < max_boxes_y - 1 && j + b < max_boxes_x) {
                    vertical_walls[i][j + b] = 0;
                }
            }
        }
        printf("\n");

    }
    printf("\n");

    int counter = max_boxes_x * max_boxes_y / 9;
    while (counter) {
        int h_or_v = rand() % 2;
        if (h_or_v) {
            int r_y = 1 + rand() % (max_boxes_y - 2), r_x = rand() % (max_boxes_x - 1);
            if (horizontal_walls[r_y][r_x]) {
                horizontal_walls[r_y][r_x] = 0;
                counter--;
                continue;
            }
        } else {
            int r_y = rand() % (max_boxes_y - 1), r_x = 1 + rand() % (max_boxes_x - 2);
            if (vertical_walls[r_y][r_x]) {
                vertical_walls[r_y][r_x] = 0;
                counter--;
                continue;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", horizontal_walls[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", vertical_walls[i][j]);
        }
        printf("\n");
    }
}

int generate_map() {
    MAP *start, *last;
    height = (5 + rand() % 3) * 2 + 1;
    width = (7 + rand() % 3) * 2 + 1;
    max_boxes_y = height / 2 + 1;
    max_boxes_x = width / 2 + 1;
    initialize();
    start = &cells[1][1];
    start->parent = start;
    last = start;
    while ((last = connect_cells(last)) != start);
    render_walls();
}