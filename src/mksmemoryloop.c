// mksmemoryloop.c
// Author: Micah Koleoso Software, 2021

#define _DEFAULT_SOURCE

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
 #include <errno.h>

#include "mksmemoryloop.h"
#include "mksstructures.h"

#define MICROSECONDS_PER_MILLISECOND 1000

#define SLOT_UNITIALIZED 0
#define SLOT_WAITING_TO_CONNECT 1
#define SLOT_CONNECTED 2
#define SLOT_ERRORED 3

uint8_t mksml_initialize_internal(uint8_t platform_index, uint16_t frame_interval_ms, char hostnames[MKS_MAX_FEDERATES][MKS_MAX_ADDRESS_LENGTH], uint16_t* ports, uint8_t nos_hosts);

void* main_thread_function(void* ptr);

void handle_external_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr);
void handle_internal_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr);

bool read_next_line_int(FILE * fp, int* value);
bool read_next_line_string(FILE * fp, char* text);

pthread_t __thread main_thread;
bool __thread is_running = false;

struct ThreadInfoBlock __thread threadInfoBlock;

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

uint8_t mksml_initialize_internal(uint8_t platform_index, uint16_t frame_interval_ms, char hostnames[MKS_MAX_FEDERATES][MKS_MAX_ADDRESS_LENGTH], uint16_t* ports, uint8_t nos_hosts) {
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

        threadInfoBlock.federates[federate_index] = SLOT_WAITING_TO_CONNECT;

        threadInfoBlock.sockets[federate_index] = socket(AF_INET, SOCK_DGRAM, 0);  

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(ports[federate_index]);
        inet_aton(hostnames[federate_index], (struct in_addr*)&addr.sin_addr.s_addr);

        if(bind(threadInfoBlock.sockets[federate_index], (struct sockaddr*)&addr, sizeof(addr)) == 0) {
            threadInfoBlock.federates[federate_index] = SLOT_CONNECTED;
        }
        else {
            threadInfoBlock.federates[federate_index] = SLOT_ERRORED; 

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

void* main_thread_function(void* ptr) {
    struct ThreadInfoBlock* threadInfoBlock_ptr = ptr;
 
    useconds_t sleep_interval_max = threadInfoBlock_ptr->frame_interval_ms * MICROSECONDS_PER_MILLISECOND;

    while(!threadInfoBlock_ptr->quit_flag) {
        for(uint8_t federate_index = 0; federate_index < threadInfoBlock.nos_hosts; federate_index++) {
            if(threadInfoBlock.federates[federate_index] == SLOT_ERRORED) {
                continue;
            }

            if(federate_index != threadInfoBlock_ptr->platform_index) {
                handle_external_frame(federate_index, threadInfoBlock_ptr);
            } else {
                handle_internal_frame(federate_index, threadInfoBlock_ptr);
            } 
        } 

        if(sleep_interval_max > 0) { 
            usleep(sleep_interval_max);
        } 
    } 

    return (void*)NULL;
}

void handle_external_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr) {
} 

void handle_internal_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr) {
} 

bool read_next_line_int(FILE * fp, int* value) {
    char * line = NULL;
    size_t len = 0;

    if(fp == NULL) return false;

    ssize_t read = getline(&line, &len, fp);
    if(read == -1) return false;
    *value = atoi(line);
    
    free(line);

    return true;
}

bool read_next_line_string(FILE * fp, char* text) {
    char * line = NULL;
    size_t len = 0;

    if(fp == NULL) return false;

    ssize_t read = getline(&line, &len, fp);
    if(read == -1 || read >= MKS_MAX_ADDRESS_LENGTH) return false;
    strncpy(text, line, strlen(line));
    
    free(line);

    return true;
}
