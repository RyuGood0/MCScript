#ifndef UTILS_H
#define UTILS_H
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(char* filename);

void write_to_file(parser_T* parser, char* filename);

char* get_filename_ext(const char *filename);

#endif