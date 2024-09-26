//
// Created by rcarlini on 22/10/19.
//

#include <string.h>

#include "../headers/wall.h"

/**
 * Initializes the wall with an empty label and no door (so is_open and is_exit are FALSE).
 *
 * @param wall The wall to be initialized.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Sets all properties of The wall as FALSE.
 */
void init_wall(Wall *wall) {
    wall->is_open = FALSE;
    wall->is_exit = FALSE;
    wall->has_door = FALSE;
    strcpy(wall->label, "");
}

/**
 * Sets the label of the wall.
 *
 * Hint: The label must be copied!
 *
 * @param door The wall to be modified.
 * @param label The label for the wall.
 *
 * Pre: The wall and label needs to be different from NULL.
 * Post: Copies the label string.
 */
void set_wall_label(Wall *wall, const char label[MAX_STRING]) {
    strcpy(wall->label, label);
}

/**
 * Returns the label of the wall.
 * @param room The wall to be queried.
 * @return The wall label.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: The string corresponding to the label is returned.
 */
char* get_wall_label(Wall *wall) {
    return wall->label;
}

/**
 * Checks if the wall has a door.
 * @param wall
 * @return TRUE if the wall has a door, FALSE otherwise.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Returns TRUE or FALSE for the has_door condition.
 */
#include "stdio.h"
int has_door(Wall *wall) {
    return wall->has_door;
}

/**
 * Adds a door to the wall.
 * @param wall
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Sets wall with an opened door properties.
 */
void add_door(Wall *wall) {
    wall->has_door = TRUE;
    wall->is_open = TRUE;
}
/**
 * Removes the door from the wall.
 * @param wall
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Sets wall without an opened door properties.
 */
void remove_door(Wall *wall) {
    wall->has_door = FALSE;
    wall->is_open = FALSE;
}

/**
 * Sets the state of the wall door to state value.
 *
 * @param wall The wall to be modified.
 * @param state TRUE/FALSE if the door is open/closed respectively.
 * @return If the wall has no door, NO_DOOR_ERROR code is returned. SUCCESS code is returned otherwise.
 *
 * Pre: The wall needs to be different from NULL and state and integer.
 * Post: Returns the the value to state if it was a door, if not, returns an error.
 */
int set_door_open(Wall* wall, int state) {
    if (has_door(wall)){
        wall->is_open = state;
        wall->is_open = state;
        return state;
    }else{
        return NO_DOOR_ERROR;
    }
}

/**
 * IF the wall has a door, set it's state as open.
 *
 * @param wall The wall to be modified.
 * @return If the wall has no door, NO_DOOR_ERROR code is returned. SUCCESS code is returned otherwise.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Returns SUCCESS if door is present, sets it opened and returns an error if does not exist.
 */
int open_door(Wall* wall) {
    if (has_door(wall)){
        wall->is_open = TRUE;
        return SUCCESS;
    }else{
        return NO_DOOR_ERROR;
    }
}

/**
 * IF the wall has a door, set it's state as closed.
 *
 * @param wall The wall to be modified.
 * @return If the wall has no door, NO_DOOR_ERROR code is returned. SUCCESS code is returned otherwise.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Returns SUCCESS if door is present, sets it closed and returns an error if does not exist.
 */
int close_door(Wall* wall) {
    if (has_door(wall)){
        wall->is_open = FALSE;
        return SUCCESS;
    }else{
        return NO_DOOR_ERROR;
    }
}

/**
 * Checks if the wall has a door and is opened.
 * @param wall The wall to be checked.
 * @return TRUE if the wall has an open door, FALSE otherwise.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Returns TRUE is wall is set to is_open, and FALSE otherwise.
 */
int has_open_door(Wall *wall) {
    return (has_door(wall) && wall->is_open);
}

/**
 * Checks if the wall has an exit door.
 * @param wall The wall to be checked.
 * @return TRUE if the wall has an exit door, FALSE otherwise.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Returns TRUE is wall is set to is_exit, and FALSE otherwise.
 */
int has_exit_door(Wall *wall) {
    return (has_door(wall) && wall->is_exit);
}

/**
 * Adds an exit door to the wall.
 * @param wall The wall to be modified.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Sets a wall with is_exit being TRUE.
 */
void add_exit_door(Wall *wall) {
    add_door(wall);
    wall->is_exit = TRUE;
}

/**
 * Removes the exit door from the wall.
 * @param wall The wall to be modified.
 *
 * Pre: The wall needs to be different from NULL.
 * Post: Changes to FALSE both has_door and is_exit properties.
 */
void remove_exit_door(Wall *wall) {
    wall->has_door = FALSE;
    wall->is_exit = FALSE;
}
