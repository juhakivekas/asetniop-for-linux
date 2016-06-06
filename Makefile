CC=gcc
CFLAGS=-Wall

libevdev_flags=`pkg-config --libs --cflags libevdev`

#obtained by 'pkg-config --libs --cflags libevdev'
#libevdev_flags=-I/usr/include/libevdev-1.0/ -levdev

asetniop:src/asetniop.c
	$(CC) $(CFLAGS) $^ -o $@ $(libevdev_flags)
#$(CC) $(CFLAGS) $^ -o $@ $(libevdev_obj) -I $(libevdev)

documentaion: doc/*.gv
doc/%.svg:doc/%.gv
	dot $^ -o $@ -Tsvg

clean:
	rm asetniop
