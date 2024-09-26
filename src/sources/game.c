//
// Created by rcarlini on 4/1/20.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../headers/utils.h"
#include "../headers/state.h"
#include "../headers/dungeon_utils.h"
#include "../headers/game.h"

int print_wall_info(Wall* wall, char *side) {

    int doors = 0;
    if (has_door(wall) == TRUE) {
        printf("There is a door at the %s wall. ", side);
        doors++;

        if (has_open_door(wall) == TRUE) {
            printf("Seems to be OPEN.   ");

        } else {
            printf("Seems to be CLOSED. ");
        }

//        if (strlen(wall->label) > 0) {
//            printf("It has a label: \"%s\". ", wall->label);
//        }
        printf("\n");
    }

    return doors;
}

char choose_direction(Room* room) {

    int doors = 0;

    printf("\n");
    doors += print_wall_info(get_wall(room, NORTH), "North");
    doors += print_wall_info(get_wall(room, EAST), "East");
    doors += print_wall_info(get_wall(room, SOUTH), "South");
    doors += print_wall_info(get_wall(room, WEST), "West");

    char option;
    if (doors == 0) {
        option = read_char_option("There are no doors! Press Q for quit and try again...\n");

    } else {
        option = toupper(read_char_option("Which direction do you want to go? (N/E/S/W)\nPress H to use a potion\nPress C to check traps\nPress D to open and close doors\nPress P for showing path, Q for quit\n"));
    }

    return option;
}

int is_valid_direction_option(char direction) {
    if (direction == NORTH || direction == EAST ||
        direction == SOUTH || direction == WEST) {
        return TRUE;

    } else {
        return INVALID_OPTION;
    }
}

void do_move(State* state, char direction) {

    int move_result = move(state, direction, TRUE);
    switch (move_result) {
        case SUCCESS:
            mark_visited(&state->dun->map[state->current.row][state->current.column]);
            add_as_last_step(state, direction);
            if (has_active_trap(get_current_room(state))){
                update_hp(state);
                deactivate_trap(get_current_room(state));
                if (get_current_room(state)->trap.damage == 6){
                    printf("You found an instakill trap\n");
                }else{
                    printf("You found a trap and get %d damage\n", get_current_room(state)->trap.damage);
                }
            }
            break;

        case NO_DOOR_ERROR:
            printf("This side has no door!\n");
            break;

        case INVALID_MOVE:
            printf("This move is invalid!\n");
            break;

        case INVALID_DIRECTION:
            printf("This direction is invalid!\n");
            break;

        default:
            printf("Unexpected move result code!\n");
            break;
    }
}

int check_doors(State* state){
    char option = toupper(read_char_option("In which direction do you wish to check?(N/E/S/W)\n"));
    if (is_valid_direction_option(option) == TRUE) {
        Room* current_room = get_current_room(state);
        int move_result = move(state, option, FALSE);
        switch (move_result) {
            case SUCCESS:
                if (has_open_door(get_wall(get_current_room(state), rever_direction(option)))) {
                    printf("There is an open door in %c direction\n", option);
                    while (TRUE) {
                        char option2 = toupper(read_char_option("Do you want to close it? (Y/N)\n"));
                        if (option2 == 'Y') {
                            close_door(get_wall(get_current_room(state), rever_direction(option)));
                            close_door(get_wall(current_room, option));
                            break;
                        } else if (option2 == 'N') {
                            break;
                        } else {
                            printf("There is no %c option, try again\n", option2);
                        }
                    }
                }else{
                        printf("There is a closed door in %c direction\n", option);
                        while (TRUE) {
                            char option2 = toupper(read_char_option("Do you want to open it? (Y/N)\n"));
                            if (option2 == 'Y') {
                                open_door(get_wall(get_current_room(state), rever_direction(option)));
                                open_door(get_wall(current_room, option));
                                break;
                            } else if (option2 == 'N') {
                                break;
                            } else {
                                printf("There is no %c option, try again\n", option2);
                            }
                        }
                }
                go_back(state, option);
                return TRUE;

            case NO_DOOR_ERROR:
                printf("This side has no door!\n");
                break;

            case INVALID_MOVE:
                printf("This move is invalid!\n");
                break;

            case INVALID_DIRECTION:
                printf("This direction is invalid!\n");
                break;

            default:
                printf("Unexpected move result code!\n");
                break;
        }
    }else{
        return check_doors(state);
    }
    return FALSE;
}

int check_traps(State* state){
    char option = toupper(read_char_option("In which direction do you wish to check?(N/E/S/W)\n"));
    if (is_valid_direction_option(option) == TRUE) {
        int move_result = move(state, option, FALSE);
        switch (move_result) {
            case SUCCESS:
                if (has_active_trap(get_current_room(state))) {
                    printf("There is a trap in %c direction\n", option);
                    while (TRUE) {
                        char option2 = toupper(read_char_option("Do you want to deactivate it? (Y/N)\n"));
                        if (option2 == 'Y') {
                            deactivate_trap(get_current_room(state));
                            break;
                        } else if (option2 == 'N') {
                            break;
                        } else {
                            printf("There is no %c option, try again\n", option2);
                        }
                    }
                } else {
                    printf("There is no trap in direction %c\n", option);
                }
                go_back(state, option);
                return TRUE;

            case NO_DOOR_ERROR:
                printf("This side has no door!\n");
                break;

            case INVALID_MOVE:
                printf("This move is invalid!\n");
                break;

            case INVALID_DIRECTION:
                printf("This direction is invalid!\n");
                break;

            default:
                printf("Unexpected move result code!\n");
                break;
        }
    }else{
        return check_traps(state);
    }
    return FALSE;
}

