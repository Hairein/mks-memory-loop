// simple_example.c
// Author: Micah Koleoso Software, 2021

#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "mksmemoryloop.h"

static void catch_sigint(int sig_nr);
static bool install_sigint_signal();

volatile sig_atomic_t sigint = 1;

struct basic_counter {
    uint16_t counter;
};

//
// This example shows using a struct over the raw frame pointer between begin_frame and end_frame.
//
int main(int argc, char *argv[]) {
    // char buffer[1024];
    // memset(buffer, 0, 1024);
    // getcwd(buffer, 1024);

    if(argc != 2) {
        printf("USAGE ./simple_example <cfg_filename>\n");
    }

    printf("Simple example using mksmemoryloop.\n");
    //printf("CWD:%s\n", buffer);

    if(!install_sigint_signal()) {
        printf("Error: Unable to install SIGINT signal handler for <CTRL+C> break.\n");
        
        return EXIT_SUCCESS;
    }

    printf("Press <CTRL+C> to quit.\n\n");

    uint8_t result = mksml_initialize(argv[1]);
    if(result == MKS_NO_ERROR) { 
        uint8_t platform_index = mksml_get_platform_index();
        uint8_t nos_federates = mksml_get_nos_federates();

        while(sigint) {
            for(uint8_t index = 0; index < nos_federates; index++) {
                uint16_t counter = 0;

                // Use the frame pointer begin_frame/end_frame duration for as short a time as possible
                // since this prevents network updates during this period. Only access 1 pair for a frame at a time
                uint8_t* frame_pointer = mksml_begin_frame(index); 
                if(frame_pointer != NULL) {              
                    struct basic_counter* struct_pointer = (struct basic_counter*)frame_pointer;
    
                    counter = struct_pointer->counter;

                    if(index == platform_index) {
                        struct_pointer->counter = struct_pointer->counter + 1;
                        if(struct_pointer->counter >= 32000) {
                            struct_pointer->counter = 0;
                        }
                    }

                    mksml_end_frame();
                }

                // write the counter
                printf("%5d(%5d) ", index, counter);
            }

            printf("\r");
        }
    }

    printf("\n");

    mksml_uninitialize();


    return EXIT_SUCCESS;
}

static bool install_sigint_signal() {
    struct sigaction sig_new;

    sig_new.sa_handler = catch_sigint;
    sigemptyset(&sig_new.sa_mask);
    sig_new.sa_flags = 0; // SA_RESTART?

    if(sigaction(SIGINT, &sig_new, NULL) < 0) {
        return false;
    }

    return true;
}

static void catch_sigint(int sig_nr) {
    sigint--;
}
