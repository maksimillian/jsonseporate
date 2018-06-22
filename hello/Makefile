CC      = clang
CFLAGS  = -std=c11 -Wall -g
LDFLAGS = # -lm

default: hello

clean:
	rm -f $(patsubst %.c,%,$(wildcard *.c))