/**
 * Print the path stored in the state. If there are no steps, shows the message 'No steps yet'. If there are, should
 * print the starting position and the sequence of directions followed, e.g. 'Starting from (0, 0): N E'.
 *
 * @param state The state storing the path to be printed.
 */
void print_path(State* state) {
    printf("Starting from (%d,%d): ", state->dun->initialPos.row, state->dun->initialPos.column);
    for (int i = 0; i < state->path.pathSize; i++){
        printf("%c ", state->path.steps[i].direction);
    }
    printf("\n");
}

void choose_difficulty(State* state, Dungeon* dungeon){
    int easy = 1, normal = 2, hard = 3;
    while (TRUE){
        int option = read_int_option("Please select your difficulty\n1- Easy (Only dungeon)\n2- Normal (With traps)\n3- Hard (With traps and enemies)\n");
        if (option == easy){
            break;
        }
        else if(option == normal){
            add_traps(dungeon);
            break;
        }
        else if (option == hard){
            add_traps(dungeon);
            add_enemies(state);
            break;
        }
    }
}

void start_game(Dungeon* dungeon) {

    State state;
    init_state(&state, dungeon);

    int quit = FALSE;
    choose_difficulty(&state, dungeon);
    printf("Starting game...\n\n");
    Room* current_room = get_current_room(&state);
    Room** monster_room = (Room**)malloc(state.numEn*sizeof(Room*));
    int* hp = (int*)malloc(state.numEn*sizeof(int));

    while (quit == FALSE) {
        for (int i = 0; i < state.numEn; i++){
            monster_room[i] = get_room_at_position(state.dun, state.enemies[i].current);
            hp[i] = state.enemies[i].hp;
        }

        if (current_room == NULL) {
            quit = TRUE;
            printf("No current room found! Quitting...\n");

        } else {
            draw_dungeon(stdout, dungeon, current_room, monster_room, state.numEn, TRUE, TRUE, hp);
            printf("\nYour current health is: %d\nYou have %d potions", state.hp, state.healthPotions);

            char direction = toupper(choose_direction(current_room));

            if (is_valid_direction_option(direction) == TRUE) {
                do_move(&state, direction);
                if (is_finished(&state)) {
                    quit = TRUE;
                }

            } else if (direction == PATH) {
                print_path(&state);

            } else if (direction == QUIT) {
                quit = TRUE;

            } else if (direction == CHECK_TRAP) {
                check_traps(&state);

            } else if (direction == CHECK_DOOR){
                check_doors(&state);

            } else if (direction == HEAL){
                state.healthPotions--;
                state.hp += 9;
            } else {
                printf("Invalid option! Try again...\n");
            }
            move_monster(&state);
            check_enemies(&state);
            if (state.hp <= 0){
                quit = TRUE;
            }
        }
        free(monster_room);
        free(hp);
    }

    if (is_finished(&state) == TRUE) {
        printf("You found the exit!\n\n");
        print_path(&state);
    }else if (state.hp <= 0){
        printf("GAME OVER\nYou are dead\n\n");
        print_path(&state);
    }
    reset(&state);
    free(state.enemies);
}

void reverse(State* state){
    Step* to_rev = state->path.steps;
    int n = state->path.pathSize, m = n/2;
    for(unsigned int i = 0; i < m; i++) {
        Step aux = to_rev[i];
        to_rev[i] = to_rev[n-1-i];
        to_rev[n-1-i] = aux;
    }
}

// Header needed for the recursion
int check_current_room(State* state);

/**
 * Checks if there is a door in the input direction. If there is and it is an exit door, returns TRUE. If it is not an
 * exit door, it should move and check the new current room by calling check_current_room. If that call returns TRUE, it
 * means that the step was correct, so it has to be stored in the path. Take into account that it should also go back
 * to the original room reverting the move in all cases.
 *
 * @param state The current state of the solving process.
 * @param room The current room.
 * @param direction The direction to be checked.
 * @return TRUE if this step leads to an exit, FALSE otherwise.
 */
int check_direction(State* state, Room* room, char direction) {
    Wall* wall = get_wall(room, direction);
    if (has_exit_door(wall)) {
        add_as_last_step(state, direction);
        return TRUE;
    }else{
        if (has_door(wall)){
            move(state, direction, FALSE);
            if (!is_visited(get_current_room(state))){
                mark_visited(get_current_room(state));
                int check = check_current_room(state);
                if (check) {
                    add_as_last_step(state, direction);
                    return TRUE;
                }
            }else{
                return FALSE;
            }
        }else{
            return FALSE;
        }
    }
    return FALSE;
}

/**
 *  Checks if the current room is already visited. If it was not visited, checks each direction in order to find an exit.
 *  If one of the directions returns TRUE, it means that this room is part of the path to the exit and returns TRUE as well.
 *
 * @param state The current state of the solving process.
 * @return TRUE if the room belongs to the path to an exit.
 */
int check_current_room(State* state) {
    char directions[WALLS] = {'N', 'E', 'S', 'W'};
    Position pos = state->current;
    Room* current = get_current_room(state);
    for (int i = 0; i < WALLS; i++) {
        int check = check_direction(state, current, directions[i]);
        if (check) {
            return TRUE;
        }else{
            state->current = pos;
        }
    }
    return FALSE;
}

/**
 * Given a dungeon, searches for a path to an exit, printing the path if found or printing the message 'No solution found' otherwise.
 * @param dungeon
 */
void solve(Dungeon* dungeon) {
    State state;
    init_state(&state, dungeon);

    int solved = check_current_room(&state);
    if (solved == TRUE) {
        reverse(&state);
        printf("\nSolution:\n");

        print_path(&state);
        printf("\n");
    } else {
        printf("\nNo solution found!\n");
    }

    reset(&state);
}
