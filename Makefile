LIB= mksmemoryloop

SHLIB_MAJOR= 0
SHLIB_MINOR= 0
SHLIB_TEENY= 1

SRCS= src/mksmemoryloop.c 
INCS= include/mksmemoryloop.h include/mksstructures.h

CFLAGS+= -std=c11 -Wall -ansi -pedantic -I${.CURDIR}/include
CFLAGS+= -g -I/usr/include -I/usr/local/include -I${.CURDIR}/include/
LDFLAGS+= -L/usr/lib -L/usr/local/lib -lc -lpthread

.include <bsd.lib.mk>
