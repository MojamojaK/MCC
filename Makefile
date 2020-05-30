SOURCEDIR = src
INCLUDEDIR = includes
BUILDDIR = build

CC = cc
CFLAGS = -std=c11 -g -static -I$(INCLUDEDIR) -Wall

BIN=main
SRCS=$(wildcard $(SOURCEDIR)/*.c)
OBJS=$(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
INCLUDES=$(wildcard $(INCLUDEDIR)/*.h)

all: dir $(BUILDDIR)/$(BIN)

dir:
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(BIN): $(OBJS)
	$(CC) $^ -o $@

$(OBJS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BUILDDIR)/$(BIN)
	./test.sh $(CC) $<

clean:
	rm -rf build

.PHONY: test clean dir
