// mksmemoryloop.c
// Author: Micah Koleoso Software, 2021

#define _DEFAULT_SOURCE
//#define __USE_MISC
// #define __USE_XOPEN

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mksmemoryloop.h"
#include "mksstructures.h"

#define MICROSECONDS_PER_MILLISECOND 1000

#define SLOT_UNITIALIZED 0
#define SLOT_WAITING_TO_CONNECT 1
#define SLOT_CONNECTED 2
#define SLOT_ERRORED 3

void* main_thread_function(void* ptr);

void handle_external_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr);
void handle_internal_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr);

pthread_t __thread main_thread;
bool __thread is_running = false;

struct ThreadInfoBlock __thread threadInfoBlock;

uint8_t mksml_initialize(uint8_t platform_index, uint16_t frame_interval_ms, char** hostnames, uint16_t* ports, uint8_t nos_hosts) {
    if(nos_hosts > MKS_MAX_FEDERATES){
        return MKS_ERROR_TOO_MANY_HOSTS;
    } 

    printf("mks-memory-loop: initialize.\n");

    memset(&threadInfoBlock, 0, sizeof(struct ThreadInfoBlock));
   
    printf("mks-memory-loop: platform_index=%d.\n", platform_index);
    printf("mks-memory-loop: frame_interval_ms=%d.\n", frame_interval_ms);
    printf("mks-memory-loop: nos_hosts=%d.\n", nos_hosts);

    threadInfoBlock.quit_flag = false;
    threadInfoBlock.platform_index = platform_index;
    threadInfoBlock.frame_interval_ms = frame_interval_ms;
    threadInfoBlock.nos_hosts = nos_hosts;
 
    for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++) {
        printf("mks-memory-loop: host=%d) %s:%d.\n", federate_index, hostnames[federate_index], ports[federate_index]);

        if(inet_aton(hostnames[federate_index], threadInfoBlock.hosts[federate_index]) == 1) {  
            threadInfoBlock.federates[federate_index] = SLOT_WAITING_TO_CONNECT;
            threadInfoBlock.sockets[federate_index] = socket(AF_INET, SOCK_DGRAM, 0);        
        } else {
            threadInfoBlock.federates[federate_index] = SLOT_ERRORED;
            threadInfoBlock.sockets[federate_index] = -1;        
        } 
    } 

    int thread_create_result = pthread_create(&main_thread, NULL, main_thread_function, (void*) &threadInfoBlock);
    if(thread_create_result != 0) {
        printf("mks-memory-loop: Error creating main thread.\n");
        return false;
    } 

    is_running = true;

    printf("mks-memory-loop: running.\n");

    return MKS_NO_ERROR;
} 

void mksml_uninitialize() {
    printf("mks-memory-loop: uninitialize.\n");
 
    if(is_running) { 
        threadInfoBlock.quit_flag = true;

        printf("mks-memory-loop: awaiting shutdown.\n");

        pthread_join(main_thread, NULL);
    }
   
    for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++) {
        if(threadInfoBlock.sockets[federate_index] != -1) { 
            close(threadInfoBlock.sockets[federate_index]); 
        } 
    } 

    printf("mks-memory-loop: shutdown.\n");
} 

void* main_thread_function(void* ptr) {
    struct ThreadInfoBlock* threadInfoBlock_ptr = ptr;
 
    useconds_t sleep_interval_max = threadInfoBlock_ptr->frame_interval_ms * MICROSECONDS_PER_MILLISECOND;

    while(!threadInfoBlock_ptr->quit_flag){
        for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++){
            if(threadInfoBlock.federates[federate_index] == SLOT_ERRORED) { 
                continue;
            } 

            if(federate_index != threadInfoBlock_ptr->platform_index){
                handle_external_frame(federate_index, threadInfoBlock_ptr);
            } else {
                handle_internal_frame(federate_index, threadInfoBlock_ptr);
            } 
        } 

        if(sleep_interval_max > 0){ 
            usleep(sleep_interval_max);
        } 
    } 

    return (void*)NULL;
}

void handle_external_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr) {
} 

void handle_internal_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr) {
} 
