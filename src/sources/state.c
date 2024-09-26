//
// Created by rcarlini on 11/12/19.
//
#include "stdio.h"

#include <stddef.h>
#include "../headers/state.h"
#include "stdlib.h"
#include "../headers/utils.h"

/**
 * Initializes the state, storing the dungeon in it and setting the current position to the dungeon starting position.
 * Also sets finished flag to FALSE.
 *
 * @param state The state to be initialized.
 * @param dungeon The dungeon to be set as the state dungeon.
 *
 * Pre: State and dungeon must be different from NULL.
 * Post: All the state variables needs to have initialized.
 */

void init_state(State* state, Dungeon* dungeon) {
    state->dun = dungeon;
    state->is_finished = FALSE;
    state->path.pathSize = 0;
    state->path.steps = (Step*)malloc((state->path.pathSize)*sizeof(Step));
    state->current = dungeon->initialPos;
    mark_visited(&state->dun->map[state->current.row][state->current.column]);
    state->hp = 20;
    state->healthPotions = 5;
    state->agility = 4;
    state->damage = 5;
    state->numEn = 1;
    state->enemies = (Enemy*)malloc(state->numEn*sizeof(Enemy));
    init_enemies(state);
}

void init_enemies(State* state){
    int n = 6;
    for (int i = 0; i < state->numEn; i++){
        state->enemies[i].hp = 0;
        state->enemies[i].damage = random_number(get_current_room(state), 3, 5, n);
        state->enemies[i].acc = random_number(get_current_room(state), 2, 6, n);
        state->enemies[i].current = state->dun->initialPos;
        state->enemies[i].lastDir = ' ';
        n  = (n * 512 - 78) % 12 + 1;
    }
}

void add_enemy(State* state, Position position, int* idx, int n){
    *idx = *idx + 1;
    if (is_valid_position(state->dun, position)){
        state->enemies[*idx-1].hp = random_number(get_current_room(state), 5, 25, n);
        state->enemies[*idx-1].current = position;
    }
}

int add_enemies(State* state){
    int max = (state->dun->rows*state->dun->columns)/9;
    while (TRUE) {
        printf("How many enemies do you want? (1-%d)\n", max);
        int option = read_int_option("");
        if (option > 0 && option <= max) {
            state->numEn = option;
            break;
        }
        printf("Please enter a number between 1 and %d\n", max);
    }
    state->enemies = (Enemy*)realloc(state->enemies, state->numEn*sizeof(Enemy));
    init_enemies(state);
    int n = 1, idx = 0;
    Position position;
    while (idx < state->numEn) {
        for (int i = 0; i < state->dun->rows; i++) {
            for (int j = 0; j < state->dun->columns; j++) {
                if (idx >= state->numEn){
                    return TRUE;
                }
                if (is_enabled(&state->dun->map[i][j])) {
                    int ran = random_number(&state->dun->map[i][j], 0, 1, n);
                    if (ran == 1 && (i != 0 || j != 0) && idx < state->numEn) {
                        position.column = j;
                        position.row = i;
                        add_enemy(state, position, &idx, n+76);
                    }
                    n++;
                }
            }
        }
    }
    return TRUE;
}

void fight(State* state, Enemy* enemy)
{
    int m = 56;
    printf("You are in front of a monster. What do you want to do?\n");

    char option = '\0';
    while (option != OPTION_FLEE) {

        printf("\n");
        printf( "%c. Attack\n",  OPTION_ATTACK);
        printf( "%c. Heal\n",  OPTION_HEAL);
        printf( "%c. Flee\n", OPTION_FLEE);
        option = read_char_option("Choose an option: \n");

        Room *room = get_room_at_position(state->dun, enemy->current);
        int i = random_number(room, 0, state->agility + enemy->acc, m);

        if (option == OPTION_FLEE){
            if (i > state->agility){
                 break;
            }
        } else if (option == OPTION_ATTACK) {
            if (i > state->agility)
            {
                state->hp -= enemy->damage;
                printf("You received %d damage, your health is %d.\n", enemy->damage, state->hp);
            }
            else 
            {
                printf("The monster missed!");
            }
            enemy->hp = enemy->hp - state->damage;
        }
        else if (option == OPTION_HEAL) {
            if (state->healthPotions)
            {
                if (i > state->agility)
                {
                    state->hp += 6 - enemy->damage;
                    printf("You received %d damage, your health is %d.\n", enemy->damage, state->hp);
                }
                else 
                {
                    state->hp += 6;
                    printf("The monster missed!");
                }
                state->healthPotions--;
            }
        }
        else {
            printf("Invalid option!\n");
        }
        if (state->hp <= 0) {
            printf("You encountered a monster and killed you\n");
            break;
        } else if (state->enemies[i].hp <= 0) {
            printf("You have killed a monster!\n");
            break;
        }
    }
}

