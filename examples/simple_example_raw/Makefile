PROG=simple_example_raw
SRCS=simple_example_raw.c
INCS=
MAN=

CFLAGS+= -std=c11 -Wall -g
CFLAGS+= -I. -I/usr/include
LDFLAGS+= -L/usr/lib -lmksmemoryloop -lpthread

${PROG} : ${INCS} ${SRCS}
	cc ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


