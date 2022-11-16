//
// Created by rcarlini on 14/10/19.
//

#ifndef DUNGEON_UTILS_H
#define DUNGEON_UTILS_H

#include <stdio.h>

#define OPTION_INVALID -1

#define ERROR_LEVEL 0
#define WARN_LEVEL 1
#define INFO_LEVEL 2

#define LOG_LEVEL 2

int read_int_option(const char* msg);
char read_char_option(const char* msg);


void flush_input();

void log_warn(FILE* fd, char* msg);
void log_error(FILE* fd, char* msg);
void log_info(FILE* fd, char* msg);


#endif //DUNGEON_UTILS_H
