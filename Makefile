CFLAGS = -std=c11 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
INCLUDES=$(wildcard *.h)

main: $(OBJS)

$(OBJS): $(INCLUDES)

test: main
	./test.sh

clean:
	rm -f main *.o *~ tmp*

.PHONY: test clean
