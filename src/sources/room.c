//
// Created by rcarlini on 15/10/19.
//

#include <string.h>

#include "../headers/room.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initializes the room with and empty label, disabled and not visited by default.
 * Also initializes all 4 rooms (N/E/S/W)).
 *
 * @param room The room to be initialized.
 *
 * Pre: The room needs to be different from NULL.
 * Post: The rooms conditions, enabled and visited, both FALSE.
 */
void init_room(Room *room) {
    room->enabled = FALSE;
    room->visited = FALSE;
    for (int i = 0; i < WALLS; i++){
        init_wall(&room->walls[i]);
    }
    room->trap.activated = FALSE;
    room->trap.trap = FALSE;
    room->trap.damage = 0;
}

/**
 * Enables the room, marking it as visible and usable.
 *
 * @param room The room to be modified.
 *
 * Pre: The room must to be different from NULL.
 * Post: The condition, enabled, is TRUE.
 */
void enable_room(Room* room) {
    room->enabled = TRUE;
}

/**
 * Disables the room, marking it as invisible and unusable.
 *
 * @param room The room to be modified.
 *
 * Pre: The room must to be different from NULL.
 * Post: The condition, enabled, is FALSE.
 */
void disable_room(Room* room) {
    room->enabled = FALSE;
}

/**
 * Checks if the room is enabled or disabled.
 *
 * @param room The room to be checked.
 * @return TRUE if the room is enabled, FALSE otherwise.
 *
 * Pre: The room must to be different from NULL.
 * Post: Returns TRUE or FALSE based on The room enabled or disabled.
 */
int is_enabled(Room* room) {
    return room->enabled;
}

/**
 * Marks the room as visited.
 *
 * @param room The room to be modified.
 *
 * Pre: The room must to be different from NULL.
 * Post: The room condition, visited, is TRUE.
 */
void mark_visited(Room* room) {
    room->visited = TRUE;
}

/**
 * Removes the visited mark of the room, if marked.
 *
 * @param room The room to be modified.
 *
 * Pre: The room must to be different from NULL.
 * Post: The room condition, visited, is FALSE.
 */
void clear_visited(Room* room) {
    room->visited = FALSE;
}

/**
 * Checks if the room has been marked as visited.
 *
 * @param room The room to be checked.
 * @return TRUE if the room is marked as visited, FALSE otherwise.
 *
 * Pre: The room must to be different from NULL.
 * Post: Returns TRUE or FALSE based on The room visited or clear_visited.
 */
int is_visited(Room* room) {
    return room->visited;
}

/**
 * Returns the (reference to) the wall of the room corresponding to the direction.
 * @param room The room from were the wall is being looked up.
 * @param direction The direction where the wall sits.
 * @return (A reference to) The wall of the room sitting in the input direction.
 *
 * Pre: The room and direction must to be different from NULL.
 * Post: Returns one of the four walls of The room by char input.
 */
Wall* get_wall(Room* room, char direction) {
    switch(direction){
        case 'N':
            return &room->walls[0];
        case 'E':
            return &room->walls[1];
        case 'S':
            return &room->walls[2];
        case 'W':
            return &room->walls[3];
        default:
            return NULL;
    }
}

/**
 * Add a trap in a specified Room.
 * @param room The room to add the trap.
 *
 * Pre: The room must to be different from NULL.
 */

void add_trap(Room* room){
    room->trap.trap = TRUE;
    room->trap.activated = TRUE;
    trap_damage(room);
}

/**
 * Returns if there is a trap or not.
 * @param room The room check if there is a trap.
 * @return TRUE if the room have a trap, FALSE if not.
 *
 * Pre: The room must to be different from NULL.
 * Post: Returns the value of the trap.trap, that indicates if there is a trap or not.
 */

int has_trap(Room* room){
    return room->trap.trap;
}

/**
 * Returns if the trap is active or not.
 * @param room The room to check if there is an active trap.
 * @return TRUE if the room have a trap, FALSE if not.
 *
 * Pre: The room must to be different from NULL.
 * Post: Returns the value of the trap.trap and trap.activated, that indicates if there is an active trap or not.
 */

int has_active_trap(Room* room){
    if (has_trap(room) && room->trap.activated == TRUE){
        return TRUE;
    }else{
        return FALSE;
    }
}

/**
 * Deactivate the trap in the specified room.
 * @param room The room to deactivate the trap.
 *
 * Pre: The room must to be different from NULL.
 * Post: Deactivates the trap.
 */

void deactivate_trap(Room* room){
    room->trap.activated = FALSE;
}

/**
 * Returns a pseudo-random integer.
 * @param room The room with a seed, the minimum and maximum values of the random value and an index for better randomize.
 * @return A pseudo-random number.
 *
 * Pre: The room must to be different from NULL.
 * Post: Returns a pseudo-random number.
 */

int random_number(Room* room, int min, int max, int idx){
    int result = 0, aux = 0;
    room->seed = (room->seed + ((idx * 4573 + max / 65) * (rand() % 47)) % 53);

    if (min < max)
    {
        max++; // include max_num in output
    } else {
        aux = min;
        min = max+1;
        max = aux;
    }
    srand(room->seed);
    srand(rand() * (room->seed * idx * 5645) / 354);
    result = ((rand() - idx) % (max - min)) + min;
    return result;
}

/**
 * Sets the damage of a trap.
 * @param room The room of the trap to set the damage.
 *
 * Pre: The room must to be different from NULL.
 * Post: Use a pseudo-random number of the random_number to set the damage of the trap.
 */

void trap_damage(Room* room){
    int n = 0;
    for (int i = 0; i < WALLS; i++){
        n = n + room->walls[i].has_door;
    }
    int damage = random_number(room, 1, 6, n);
    room->trap.damage = damage;
}
