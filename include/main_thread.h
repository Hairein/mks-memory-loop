// main_thread.h
// Author: Micah Koleoso Software, 2022

#ifndef MKS_MAIN_THREAD_H
#define MKS_MAIN_THREAD_H

#include <sys/select.h>

void* main_thread_function(void* ptr);

void perform_read(fd_set* read_set, fd_set* except_set, struct ThreadInfoBlock* threadInfoBlock_ptr);
void perform_write(fd_set* write_set, fd_set* except_set, struct ThreadInfoBlock* threadInfoBlock_ptr);

#endif
