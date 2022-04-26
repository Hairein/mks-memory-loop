PROG=simple_example_raw
SRCS=simple_example_raw.c
INCS=
MAN=

CFLAGS+= -std=c11 -Wall -pedantic -g
CFLAGS+= -I. -I/usr/include -I/usr/local/include -I../../include
LDFLAGS+= -static -L../.. -L/usr/lib -L/usr/local/lib  -lmksmemoryloop -lpthread

${PROG} : ${INCS} ${SRCS} ../../libmksmemoryloop.a
	gcc ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


