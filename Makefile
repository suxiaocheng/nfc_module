INCPATH=
LIBSPATH=
LIBS=-lpthread -lmicrohttpd -lwiringPi
CC=gcc
CFLAGS+=-g #-D__RASPBERRY_PI__

SRC:=$(wildcard *.c)
OBJ := $(patsubst %.c,%.o,${SRC})

nfc_module:$(OBJ)
	$(CC) $^ -o $@ $(LIBSPATH) $(LIBS) $(INCPATH)

%.d:%.c
	${CC} -MM $< > $@ >$@.tmp; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@; \
	rm -f $@.tmp

include $(SRC:.c=.d)


.PHONY: clean
clean:
	rm -rf *.o ${OBJ} %.d nfc_module *.tmp
