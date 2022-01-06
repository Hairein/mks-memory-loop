// mksstructures.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSSTRUCTURES_H
#define MKS_MKSSTRUCTURES_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mksmemoryloop.h"

struct ThreadInfoBlock {
    bool quit_flag;
    uint8_t platform_index;
    uint16_t frame_interval_ms;

    uint8_t nos_hosts;
    struct in_addr* hosts[MKS_MAX_FEDERATES];

    uint8_t federates[MKS_MAX_FEDERATES];
    int sockets[MKS_MAX_FEDERATES];
    uint8_t frames[MKS_MAX_FEDERATES][MKS_MAX_FRAME_SIZE];  
};

#endif
