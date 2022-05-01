PROG=simple_example
SRCS=simple_example.c
INCS=
MAN=

CFLAGS+= -std=c11 -Wall -pedantic -g
CFLAGS+= -I. -I/usr/include -I/usr/local/include
LDFLAGS+= -L/usr/lib -L/usr/local/lib -lmksmemoryloop -lpthread

${PROG} : ${INCS} ${SRCS} ../../libmksmemoryloop.a
	gcc ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


