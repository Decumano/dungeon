//
// Created by rcarlini on 14/10/19.
//

#ifndef DUNGEON_H
#define DUNGEON_H

#include "room.h"

#define ROwS 4
#define COLUMNS 4

typedef struct {
    int row;
    int column;
} Position;

typedef struct {
    Position initialPos;
    Room** map;
    int rows;
    int columns;
} Dungeon;

void set_position(Position* position, int row, int column);

void set_starting_position(Dungeon *dungeon, int row, int column);
Position get_starting_position(Dungeon *dungeon);
Room* get_starting_room(Dungeon *dungeon);

int init_dungeon(Dungeon *dungeon);

int is_valid_coordinates(Dungeon* dungeon, int row, int column);
int is_valid_position(Dungeon* dungeon, Position position);
int reset_visited(Dungeon* dungeon);

Room *get_room_at(Dungeon *dungeon, int row, int column);
Room *get_room_at_position(Dungeon *dungeon, Position position);

void free_dungeon(Dungeon* dungeon);
void add_traps(Dungeon* dungeon);

#endif //DUNGEON_H
