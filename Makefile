LIB= mksmemoryloop

SHLIB_MAJOR= 0
SHLIB_MINOR= 0
SHLIB_TEENY= 1

SRCS= src/mksmemoryloop.c src/main_thread.c src/file_read.c
INCS= include/mksmemoryloop.h include/mksstructures.h include/main_thread.h include/file_read.h
OBJS= mksmemoryloop.o main_thread.o file_read.o

CFLAGS+= -std=c11 -Wall -pedantic -g 
CFLAGS+= -I./include -I/usr/include -I/usr/local/include

LDFLAGS+= -L/usr/lib -L/usr/local/lib -lpthread

%.o : src/%.c ${INCS} ${SRCS}
	gcc -c ${CFLAGS} $< -o $@ ${LDFLAGS}

${LIB}.a : ${OBJS}
	ar rcs ${LIB}.a ${OBJS}

clean:
	rm -f ${LIB}
	rm -f *.o
	rm -f *.a