void check_enemies(State* state){
    for (int i = 0; i < state->numEn; i++) {
        Enemy* enemy = &state->enemies[i];
        if (enemy->hp > 0) {
            if (state->current.row == enemy->current.row &&
                state->current.column == enemy->current.column) {
                fight(state, enemy);
            } else {
                Room *room = get_room_at_position(state->dun, state->enemies[i].current);
                if (has_active_trap(room)) {
                    if (room->trap.damage >= 6){
                        enemy->hp = 0;
                    }else{
                        enemy->hp = enemy->hp - room->trap.damage;
                    }
                    deactivate_trap(room);
                }
            }
        }
    }
}

/**
 * Sets the current position from the dungeon associated to the state.
 *
 * @param state The state that stores the current position.
 * @param row The current row coordinate.
 * @param column The current column coordinate.
 *
 * Pre: The state and position must be different from NULL.
 * Post: The posLog of the state needs to be refreshed.
 */
void set_current_position(State* state, Position position) {
    state->current = position;
}

/**
 * Retrieves the current position from the dungeon associated to the state.
 *
 * @param state The state that stores the current room.
 * @return The current room of the dungeon.
 *
 * Pre: The state must be different to NULL.
 * Post: return the latest position record.
 */
Position get_current_position(State* state) {
    return state->current;
}

/**
 * Retrieves the current room from the dungeon associated to the state.
 *
 * @param state The state that stores the current room.
 * @return The current room of the dungeon.
 *
 * Pre: The state must be different to NULL.
 * Post: return the room with the latest positon record.
 */
Room* get_current_room(State* state) {
    return &state->dun->map[state->current.row][state->current.column];
}

/**
 * Sets the finished flag to finished value (TRUE or FALSE)
 *
 * @param state The state to be updated.
 * @param is_finished The value marking the game as finished or not.
 *
 * Pre: The state must be different to NULL.
 * Post: Set if the state is finished or not.
 */
void set_finished(State* state, int finished) {
    state->is_finished = finished;
}

/**
 * Returns the value of is_finished flag (TRUE or FALSE), marking a game as finished.
 *
 * @param state The state to be updated.
 *
 * Pre: The state must be different to NULL.
 * Post: return the is_finished variable from state.
 */
int is_finished(State* state) {
    return state->is_finished;
}

void add_as_first_step(State* state, char direction) {
    state->path.steps[0].direction = direction;
    state->path.steps[0].pos = state->dun->initialPos;
}

char rever_direction(char direction){
    switch(direction){
        case NORTH:
            direction = SOUTH;
            break;
        case EAST:
            direction = WEST;
            break;
        case SOUTH:
            direction = NORTH;
            break;
        case WEST:
            direction = EAST;
            break;
        default:
            return INVALID_DIRECTION;
    }
    return direction;
}

void get_move(char direction, Position* position, int move){
    switch(direction){
        case NORTH:
            set_position(position, position->row - move, position->column);
            break;
        case EAST:
            set_position(position, position->row, position->column + move);
            break;
        case SOUTH:
            set_position(position, position->row + move, position->column);
            break;
        case WEST:
            set_position(position, position->row, position->column - move);
            break;
        default:
            break;
    }
}

/**
 * Adds the step as the last step of the path sequence.
 *
 * @param state The current state.
 * @param direction The direction of the step.
 */
void add_as_last_step(State* state, char direction) {
    state->path.pathSize++;
    Step* aux = (Step*)realloc(state->path.steps, (state->path.pathSize)*sizeof(Step));
    if (aux == NULL){
        state->path.pathSize--;
        free(aux);
    }else {
        state->path.steps = aux;
        int move = -1;
        Position position = get_current_position(state);get_move(direction, &position, move);
        state->path.steps[state->path.pathSize - 1].pos = position;
        state->path.steps[state->path.pathSize - 1].direction = direction;
    }
}

/**
 * Deletes (freeing if needed) all the steps stored in the path of the state.
 * @param state The state with the path to be cleare.
 */
void clear_path(State* state) {
    state->path.pathSize = 1;
    free(state->path.steps);
}

/**
 * Resets the state, clearing the state path and reverting the visited marks on the dungeon rooms.
 *
 * @param state The state to be reset.
 */
void reset(State* state) {
    reset_visited(state->dun);
    clear_path(state);
}

/**
 * @param state The current state.
 *
 * Pre: The state needs to be different from NULL.
 * Post: Updates the log variables on the state.
 */

