PROG=simple_example
SRCS=simple_example.c
INCS=
MAN=

CFLAGS+= -std=c11 -Wall -pedantic -g
CFLAGS+= -I. -I/usr/include -I/usr/local/include -I../../include
LDFLAGS+= -static -L../.. -L/usr/lib -L/usr/local/lib  -l:mksmemoryloop.a -lpthread

${PROG} : ${INCS} ${SRCS} ../../mksmemoryloop.a
	gcc ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


