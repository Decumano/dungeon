cmake_minimum_required(VERSION 3.14.5)
project(dungeon C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
set(CMAKE_CXX_STANDARD 14)
set(DUNGEON_FILES ${PROJECT_SOURCE_DIR}/src)

#add_subdirectory(tests)

add_executable(dungeon src/sources/main.c
        src/headers/common.h
        src/sources/utils.c src/headers/utils.h
        src/sources/wall.c src/headers/wall.h
        src/sources/room.c src/headers/room.h
        src/sources/dungeon.c src/headers/dungeon.h
        src/sources/menu.c src/headers/menu.h
        src/sources/state.c src/headers/state.h
        src/sources/dungeon_utils.c src/headers/dungeon_utils.h
        src/sources/game.c src/headers/game.h)

add_custom_command(TARGET dungeon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/resources/
                $<TARGET_FILE_DIR:dungeon>/resources/)

