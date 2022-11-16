//
// Created by rcarlini on 22/10/19.
//

#ifndef DUNGEON_DUNGEON_UTILS_H
#define DUNGEON_DUNGEON_UTILS_H

#include "dungeon.h"

#define MAX_LOADING_BUFFER 50
#define MAX_ROOM_DATA 10

#define FILE_NOT_FOUND -1
#define INVALID_ROOM_DATA -2
#define INVALID_ROOM_LINE -3

#define UPPER_LEFT_CORNER "┏"
#define UPPER_RIGHT_CORNER "┓"
#define BOTTOM_LEFT_CORNER "┗"
#define BOTTOM_RIGHT_CORNER "┛"
#define HORIZONTAL_WALL "━━━"
#define HORIZONTAL_OPEN_DOOR "╸ ╺"
#define HORIZONTAL_CLOSED_DOOR "╸┄╺"
#define VERTICAL_WALL "┃"
#define VERTICAL_OPEN_DOOR " "
#define VERTICAL_CLOSED_DOOR "┊"

#define INITIAL_PLACEHOLDER "I"
#define CURRENT_PLACEHOLDER "X"
#define ENEMY_PLACEHOLDER "M"
#define EMPTY_SPACE " "
#define EMPTY_CELL "     "

void draw_dungeon(FILE *fd, Dungeon *dungeon, Room* current_room, Room** monster_room, int sizeEn, int hide_disabled, int draw, const int* mhp);

void init_simple_dungeon(Dungeon* dungeon);
void init_two_rooms_dungeon(Dungeon* dungeon);

int load_dungeon(Dungeon* dungeon, char* path);


#endif //DUNGEON_DUNGEON_UTILS_H
