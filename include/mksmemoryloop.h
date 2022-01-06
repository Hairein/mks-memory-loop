// mksmemoryloop.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSMEMORYLOOP_H
#define MKS_MKSMEMORYLOOP_H

#include <stdbool.h>
#include <stdint.h>

#define MKS_MAX_FEDERATES 16
#define MKS_MAX_FRAME_SIZE 508
#define MKS_MAX_ADDRESS_LENGTH 32

#define MKS_NO_ERROR 0
#define MKS_ERROR_TOO_MANY_HOSTS 1
#define MKS_ERROR_READING_CFG_FILE 2
#define MKS_ERROR_BAD_CFG_PARAMETER 3

extern uint8_t mksml_initialize(char* cfg_filename);
extern void mksml_uninitialize();

#endif
