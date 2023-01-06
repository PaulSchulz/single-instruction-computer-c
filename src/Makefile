.PHONY: all src clean

all: src

src: sic

sic: main.c
	gcc -Wall `pkg-config --cflags glib-2.0` -o $@ $< `pkg-config --libs glib-2.0`

clean:
	-rm sic
