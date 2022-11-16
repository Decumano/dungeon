//
// Created by rcarlini on 22/10/19.
//

#include <stdio.h>
#include <string.h>
#include "../headers/dungeon_utils.h"
#include <stdlib.h>

void draw_top(FILE *fd, Room *room) {
    fprintf(fd, "%s", UPPER_LEFT_CORNER);

    Wall* wall = get_wall(room, NORTH);
    if (has_open_door(wall)) {
        fprintf(fd, HORIZONTAL_OPEN_DOOR);
    } else if (has_door(wall)) {
        fprintf(fd, HORIZONTAL_CLOSED_DOOR);
    } else {
        fprintf(fd, "%s", HORIZONTAL_WALL);
    }
    fprintf(fd, "%s", UPPER_RIGHT_CORNER);
}

void draw_middle(FILE *fd, Room *room, int is_initial, int is_current, int is_monster) {

    Wall* wall = get_wall(room, WEST);
    if (has_open_door(wall)) {
        fprintf(fd, VERTICAL_OPEN_DOOR);
    } else if (has_door(wall)) {
        fprintf(fd, VERTICAL_CLOSED_DOOR);
    } else {
        fprintf(fd, "%s", VERTICAL_WALL);
    }

    if (is_initial == TRUE) {
        fprintf(fd, INITIAL_PLACEHOLDER);
    } else {
        fprintf(fd, EMPTY_SPACE);
    }

    if (is_current == TRUE) {
        fprintf(fd, CURRENT_PLACEHOLDER);
    }else if (is_monster == TRUE){
        fprintf(fd, ENEMY_PLACEHOLDER);
    }
    else {
        fprintf(fd, EMPTY_SPACE);
    }


    fprintf(fd, EMPTY_SPACE);

    wall = get_wall(room, EAST);
    if (has_open_door(wall)) {
        fprintf(fd, VERTICAL_OPEN_DOOR);
    } else if (has_door(wall)) {
        fprintf(fd, VERTICAL_CLOSED_DOOR);
    } else {
        fprintf(fd, "%s", VERTICAL_WALL);
    }
}

void draw_bottom(FILE *fd, Room *room){
    fprintf(fd, "%s", BOTTOM_LEFT_CORNER);

    Wall *wall = get_wall(room, SOUTH);
    if (has_open_door(wall)) {
        fprintf(fd, HORIZONTAL_OPEN_DOOR);
    } else if (has_door(wall)) {
        fprintf(fd, HORIZONTAL_CLOSED_DOOR);
    } else {
        fprintf(fd, "%s", HORIZONTAL_WALL);
    }
    fprintf(fd, "%s", BOTTOM_RIGHT_CORNER);
}

void draw_room(FILE *fd, Room *room, int line, int is_initial, int is_current, int is_monster, int hide_disabled, int hide_non_visited) {
    if (is_enabled(room) == FALSE && hide_disabled == TRUE) {
        fprintf(fd, EMPTY_CELL);

    } else if (line == 0 && (is_visited(room) == TRUE || hide_non_visited == FALSE)) {
        draw_top(fd, room);

    } else if (line == 1 && (is_visited(room) == TRUE || hide_non_visited == FALSE)) {
        draw_middle(fd, room, is_initial, is_current, is_monster);

    } else if (line == 2 && (is_visited(room) == TRUE || hide_non_visited == FALSE)) {
        draw_bottom(fd, room);

    } else {
        // error
    }
}

void
draw_dungeon(FILE *fd, Dungeon *dungeon, Room* current_room, Room** monster_room, int sizeEn, int hide_disabled, int draw, const int* mhp) {

    Position position = get_starting_position(dungeon);
    for (int idx=0; idx<dungeon->rows; idx++) {
        for (int line=0; line<3; line++) {
            for (int jdx = 0; jdx < dungeon->columns; jdx++) {
                Room* room = get_room_at(dungeon, idx, jdx);

                int is_initial = FALSE;
                if (line == 1 && position.row == idx && position.column == jdx) {
                    is_initial = TRUE;
                }

                int is_current = FALSE;
                if (current_room != NULL && current_room == room) {
                    is_current = TRUE;
                }
                int is_monster = FALSE;
                for (int i = 0; i < sizeEn; i++) {
                    if (monster_room[i] == room && mhp[i] > 0) {
                        is_monster = TRUE;
                        break;
                    }
                }

                if (draw){
                    draw_room(fd, room, line, is_initial, is_current, is_monster, hide_disabled, TRUE);
                }else{
                    draw_room(fd, room, line, is_initial, is_current, is_monster, hide_disabled, FALSE);
                }
                fprintf(fd, " ");
            }
            fprintf(fd, "\n");
        }
    }
    fprintf(fd, "\n");
}

