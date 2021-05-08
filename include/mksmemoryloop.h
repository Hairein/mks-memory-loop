// mksmemoryloop.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSMEMORYLOOP_H
#define MKS_MKSMEMORYLOOP_H

#include <stdbool.h>
#include <stdint.h>

#define MKS_MAX_FEDERATES 16
#define MKS_MAX_FRAME_SIZE 508

#define MKS_NO_ERROR 0
#define MKS_ERROR_TOO_MANY_HOSTS 1

extern uint8_t mksml_initialize(uint8_t platform_index, uint16_t frame_interval_ms, char** hostnames, uint16_t* ports, uint8_t nos_hosts);
extern void mksml_uninitialize();

#endif
