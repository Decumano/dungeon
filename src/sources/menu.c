//
// Created by rcarlini on 20/10/19.
//
#include <stdio.h>
#include <string.h>

#include "../headers/utils.h"
#include "../headers/dungeon_utils.h"
#include "../headers/game.h"
#include "../headers/menu.h"
#include <stdlib.h>

int load_from_file(Dungeon* dungeon) {

    char name[MAX_FILENAME];

    printf("\n");
    printf("Which dungeon do you want to load?\n");
    scanf("%s", name);
    flush_input();

    char path[MAX_FILENAME];
    strncpy(path, "resources/", MAX_FILENAME);
    strcat(path, name);
    strcat(path, ".txt");

    int status = load_dungeon(dungeon, path);

    return status;
}

int load_map(Dungeon* dungeon) {

    int status = INVALID_OPTION;
    while (status != SUCCESS && status != CANCELLED) {

        printf("\n");
        printf("Choose a map:\n");
        printf("%d. Single-room map\n", 1);
        printf("%d. Two-room map\n", 2);
        printf("%d. Load from file\n", 3);
        printf("%d. Cancel\n", 0);

        status = SUCCESS;
        int option = read_int_option("");
        switch (option) {
            case 1:
                init_simple_dungeon(dungeon);
                break;
            case 2:
                init_two_rooms_dungeon(dungeon);
                break;
            case 3:
                status = load_from_file(dungeon);
                switch (status) {
                    case SUCCESS:
                        printf("File loaded successfully!\n");
                        break;
                    default:
                        printf("Unexpected status after loading!\n");
                        break;
                }
                break;
            case 0:
                status = CANCELLED;
                break;
            default:
                status = INVALID_OPTION;
                printf("Invalid option, try again...\n");
                break;
        }
    }
    // draw_dungeon(stdout, dungeon, NULL);

    return status;
}

void show_game_menu(Dungeon* dungeon) {

    int option = OPTION_INVALID;
    while (option != OPTION_CANCEL) {

        printf("\n");
        printf( "%d. Start game\n", OPTION_START_GAME);
        printf( "%d. Show solution\n", OPTION_SHOW_SOLUTION);
        printf( "%d. Cancel\n", OPTION_CANCEL);
        option = read_int_option("Choose an option: \n");

        switch (option) {
            case OPTION_START_GAME:
                start_game(dungeon);
                break;

            case OPTION_SHOW_SOLUTION:
                solve(dungeon);
                break;

            case OPTION_CANCEL:
                for (int i = 0; i < dungeon->columns; i++){
                    free(dungeon->map[i]);
                }
                free(dungeon->map);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }
}

void show_menu() {

    printf("Welcome! What do you want to do?\n");

    int option = OPTION_INVALID;
    while (option != OPTION_QUIT) {

        printf("\n");
        printf( "%d. Load map\n",  OPTION_LOAD);
        printf( "%d. Show dungeon grid\n",  OPTION_DUMMY);
        printf( "%d. Quit\n", OPTION_QUIT);
        option = read_int_option("Choose an option: \n");

        if (option == OPTION_QUIT) {
            // Do nothing, it will exit the loop

        } else if (option == OPTION_LOAD) {

            Dungeon dungeon;
            int load_result = load_map(&dungeon);
            if (load_result == SUCCESS) {
                    show_game_menu(&dungeon);
            }
            printf("Do you want to play again?\n");

        } else if (option == OPTION_DUMMY) {

            Dungeon dungeon;
            int status = init_dungeon(&dungeon);
            if (status == SUCCESS) {
                draw_dungeon(stdout, &dungeon, NULL, NULL, 0, FALSE, FALSE, 0);

            } else {
                printf("The initialization was unsuccessful...\n");
            }

        } else {
            printf("Invalid option!\n");
        }
    }
}