/**
 * Creates a simple dungeon with a single room with a closed entrance door at the west wall, labeled as Entrance, and
 * an open exit door at the east wall, labeled as Exit.
 *
 * @param dungeon Dungeon structure to be initialized.
 */
void init_simple_dungeon(Dungeon* dungeon) {

    init_dungeon(dungeon);

    Room* start = get_starting_room(dungeon);
    enable_room(start);

    Wall* north_wall = get_wall(start, NORTH);
    set_wall_label(north_wall, "Exit");
    add_exit_door(north_wall);
}

/**
 * Creates a simple dungeon with two rooms:
 * - The starting room with a closed entrance door at the west wall, labeled as Entrance, and
 * an opened door at the east wall.
 * - The second room with an opened door at the west wall and an opened door, labeled as Exit, at the north wall.
 *
 * @param dungeon Dungeon structure to be initialized.
 */
void init_two_rooms_dungeon(Dungeon *dungeon) {

    init_dungeon(dungeon);
    {
        Room* start = get_starting_room(dungeon);
        enable_room(start);

        Wall* east_wall = get_wall(start, EAST);
        add_door(east_wall);
    }

    {
        Position position;
        position = get_starting_position(dungeon);
        position.column++;

        Room* second_room = get_room_at_position(dungeon, position);
        enable_room(second_room);

        Wall* west_wall = get_wall(second_room, WEST);
        add_door(west_wall);

        Wall* north_wall =  get_wall(second_room, NORTH);
        set_wall_label(north_wall, "Exit");
        add_exit_door(north_wall);
    }
}

/**
 * Tries to parse the room data contained in data parameter, initializing the corresponding room accordingly.
 *
 * @param dungeon The dungeon to be initialized.
 * @param row Row coordinate of the room.
 * @param column Column coordinate of the room.
 * @param data The encoded room data.
 * @return SUCCESS if the room data was parsed properly or INVALID_ROOM_DATA if the room data is invalid.
 *
 * Pre: Pointers od dungeon and data are different from NULL. And row and column are positive integers.
 * Post: If the room is initialized with the data SUCCESS is returned. And INVALID_ROOM_DATA if the room data is invalid.
 */
int read_room_data(Dungeon* dungeon, int row, int column, const char* data) {

    Room* temp = get_room_at(dungeon, row, column);

    temp->enabled = TRUE;

    if (*data != 'N' && *data != 'S' && *data != 'E' && *data != 'W' && *data != 'I' && *data != 'X' && *data != 'C') {
        return INVALID_ROOM_DATA;
    }
    else if (*data == 'I') {
        set_starting_position(dungeon, row, column);
    }else {
        Wall *wall = get_wall(temp, *data);
        if (wall != NULL) {
            add_door(wall);
            if (*(data + sizeof(char)) == 'C')  {
                close_door(wall);
                if (*(data + 2*sizeof(char)) == 'X') {
                    add_exit_door(wall);
                }
            }else if(*(data + sizeof(char)) == 'X'){
                add_exit_door(wall);
                if (*(data + 2*sizeof(char)) == 'C'){
                    close_door(wall);
                }
            }
        }
    }
    return SUCCESS;
}

/**
 * Reads a line from the file fd and tries to parse the room data contained in it.
 *
 * @param dungeon The dungeon to be initialized with the contents of the file.
 * @param fd The opened file containing a dungeon map encoded as a text.
 * @return SUCCESS if the room data was parsed properly, INVALID_ROOM_LINE if the format of a room line is invalid or INVALID_ROOM_DATA if the room data is invalid.
 *
 * Pre: The dungeon pointer, and file descriptor are different from Null. numRows and numCols are smaller than RoWS and COLUMNS.
 * Post: If the line is read properly, SUCCESS is returned. INVALID_ROOM_LINE if the line has incorrect format. And INVALID_ROOM_DATA if the room data is invalid.
 */
