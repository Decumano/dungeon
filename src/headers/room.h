//
// Created by rcarlini on 15/10/19.
//

#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "common.h"
#include "wall.h"

#define NORTH 'N'
#define EAST 'E'
#define SOUTH 'S'
#define WEST 'W'
#define QUIT 'Q'
#define PATH 'P'
#define CHECK_TRAP 'C'
#define CHECK_DOOR 'D'
#define HEAL 'H'
#define WALLS 4

typedef struct {
    int trap;
    int activated;
    int damage;
} Trap;

typedef struct {
    Wall walls[WALLS];
    int enabled;
    int visited;
    Trap trap;
    int seed;
} Room;

void init_room(Room* room);

void enable_room(Room* room);
void disable_room(Room* room);
int is_enabled(Room* room);

void mark_visited(Room* room);
void clear_visited(Room* room);
int is_visited(Room* room);

Wall* get_wall(Room* room, char direction);

void add_trap(Room* room);
int has_active_trap(Room* room);
void deactivate_trap(Room* room);
void trap_damage(Room* room);
int random_number(Room* room, int min, int max, int idx);

#endif //DUNGEON_ROOM_H
