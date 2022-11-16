//
// Created by rcarlini on 14/10/19.
//
#include <stdio.h>
#include <stdlib.h>

#include "../headers/dungeon.h"

/**
 * Sets the value for row and column into the position.
 *
 * @param position The position to be modified.
 * @param row The row value.
 * @param column The column value.
 *
 * Pre: The position needs to be different from NULL.
 * Post: The updated position with the row and column variables.
 */
void set_position(Position* position, int row, int column) {
    position->row = row;
    position->column = column;
}

/**
 * Sets the starting position of the dungeon.
 * @param dungeon The dungeon to be set.
 *
 * Pre: The dungeon needs to be different from NULL.
 * Post: set the initial position of the dungeon.
 */
void set_starting_position(Dungeon *dungeon, int row, int column) {
    dungeon->initialPos.column = column;
    dungeon->initialPos.row = row;
}

/**
 * Returns the starting position of the dungeon.
 * @param dungeon The dungeon to be queried.
 * @return The starting position of the dungeon.
 *
 * Pre: The dungeon needs to be different from NULL.
 * Post: return the initialPos variable from the dungeon struct.
 */
Position get_starting_position(Dungeon *dungeon) {
    return dungeon->initialPos;
}

/**
 * Returns the starting room of the dungeon.
 * @param dungeon The dungeon to be queried.
 * @return The starting room of the dungeon.
 *
 * Pre: The dungeon needs to be different from NULL.
 * Post: return the room that is in the initialPos position.
 */
Room* get_starting_room(Dungeon *dungeon) {
    return &dungeon->map[dungeon->initialPos.row][dungeon->initialPos.column];
}

/**
 * Checks if the row and column values fits in the dungeon room matrix.
 *
 * @param row The row value.
 * @param column The column value.
 *
 * @return TRUE if the values are between 0 and ROWS for row and between 0 and COLUMN for column. If the row value is out of range, it returns INVALID_ROW code. If the column value is out of range, it returns INVALID_COLUMN code.
 *
 * Pre: The variables must be integers.
 * Post: returns if the coordinates are valid or not.
 */
int is_valid_coordinates(Dungeon* dungeon, int row, int column) {
    if (row >= 0 && row < dungeon->rows){
        if (column >= 0 && column < dungeon->columns){
            return TRUE;
        }else{
            return FALSE;
        }
    }else{
        return FALSE;
    }
}

/**
 * Checks if the position fits in the dungeon room matrix.
 *
 * @param position The position to be checked.
 * @return TRUE if the position is between 0 and ROWS and between 0 and COLUMN. If the row value is out of range, it returns INVALID_ROW code. If the column value is out of range, it returns INVALID_COLUMN code.
 *
 * Pre: The position must be different from NULL.
 * Post: returns if the coordinates of the position are valid.
 */
int is_valid_position(Dungeon* dungeon, Position position) {
    return is_valid_coordinates(dungeon, position.row, position.column);
}

/**
 * Returns the dungeon room at the input row and column position.
 *
 * @param dungeon The dungeon from which the room should be retrieved.
 * @param row The row position of the room.
 * @param column The column position of the room.
 * @return The (reference to the) room of the dungeon corresponding to the row and column values if those are valid. NULL otherwise.
 *
 * Pre: The dungeon must be different from NULL.
 * Post: returns the room at the coordinates, if is not valid returns NULL.
 */
Room *get_room_at(Dungeon *dungeon, int row, int column) {
    if (is_valid_coordinates(dungeon, row, column)){
        return &dungeon->map[row][column];
    }else{
        return NULL;
    }
}

/**
 * Returns the dungeon room at the input position.
 *
 * @param dungeon The dungeon from which the room should be retrieved.
 * @param position The position of the room.
 * @return The (reference to the) room of the dungeon corresponding to the position if valid. NULL otherwise.
 *
 * Pre: The dungeon and position must be not NULL.
 * Post: returns the room at the coordinates of the position, it returns NULL if is not valid.
 */
Room* get_room_at_position(Dungeon *dungeon, Position position) {
    return get_room_at(dungeon, position.row, position.column);
}

/**
 * Initializes the dungeon, setting the starting position to (0,0) and initializes each room in the matrix.
 * @param dungeon The dungeon to be initialized
 * @return SUCCESS code if the initialization was successful, ERROR code if something went wrong.
 *
 * Pre: The dungeon needs to be different from NULL.
 * Post: Initialize all the variables in the dungeon struct.
 */
int init_dungeon(Dungeon *dungeon) {
    dungeon->initialPos.row = 0;
    dungeon->initialPos.column = 0;
    dungeon->columns = COLUMNS;
    dungeon->rows = ROwS;
    dungeon->map = (Room **) malloc(dungeon->columns * sizeof(Room*));
    for (int i = 0; i < dungeon->columns; i++){
        dungeon->map[i] = (Room*)malloc(dungeon->rows*sizeof(Room));
        for (int j = 0; j < dungeon->rows; j++){
            init_room(&dungeon->map[i][j]);
        }
    }
    return SUCCESS;
}

/**
 * Resets the visited flag in all the rooms, clearing it.
 * @param dungeon The dungeon to be reset.
 * @return SUCCESS code if the reset was successful, ERROR code if something went wrong.
 */
int reset_visited(Dungeon* dungeon) {
    for (int i = 0; i < dungeon->rows; i++){
        for (int j = 0; j < dungeon->columns; j++){
            clear_visited(&dungeon->map[i][j]);
        }
    }
    return TRUE;
}

/**
 * Adds traps to the dungeon.
 * @param dungeon The dungeon to add traps.
 *
 * Pre: The dungeon needs to be different to NULL.
 * Post: Add traps to all the dungeon with a random number generator.
 */
void add_traps(Dungeon* dungeon){
    int n = 1;
    for (int i = 0; i < dungeon->rows; i++){
        for (int j = 0; j < dungeon->columns; j++){
            if (is_enabled(&dungeon->map[i][j])){
                int ran = random_number(&dungeon->map[i][j], 0, 1, n);
                if (ran == 1){
                    add_trap(&dungeon->map[i][j]);
                }
                n++;
            }
        }
    }
}