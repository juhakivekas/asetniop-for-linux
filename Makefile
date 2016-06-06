CC=gcc
CFLAGS=-Wall

libevdev=./libevdev/
libevdev_obj=./libevdev/libevdev/libevdev.o ./libevdev/libevdev/libevdev-uinput.o

asetniop:src/asetniop.c
	$(CC) $(CFLAGS) $^ -o $@ $(libevdev_obj) -I $(libevdev)

documentaion: doc/*.gv
doc/%.svg:doc/%.gv
	dot $^ -o $@ -Tsvg
