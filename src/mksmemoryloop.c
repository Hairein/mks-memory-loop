// mksmemoryloop.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "mksmemoryloop.h"
#include "mksstructures.h"

void* main_thread_function(void* ptr);

uint8_t __thread federates[MAX_FEDERATES];
uint8_t __thread frames[MAX_FEDERATES][FRAME_SIZE];  

pthread_t __thread main_thread;
bool __thread is_running = false;

struct ThreadInfoBlock __thread threadInfoBlock;

bool mksml_initialize(uint8_t platform_index, uint16_t frame_interval_ms) {
    printf("mks-memory-loop: initialize.\n");

    memset(federates, 0, sizeof(uint8_t) * MAX_FEDERATES);
    memset(frames, 0, sizeof(uint8_t) * (MAX_FEDERATES * FRAME_SIZE));

    memset(&threadInfoBlock, 0, sizeof(struct ThreadInfoBlock));
    threadInfoBlock.quit_flag = false;
    threadInfoBlock.platform_index = platform_index;
    threadInfoBlock.frame_interval_ms = frame_interval_ms;

    printf("mks-memory-loop: platform_index=%d.\n", platform_index);
    printf("mks-memory-loop: frame_interval_ms=%d.\n", frame_interval_ms);

    int thread_create_result = pthread_create(&main_thread, NULL, main_thread_function, (void*) &threadInfoBlock);
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
        threadInfoBlock.quit_flag = true;

        printf("mks-memory-loop: awaiting shutdown.\n");

        pthread_join(main_thread, NULL);
    }

    printf("mks-memory-loop: shutdown.\n");
} 

void* main_thread_function(void* ptr) {
    struct ThreadInfoBlock* threadInfoBlock_ptr = ptr;
 
    useconds_t sleep_interval_max = threadInfoBlock_ptr->frame_interval_ms * MICROSECONDS_PER_MILLISECOND;

    while(!threadInfoBlock_ptr->quit_flag){
        usleep(sleep_interval_max);
    } 

    return (void*)NULL;
}
