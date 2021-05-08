// mksmemoryloop.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "mksmemoryloop.h"

void* main_thread_function(void* ptr);

uint8_t __thread federates[MAX_FEDERATES];
uint8_t __thread frames[MAX_FEDERATES][FRAME_SIZE];  

pthread_t __thread main_thread;
bool __thread is_running = false;

bool __thread quit_flag = false;

bool mksml_initialize() {
    printf("mks-memory-loop: initialize.\n");

    memset(federates, 0, sizeof(uint8_t) * MAX_FEDERATES);
    memset(frames, 0, sizeof(uint8_t) * (MAX_FEDERATES * FRAME_SIZE));

    int thread_create_result = pthread_create(&main_thread, NULL, main_thread_function, (void*) NULL);
    if(thread_create_result != 0) {
        printf("mks-memory-loop: Error creating main thread.\n");
        return false;
    } 

    is_running = true;

    printf("mks-memory-loop: running.\n");

    return true;
} 

void mksml_uninitialize() {
    printf("mks-memory-loop: uninitialize.\n");

    if(is_running) { 
        quit_flag = true;

        pthread_join(main_thread, NULL);
    }

    printf("mks-memory-loop: shutdown.\n");
} 

void* main_thread_function(void* ptr) {
    while(!quit_flag){
        sleep(1);
    } 

    return (void*)NULL;
}
