// main_thread.c
// Author: Micah Koleoso Software, 2022

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "mksstructures.h"
#include "mksmemoryloop.h"
#include "main_thread.h"

#define MICROSECONDS_PER_MILLISECOND 1000

extern pthread_mutex_t access_mutex;

void* main_thread_function(void* ptr) {
    struct ThreadInfoBlock* threadInfoBlock_ptr = ptr;
 
    useconds_t sleep_interval_max = threadInfoBlock_ptr->frame_interval_ms * MICROSECONDS_PER_MILLISECOND;

    while(!threadInfoBlock_ptr->quit_flag) {
        pthread_mutex_lock(&access_mutex);

        perform_read(threadInfoBlock_ptr);

        perform_write(threadInfoBlock_ptr);

        pthread_mutex_unlock(&access_mutex);

        if(sleep_interval_max > 0) { 
            usleep(sleep_interval_max);
        } 
    } 

    // clear the frame before quitting
    pthread_mutex_lock(&access_mutex);

    clear_frame(threadInfoBlock_ptr->platform_index, threadInfoBlock_ptr);

    perform_write(threadInfoBlock_ptr);
   
    pthread_mutex_unlock(&access_mutex);

    return (void*)NULL;
}

void perform_read(struct ThreadInfoBlock* threadInfoBlock_ptr) {
    while(true) {
        fd_set read_set;

        FD_ZERO(&read_set);

        // check if socket is ready for reading anything
        FD_SET(threadInfoBlock_ptr->socket, &read_set);          

        struct timeval timeval_shortwait;
        memset(&timeval_shortwait, 0, sizeof(struct timeval));
        //timeval_shortwait.tv_usec = 5000;
        
        int select_result = select(threadInfoBlock_ptr->max_socket_nr, &read_set, NULL, NULL, &timeval_shortwait);
        assert(select_result >= 0);

        // break if nothing to read/timeout
        if(select_result == 0) {
            break;
        }

        // if there's somewthing to read, recv_from any host, confirm it's in federates
        if(FD_ISSET(threadInfoBlock_ptr->socket, &read_set)) {
            uint8_t temp_frame[MKS_MAX_FRAME_SIZE]; 
            memset(&temp_frame, 0, sizeof(uint8_t) * MKS_MAX_FRAME_SIZE);

            struct sockaddr src_addr; 
            socklen_t addrlen = sizeof(struct sockaddr);

            ssize_t recv_size = recvfrom(threadInfoBlock_ptr->socket, 
                temp_frame, sizeof(uint8_t) * MKS_MAX_FRAME_SIZE,
                0,
                &src_addr, &addrlen);
            assert(recv_size >= 0);

            struct sockaddr_in* parsed_src_addr = (struct sockaddr_in*)&src_addr;

            // find if message came from one of the hosts
            for(uint8_t federate_index = 0; federate_index < threadInfoBlock_ptr->nos_hosts; federate_index++) {
                if(federate_index == threadInfoBlock_ptr->platform_index) {
                    continue;
                }

                if(parsed_src_addr->sin_addr.s_addr != threadInfoBlock_ptr->addresses[federate_index].sin_addr.s_addr 
                    || parsed_src_addr->sin_port != threadInfoBlock_ptr->addresses[federate_index].sin_port) {
                    continue;
                }

                // copy temp frame to federate frame
                memcpy(threadInfoBlock_ptr->frames[federate_index], temp_frame, MKS_MAX_FRAME_SIZE);
                
                // printf("mks-main_thread::perform_read: platform(%d) received frame from federate(%d) with size(%d)\n",
                //     threadInfoBlock_ptr->platform_index,
                //     federate_index,
                //     (int)recv_size);

                break;
            }
        }
    }
}

void perform_write(struct ThreadInfoBlock* threadInfoBlock_ptr) {
    fd_set write_set;

    for(uint8_t federate_index = 0; federate_index < threadInfoBlock_ptr->nos_hosts; federate_index++) {
        if(federate_index == threadInfoBlock_ptr->platform_index) {
            continue;
        }

        FD_ZERO(&write_set);

        // check if socket is ready for writing to specific host
        FD_SET(threadInfoBlock_ptr->socket, &write_set);    

        int select_result = select(threadInfoBlock_ptr->max_socket_nr, NULL, &write_set, NULL, NULL);
        assert(select_result > 0);

        ssize_t sent_size = sendto(threadInfoBlock_ptr->socket, 
            threadInfoBlock_ptr->frames[threadInfoBlock_ptr->platform_index], sizeof(unsigned char) * MKS_MAX_FRAME_SIZE,
            0,
            (struct sockaddr*)&threadInfoBlock_ptr->addresses[federate_index], sizeof(struct sockaddr_in));
        assert(sent_size >= 0);
    }
}

void clear_frames(struct ThreadInfoBlock* threadInfoBlock_ptr) {
    for(uint8_t federate_index = 0; federate_index < threadInfoBlock_ptr->nos_hosts; federate_index++) {
        clear_frame(federate_index, threadInfoBlock_ptr);
    }
}

void clear_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr) {
    assert(platform_index >= 0 && platform_index < MKS_MAX_FEDERATES);

    memset(&threadInfoBlock_ptr->frames[platform_index], 0, sizeof(uint8_t) * MKS_MAX_FRAME_SIZE);
} 