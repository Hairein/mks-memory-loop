// main_thread.h
// Author: Micah Koleoso Software, 2022

#ifndef MKS_MAIN_THREAD_H
#define MKS_MAIN_THREAD_H

#include <sys/select.h>

void* main_thread_function(void* ptr);

void perform_read(struct ThreadInfoBlock* threadInfoBlock_ptr);
void perform_write(struct ThreadInfoBlock* threadInfoBlock_ptr);

void clear_frames(struct ThreadInfoBlock* threadInfoBlock_ptr);
void clear_frame(uint8_t platform_index, struct ThreadInfoBlock* threadInfoBlock_ptr);

#endif
