LIB= mksmemoryloop

SHLIB_MAJOR= 1
SHLIB_MINOR= 0
SHLIB_TEENY= 0

INCS= include/mksmemoryloop.h include/main_thread.h	include/file_read.h include/mksstructures.h
SRCS= src/mksmemoryloop.c src/main_thread.c src/file_read.c

CFLAGS+= -std=c++11 -I./include

.include <bsd.lib.mk>
