PROG=sfml_example
SRCS=sfml_example.cpp
INCS=
MAN=

CFLAGS+= -std=c++11 -Wall -pedantic -g
CFLAGS+= -I. -I/usr/include -I/usr/local/include
LDFLAGS+= -L/usr/lib -L/usr/local/lib -lmksmemoryloop -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lstdc++ -lm

${PROG} : ${INCS} ${SRCS}
	gcc ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


