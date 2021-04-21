PROG = f 
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lprocps
SRC = main.c info.c

${PROG}: ${SRC}
	${CC} ${CFLAGS} ${LDFLAGS} ${SRC} -o ${PROG}

install:
	mkdir -p $(DESTDIR)/usr/bin
	install -m 0755 ${PROG} $(DESTDIR)/usr/bin/${PROG}

uninstall:
	rm -f $(DESTDIR)/usr/bin/${PROG}

clean:
	rm -f ${PROG}