void open_door_e(Wall* wall, Room* room, char direction){
    direction = rever_direction(direction);
    open_door(wall);
    open_door(get_wall(room, direction));
}

/**
 * If the move is valid, moves the player in the input direction, updating the current room position.
 * If there is no room in that direction, returns INVALID_DIRECTION code.
 * If the wall in the direction does not have a door, returns NO_DOOR_ERROR.
 * If the target position is no valid, returns INVALID_MOVE code.
 *
 * @param state The current state.
 * @param direction The direction to move to.
 * @return TRUE if the move was successful, INVALID_DIRECTION code if there is no room in that direction, NO_DOOR_ERROR code if there is no door in that direction or INVALID_MOVE if the target position is invalid.
 *
 * Pre: The state needs to be different from NULL.
 * Post: Updates the current position, if is possible to do, it returns an error if the direction or move are not valid and if the door is missing.
 */

int move(State* state, char direction, int is_move) {
    int move = 1;
    Room* room = get_current_room(state);
    Wall* wall = get_wall(room, direction);
    Position position = get_current_position(state);
    get_move(direction, &position, move);
    if (has_exit_door(wall)){
        set_finished(state, TRUE);
        return TRUE;
    }else if (position.row != get_current_position(state).row || position.column != get_current_position(state).column) {
        if (has_door(wall)) {
            if (is_valid_position(state->dun, position)) {
                if (is_enabled(get_room_at_position(state->dun, position))) {
                    if (has_open_door(wall) || !is_move) {
                        set_current_position(state, position);
                        return TRUE;
                    }
                    return NO_DOOR_ERROR;
                } else {
                    return INVALID_DIRECTION;
                }
            } else {
                return INVALID_MOVE;
            }
        } else {
            return NO_DOOR_ERROR;
        }
    }
    return INVALID_DIRECTION;
}

/**
 * If the move is valid, moves the player in the opposite direction, updating the current room position.
 * If there is no room in that direction, returns INVALID_DIRECTION code.
 * If the wall in that direction does not have a door, returns NO_DOOR_ERROR.
 * If the target position is no valid, returns INVALID_MOVE code.
 *
 * @param state The current state.
 * @param direction The direction to move to.
 * @return TRUE if the move was successful, INVALID_DIRECTION code if there is no room in that direction, NO_DOOR_ERROR code if there is no door in that direction or INVALID_MOVE if the target position is invalid.
 *
 * Pre: The state and the direction need to be different from NULL.
 * Post: return if the move was possible or not and all upon the required function to move.
 */
int go_back(State* state, char direction) {
    direction = rever_direction(direction);
    if (direction != INVALID_DIRECTION){
        return move(state, direction, FALSE);
    }
    return direction;
}

void update_hp(State* state){
    Room* room = get_current_room(state);
    if (room->trap.damage >= 6){
        state->hp = 0;
    }else{
        state->hp = state->hp - room->trap.damage;
    }
}

int move_monster(State* state) {
    int m = 56;
    for (int j = 0; j < state->numEn; j++) {
        if (state->enemies[j].hp > 0) {
            char directions[4] = {NORTH, WEST, SOUTH, EAST};
            Position position = state->enemies[j].current, prev_pos = state->enemies[j].current;
            Room *room = get_room_at_position(state->dun, position);

            int move = 1, i = random_number(room, 0, 3, m);

            char direction = (state->enemies[j].lastDir != ' ') ? state->enemies[j].lastDir : directions[i];
            Wall *wall = get_wall(room, direction);
            get_move(direction, &position, move);
            if (has_door(wall)) {
                if (is_valid_position(state->dun, position)) {
                    if (is_enabled(get_room_at_position(state->dun, position))) {
                        if (!has_open_door(wall)) {
                            if (state->enemies[j].inFrontDoor1turn){
                                open_door_e(wall, get_room_at_position(state->dun, position), direction);
                                state->enemies[j].inFrontDoor1turn = FALSE;
                                state->enemies[j].lastDir = ' ';
                            }
                            else
                            {
                                state->enemies[j].inFrontDoor1turn++;
                                state->enemies[j].lastDir = direction;
                            }
                            return TRUE;
                        }
                        if ((position.row == prev_pos.row) && (position.column == prev_pos.column)) {
                            return move_monster(state);
                        }
                        state->enemies[j].current = position;
                        return TRUE;
                    } else {
                        j--;
                        m = (m * 43 - 3) % 56 + 1;
                    }
                } else {
                    j--;
                    m = (m * 43 - 3) % 56 + 1;
                }
            } else {
                j--;
                m = (m * 43 - 3) % 56 + 1;
            }
        }
    }
    return FALSE;
}