PROG=sfml_example
SRCS=sfml_example.cpp
INCS=
MAN=

CFLAGS+= -std=c++11 -ansi -Wall -g -O0
CFLAGS+= -I. -I/usr/include -I/usr/local/include
LDFLAGS+= -L/usr/lib -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lmksmemoryloop -lstdc++ -lm

${PROG} : ${INCS} ${SRCS}
	c++ ${CFLAGS} ${SRCS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o


