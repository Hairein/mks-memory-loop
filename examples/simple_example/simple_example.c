// simple_example.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>

#include "mksmemoryloop.h"

int main(int argc, char *argv[]) {
    printf("Simple example using mksmemoryloop.\n");

    mksml_initialize();
    
    // Wait for key
    printf("Press ENTER to exit.\n");
    getchar();

    mksml_uninitialize();
    
    return EXIT_SUCCESS;
}
