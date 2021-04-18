PROG=simple_example
SRCS=simple_example.c
INCS=
MAN=

CFLAGS+= -std=c11 -Wall -ansi -pedantic -I${.CURDIR}
CFLAGS+= -g -I/usr/include -I/usr/local/include -I../../include
LDFLAGS+= -static -L/usr/lib -L/usr/local/lib -L../../ -lc -lmksmemoryloop -lpthread

.include <bsd.prog.mk>