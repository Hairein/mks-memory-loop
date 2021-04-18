// mksmemoryloop.h
// Author: Micah Koleoso Software, 2021

#ifndef MKS_MKSMEMORYLOOP_H
#define MKS_MKSMEMORYLOOP_H

#include <stdbool.h>

#define MAX_FEDERATES 16
#define FRAME_SIZE 508
#define UPDATEINTERVAL_MS 200

extern bool mksml_initialize();
extern void mksml_uninitialize();

#endif
