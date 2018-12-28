#include "MapGenerate.h"
#include "Struct.h"

MAP cells[17][19];
Sint8 width, height;
Sint8 max_boxes_x;
Sint8 max_boxes_y;
bool vertical_walls[10][10];
bool horizontal_walls[10][10];
bool cells_connection[10][10] = {1};

void initialize() {
    MAP *n;
    for (Sint8 i = 0; i < width; i++) {
        for (Sint8 j = 0; j < height; j++) {
            n = &cells[i][j];
            n->parent = NULL;
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
    for (Sint8 i = 0; i < 10; i++) {
        for (Sint8 j = 0; j < 10; j++) {
            vertical_walls[i][j] = 0;
            horizontal_walls[i][j] = 0;
            cells_connection[i][j] = 0;
        }
    }
    cells_connection[0][0] = 1;
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
    }

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
}

void generate_map() {
    MAP *start, *last;
    height = (Sint8) ((5 + rand() % 3) * 2 + 1);
    width = (Sint8) ((7 + rand() % 3) * 2 + 1);
    max_boxes_y = (Sint8) (height / 2 + 1);
    max_boxes_x = (Sint8) (width / 2 + 1);
    initialize();
    start = &cells[1][1];
    start->parent = start;
    last = start;
    while ((last = connect_cells(last)) != start);
    render_walls();
}