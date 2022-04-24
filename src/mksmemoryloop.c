// mksmemoryloop.c
// Author: Micah Koleoso Software, 2021

#define _DEFAULT_SOURCE

#include <pthread.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "mksmemoryloop.h"
#include "mksstructures.h"
#include "main_thread.h"
#include "file_read.h"

uint8_t mksml_initialize_internal(uint8_t platform_index, uint16_t frame_interval_ms, char hostnames[MKS_MAX_FEDERATES][MKS_MAX_ADDRESS_LENGTH], uint16_t* ports, uint8_t nos_hosts);

pthread_t __thread main_thread;
bool __thread is_running = false;

struct ThreadInfoBlock __thread threadInfoBlock;

pthread_mutex_t access_mutex = PTHREAD_MUTEX_INITIALIZER;

uint8_t mksml_initialize(char* cfg_filename)
{
    FILE * fp;
    int tmp_int;
    bool parse_successful = true;

    uint8_t nos_hosts;
    char hostnames[MKS_MAX_FEDERATES][MKS_MAX_ADDRESS_LENGTH];
    uint16_t ports[MKS_MAX_FEDERATES];
    uint8_t platform_index;
    uint16_t frame_interval_ms;

    memset(hostnames, 0, sizeof(char) * (MKS_MAX_FEDERATES * MKS_MAX_ADDRESS_LENGTH));

    fp = fopen(cfg_filename, "r");
    if (fp == NULL) return MKS_ERROR_READING_CFG_FILE;

    if(!read_next_line_int(fp, &tmp_int) || tmp_int < 1 || tmp_int >= MKS_MAX_FEDERATES) {
        fclose(fp);
        return MKS_ERROR_READING_CFG_FILE;
    }
    nos_hosts = tmp_int;

    for(size_t index = 0;index < nos_hosts;index++) {
        if(!read_next_line_string(fp, &hostnames[index][0])) {
            parse_successful = false;
            break;
        }

        if(!read_next_line_int(fp, &tmp_int)) {
            parse_successful = false;
            break;
        }
        ports[index] = tmp_int;
    }

    if(!parse_successful) {
        fclose(fp);
        return MKS_ERROR_READING_CFG_FILE;
    }

    if(!read_next_line_int(fp, &tmp_int)) {
        fclose(fp);
        return MKS_ERROR_READING_CFG_FILE;
    }
    platform_index = tmp_int;

    if(!read_next_line_int(fp, &tmp_int)) {
        fclose(fp);
        return MKS_ERROR_READING_CFG_FILE;
    }
    frame_interval_ms = tmp_int;

    fclose(fp);

    uint8_t result = mksml_initialize_internal(platform_index, frame_interval_ms, hostnames, ports, nos_hosts);

    return result;
}

void mksml_uninitialize() {
    //printf("mks-memory-loop: uninitialize.\n");
 
    if(is_running) { 
        threadInfoBlock.quit_flag = true;

        //printf("mks-memory-loop: awaiting shutdown.\n");

        pthread_join(main_thread, NULL);
    }
   
    if(threadInfoBlock.socket != -1) { 
        close(threadInfoBlock.socket); 
    } 

    //printf("mks-memory-loop: shutdown.\n");
} 

uint8_t mksml_get_platform_index() {
    return threadInfoBlock.platform_index;
}

uint8_t mksml_get_nos_federates() {
    return threadInfoBlock.nos_hosts;
}

uint8_t* mksml_begin_frame(uint8_t frame_index) {
    if(!is_running || frame_index >= threadInfoBlock.nos_hosts) {
        return NULL;
    }

    pthread_mutex_lock(&access_mutex);

    return &threadInfoBlock.frames[frame_index][0];
}

void mksml_end_frame() {
    if(!is_running) {
        return;
    }

    pthread_mutex_unlock(&access_mutex);
}

uint8_t mksml_initialize_internal(uint8_t platform_index, uint16_t frame_interval_ms, char hostnames[MKS_MAX_FEDERATES][MKS_MAX_ADDRESS_LENGTH], uint16_t* ports, uint8_t nos_hosts) {
    //printf("mks-memory-loop: initialize.\n");

    memset(&threadInfoBlock, 0, sizeof(struct ThreadInfoBlock));
   
    //printf("mks-memory-loop: platform_index=%d.\n", platform_index);
    //printf("mks-memory-loop: frame_interval_ms=%d.\n", frame_interval_ms);
    //printf("mks-memory-loop: nos_hosts=%d.\n", nos_hosts);

    threadInfoBlock.quit_flag = false;
    threadInfoBlock.platform_index = platform_index;
    threadInfoBlock.frame_interval_ms = frame_interval_ms;
    threadInfoBlock.nos_hosts = nos_hosts;
     
     // determine host addresses
    for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++) {
        //printf("mks-memory-loop: host=%d) %s:%d.\n", federate_index, hostnames[federate_index], ports[federate_index]);

        memset(&threadInfoBlock.addresses[federate_index], 0, sizeof(struct sockaddr_in));
        threadInfoBlock.addresses[federate_index].sin_family = AF_INET;
        threadInfoBlock.addresses[federate_index].sin_port = htons(ports[federate_index]);
        inet_aton(hostnames[federate_index], (struct in_addr*)&threadInfoBlock.addresses[federate_index].sin_addr.s_addr);
    } 

    // create socket
    threadInfoBlock.socket = socket(AF_INET, SOCK_DGRAM, 0);  
    threadInfoBlock.max_socket_nr = threadInfoBlock.socket + 1;
    
    if(bind(threadInfoBlock.socket, (struct sockaddr*)&threadInfoBlock.addresses[platform_index], sizeof(struct sockaddr_in)) != 0) {  
        switch(errno)
        {
            case EACCES: printf("EACCES"); break; 
            case EADDRINUSE: printf("EADDRINUSE"); break; 
            case EBADF: printf("EBADF"); break; 
            case EINVAL: printf("EINVAL"); break; 
            case ENOTSOCK: printf("ENOTSOCK"); break; 
            default: printf("default"); break;                   
        }
    }   

    // start main thread
    int thread_create_result = pthread_create(&main_thread, NULL, main_thread_function, (void*) &threadInfoBlock);
    if(thread_create_result != 0) {
        printf("mks-memory-loop: Error creating main thread.\n");
        return false;
    } 

    is_running = true;

    //printf("mks-memory-loop: running.\n");

    return MKS_NO_ERROR;
} 
