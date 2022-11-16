//
// Created by rcarlini on 22/10/19.
//

#ifndef DUNGEON_WALL_H
#define DUNGEON_WALL_H

#include "common.h"

typedef struct {
    int has_door;
    int is_open;
    int is_exit;
    char label[MAX_STRING];
} Wall;

void init_wall(Wall* wall);

void set_wall_label(Wall* wall, const char label[MAX_STRING]);
char* get_wall_label(Wall* wall);

int has_door(Wall* wall);
void add_door(Wall* wall);
void remove_door(Wall* wall);

int has_open_door(Wall* wall);
int open_door(Wall* wall);
int close_door(Wall* wall);

int has_exit_door(Wall* wall);
void add_exit_door(Wall* wall);
void remove_exit_door(Wall* wall);

#endif //DUNGEON_WALL_H
