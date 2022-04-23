// config_parser.h
// Author: Micah Koleoso Software, 2022

#ifndef MKS_FILE_READ_H
#define MKS_FILE_READ_H

#include <stdio.h>
#include <stdbool.h>

bool read_next_line_int(FILE * fp, int* value);
bool read_next_line_string(FILE * fp, char* text);

#endif
