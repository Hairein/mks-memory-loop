// config_parser.c
// Author: Micah Koleoso Software, 2022

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "mksmemoryloop.h"
#include "file_read.h"

bool read_next_line_int(FILE * fp, int* value) {
    char * line = NULL;
    size_t len = 0;

    if(fp == NULL) return false;

    ssize_t read = getline(&line, &len, fp);
    if(read == -1) return false;
    *value = atoi(line);
    
    free(line);

    return true;
}

bool read_next_line_string(FILE * fp, char* text) {
    char * line = NULL;
    size_t len = 0;

    if(fp == NULL) return false;

    ssize_t read = getline(&line, &len, fp);
    if(read == -1 || read >= MKS_MAX_ADDRESS_LENGTH) return false;
    strncpy(text, line, strlen(line));
    
    free(line);

    return true;
}
