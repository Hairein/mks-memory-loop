// mksstructures.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSSTRUCTURES_H
#define MKS_MKSSTRUCTURES_H

#include <stdbool.h>
#include <stdint.h>

struct ThreadInfoBlock {
    bool quit_flag;
    uint8_t platform_index;
    uint16_t frame_interval_ms;
};

#endif