int read_room_line(Dungeon* dungeon, FILE* fd, int numRows, int numCols) {

    int row, column, max_info = 10;
    char roomInfo[10 /*max_info*/];
    int info_len = 0;

    if(fscanf(fd, "%d %d %s", &row, &column, roomInfo) != 3 || row>numRows || column>numCols){
        return INVALID_ROOM_LINE;
    }
    info_len = (int)strlen(roomInfo);
    for(int i = 0; i<info_len; i++) {
        read_room_data(dungeon, row, column, &roomInfo[i]);
    }

    return SUCCESS;
}

/**
 * Reads the file in fd and tries to read a dungeon map from it.
 *
 * @param dungeon The dungeon to be initialized with the contents of the file.
 * @param fd The opened file containing a dungeon map encoded as a text.
 * @return SUCCESS if the dungeon was loaded properly, INVALID_ROOM_LINE if the format of a room line is invalid or INVALID_ROOM_DATA if the room data is invalid.
 *
 * Pre: The dungeon pointer is different from Null and the file descriptor is also different from Null.
 * Post: If the dungeon is initialized with the data SUCCESS is returned. INVALID_ROOM_LINE if the line has incorrect format. And INVALID_ROOM_DATA if the room data is invalid.
 */
int load_dungeon_file(Dungeon* dungeon, FILE* fd) {

    int numRows, numCols;

    fscanf(fd, "%dx%d", &numRows, &numCols);

    int prv_columns = dungeon->columns;
    dungeon->columns = numCols;
    dungeon->rows = numRows;
    Room** aux = (Room**)realloc(dungeon->map, (numCols)*sizeof(Room*));
    if (aux == NULL){
        free(aux);
    }else {
        int col1, col2;
        if (prv_columns > dungeon->columns){
            col1 = prv_columns, col2 = dungeon->columns;
        }else{
            col1 = dungeon->columns, col2 = prv_columns;
        }
        for (int i = 0; i < col1; i++) {
            Room *aux1;
            if (i < col2) {
                aux1 = (Room *) malloc((dungeon->rows) * sizeof(Room));
            } else {
                aux1 = (Room *) malloc(dungeon->rows * sizeof(Room));
                if (prv_columns > dungeon->columns) {
                    free(dungeon->map[i]);
                }
            }
            if (aux1 == NULL) {
                free(aux1);
            } else {
                aux[i] = aux1;
            }
            for (int j = 0; j < dungeon->rows; j++) {
                init_room(&aux[i][j]);
            }
        }
        dungeon->map = aux;
    }
    while (!feof(fd)){
            int ret = read_room_line(dungeon, fd, numRows, numCols);
            if (ret == INVALID_ROOM_LINE || ret == INVALID_ROOM_DATA){
                return ret;
            }
        }
    return SUCCESS;
}

/**
 * Opens the file in path and tries to read a dungeon map from it.
 *
 * @param dungeon The dungeon to be initialized with the contents of the file in path.
 * @param path The path to the file containing a dungeon map encoded as a text file.
 * @return SUCCESS if the dungeon was loaded properly, FILE_NOT_FOUND code if the file was not found, INVALID_ROOM_LINE if the format of a room line is invalid or INVALID_ROOM_DATA if the room data is invalid.
 *
 * Pre: The dungeon pointer is different from Null.
 * Post: If the dungeon is initialized with the data SUCCESS is returned. FILE_NOT_FOUND if the file is not located. INVALID_ROOM_LINE if the line has incorrect format. And INVALID_ROOM_DATA if the room data is invalid.
 */
int load_dungeon(Dungeon* dungeon, char* path) {

    init_dungeon(dungeon);

    FILE* fp;
    fp = fopen(path, "r");

    if(fp == NULL)
    {
        printf("Cannot open file \n");
        return FILE_NOT_FOUND;
    }

    load_dungeon_file(dungeon, fp);

    fclose(fp);

    return SUCCESS;
}