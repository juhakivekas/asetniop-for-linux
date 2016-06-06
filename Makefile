CC=gcc
CFLAGS=-Wall

libevdev_flags=`pkg-config --libs --cflags libevdev`

asetniop:src/asetniop.c
	$(CC) $(CFLAGS) $^ -o $@ $(libevdev_flags)

#handle the graphviz graphs
graphviz=$(wildcard doc/*.gv)
graph=$(patsubst doc/%.gv, doc/%.svg, $(graphviz))

documentation: $(graph)

doc/%.svg:doc/%.gv
	dot $^ -o $@ -Tsvg

clean:
	rm asetniop
#	rm doc/*.svg
