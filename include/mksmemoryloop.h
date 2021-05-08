// mksmemoryloop.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSMEMORYLOOP_H
#define MKS_MKSMEMORYLOOP_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_FEDERATES 16
#define FRAME_SIZE 508
#define UPDATEINTERVAL_MS 200

#define MICROSECONDS_PER_MILLISECOND 1000

extern bool mksml_initialize(uint8_t platform_index, uint16_t frame_interval_ms);
extern void mksml_uninitialize();

#endif
