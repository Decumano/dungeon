//
// Created by rcarlini on 11/12/19.
//

#ifndef DUNGEON_STATE_H
#define DUNGEON_STATE_H

#include "dungeon.h"

#define OPTION_ATTACK '1'
#define OPTION_HEAL '2'
#define OPTION_FLEE '3'

typedef struct _step {
    char direction;
    Position pos;
} Step;

typedef struct {
    Step* steps;
    int pathSize;
} Path;

typedef struct {
    int hp;
    Position current;
    int damage;
    int acc;
    short inFrontDoor1turn;
    char lastDir;
}Enemy;

typedef struct {
    Dungeon* dun;
    int is_finished;
    Path path;
    Position current;
    int hp;
    int healthPotions;
    int damage;
    int agility;
    Enemy* enemies;
    int numEn;
} State;

void init_state(State* state, Dungeon* dungeon);
void init_enemies(State* state);

int is_finished(State* state);

Room* get_current_room(State* state);
void add_as_last_step(State* state, char direction);
void reset(State* state);
int move(State* state, char direction, int move);
int go_back(State* state, char direction);
void update_hp(State* state);

int add_enemies(State* state);
void check_enemies(State* state);
int move_monster(State* state);

char rever_direction(char direction);

#endif //DUNGEON_STATE_H
