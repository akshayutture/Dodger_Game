# Makefile
#

SH = bash
CFLAGS = -g
LDFLAGS = -lncurses

all: main

main: main.o create_destroy_windows.o start_end_screen.o
	gcc  $^ -o $@ $(LDFLAGS)

create_destroy_windows.o: create_destroy_windows.c create_destroy_windows.h
	gcc -c $(CFLAGS) $< -o $@

start_end_screen.o: start_end_screen.c start_end_screen.h
	gcc -c $(CFLAGS) $< -o $@

main.o: main.c start_end_screen.h create_destroy_windows.h
	gcc -c $(CFLAGS) $< -o $@

	
.PHONY: clean

clean:
	rm -rf *.o
	rm -rf main

