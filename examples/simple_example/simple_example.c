// simple_example.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>

#include "mksmemoryloop.h"

int main(int argc, char *argv[]) {
    printf("Simple example using mksmemoryloop.\n");

    char* hostnames[3] = {
        "192.168.2.194",
        "192.168.2.194",
        "192.168.2.194"
    };
    uint16_t ports[3] = {
        13201,
        13202,
        13203
    };

    uint8_t result = mksml_initialize(3, 20, hostnames, ports, 3);
    if(result == MKS_NO_ERROR) { 
        // Wait for key
        printf("Press ENTER to exit.\n");
        getchar();

        mksml_uninitialize();
    }

    return EXIT_SUCCESS;
}
