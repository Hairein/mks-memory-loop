// simple_example.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mksmemoryloop.h"

int main(int argc, char *argv[]) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    getcwd(buffer, 1024);

    printf("Simple example using mksmemoryloop.\n");
    printf("CWD:%s\n", buffer);

    uint8_t result = mksml_initialize("simple_example.cfg");
    if(result == MKS_NO_ERROR) { 
        // Wait for key
        printf("Press ENTER to exit.\n");
        getchar();

        mksml_uninitialize();
    }

    return EXIT_SUCCESS;
}
