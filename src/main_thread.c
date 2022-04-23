// main_thread.c
// Author: Micah Koleoso Software, 2022

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <pthread.h>

#include "mksstructures.h"
#include "mksmemoryloop.h"
#include "main_thread.h"

#define MICROSECONDS_PER_MILLISECOND 1000

pthread_mutex_t access_mutex = PTHREAD_MUTEX_INITIALIZER;

void* main_thread_function(void* ptr) {
    struct ThreadInfoBlock* threadInfoBlock_ptr = ptr;
 
    useconds_t sleep_interval_max = threadInfoBlock_ptr->frame_interval_ms * MICROSECONDS_PER_MILLISECOND;

    while(!threadInfoBlock_ptr->quit_flag) {
        // fd_set read_set;
        // fd_set write_set;
        // fd_set except_set;
        
        // perform_read(&read_set, &except_set, threadInfoBlock_ptr);

        // perform_write(&write_set, &except_set, threadInfoBlock_ptr);

        if(sleep_interval_max > 0) { 
            usleep(sleep_interval_max);
        } 
    } 

    return (void*)NULL;
}


void perform_read(fd_set* read_set, fd_set* except_set, struct ThreadInfoBlock* threadInfoBlock_ptr)
{
    FD_ZERO(read_set);
    FD_ZERO(except_set);

    // for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++) {
    //     if(threadInfoBlock.federates[federate_index] == SLOT_ERRORED) {
    //         continue;
    //     }

    //     if(federate_index != threadInfoBlock_ptr->platform_index) {
    //         FD_SET(threadInfoBlock.sockets[federate_index], read_set);
    //     }

    //     FD_SET(threadInfoBlock.sockets[federate_index], except_set);            
    // } 

    // return select(threadInfoBlock.max_socket_nr, read_set, NULL, except_set, NULL);
}

void perform_write(fd_set* write_set, fd_set* except_set, struct ThreadInfoBlock* threadInfoBlock_ptr)
{
    FD_ZERO(write_set);
    FD_ZERO(except_set);

    //select(threadInfoBlock.max_socket_nr, NULL, write_set, except_set, NULL);
}
