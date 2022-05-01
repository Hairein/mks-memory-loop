LIB= libmksmemoryloop
HEADER= mksmemoryloop.h

SHLIB_MAJOR= 1
SHLIB_MINOR= 0
SHLIB_TEENY= 0

INCLUDE_INSTALL_DIR= /usr/local/include
LIB_INSTALL_DIR= /usr/local/lib

SRCS= src/mksmemoryloop.c src/main_thread.c src/file_read.c
INCS= include/mksmemoryloop.h include/mksstructures.h include/main_thread.h include/file_read.h
OBJS= mksmemoryloop.o main_thread.o file_read.o

OS:= uname -o

CFLAGS+= -std=c11 -Wall -pedantic -fPIC
CFLAGS+= -g 
CFLAGS+= -I${.CURDIR}/include -I./include -I/usr/include -I${INCLUDE_INSTALL_DIR}

LDFLAGS+= -L/usr/lib -L${LIB_INSTALL_DIR} -lpthread

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
	cp ./${LIB}.a ${LIB_INSTALL_DIR}
	cp ./${LIB}.so ${LIB_INSTALL_DIR}/${LIB}.so.${SHLIB_MAJOR}.${SHLIB_MINOR}.${SHLIB_TEENY}
	cp ./include/${HEADER} ${INCLUDE_INSTALL_DIR}
	ln -s ${LIB_INSTALL_DIR}/${LIB}.so.${SHLIB_MAJOR}.${SHLIB_MINOR}.${SHLIB_TEENY} ${LIB_INSTALL_DIR}/${LIB}.so
	ldconfig

uninstall:
	rm ${LIB_INSTALL_DIR}/${LIB}*
	rm ${INCLUDE_INSTALL_DIR}/${HEADER} 
	ldconfig
	
os_info:
	@ $(OS)