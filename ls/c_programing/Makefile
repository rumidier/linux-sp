CC = gcc

PROGRAMS = ls

all: $(PROGRAMS)

ls: ls-main.c ls-control.c
	$(CC) -Wall -o $@ $^

clean:
	rm -rf $(PROGRAMS)
