CFLAGS = -std=c11 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
INCLUDES=main.h

main: $(OBJS)

$(OBJS): $(INCLUDES)

test: main
	./test.sh

clean:
	rm -f main *.o *~ tmp*

.PHONY: test clean
