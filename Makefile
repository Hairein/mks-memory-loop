LIB= libmksmemoryloop

SHLIB_MAJOR= 0
SHLIB_MINOR= 0
SHLIB_TEENY= 1

SRCS= src/mksmemoryloop.c src/main_thread.c src/file_read.c
INCS= include/mksmemoryloop.h include/mksstructures.h include/main_thread.h include/file_read.h
OBJS= mksmemoryloop.o main_thread.o file_read.o

OS:= uname -o

CFLAGS+= -std=c11 -Wall -pedantic -fPIC
CFLAGS+= -g 
CFLAGS+= -I${.CURDIR}/include -I./include -I/usr/include -I/usr/local/include

LDFLAGS+= -L/usr/lib -L/usr/local/lib -lpthread

%.o : src/%.c ${INCS} ${SRCS}
	gcc -c ${CFLAGS} $< -o $@ ${LDFLAGS}

${LIB}.a : ${OBJS}
	ar rcs ${LIB}.a ${OBJS}
	g++ -shared -o ${LIB}.so ${OBJS}

#freebsd:
#.include <bsd.lib.mk>

clean:
	rm -f ${LIB}
	rm -f *.o
	rm -f *.a
	rm -f *.so

install:
	cp libmksmemoryloop.a /usr/local/lib
	cp libmksmemoryloop.so /usr/local/lib
	cp ./include/mksmemoryloop.h /usr/local/include
	ldconfig

uninstall:
	rm /usr/local/lib/libmksmemoryloop.a
	rm /usr/local/lib/libmksmemoryloop.so
	rm /usr/local/include/mksmemoryloop.h 
	ldconfig
	
os_info:
	@ $(OS)