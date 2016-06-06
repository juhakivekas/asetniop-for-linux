CC=gcc
CFLAGS=-Wall -g


#======== EXECUTABLES ========
libevdev_flags=`pkg-config --libs --cflags libevdev`

asetniop_sources=$(wildcard src/*.c)
asetniop_objects=$(patsubst src/%.c, src/%.o, $(asetniop_sources))

asetniop:$(asetniop_objects)
	$(CC) $(CFLAGS) $^ -o $@ $(libevdev_flags)

src/%.o:src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@ $(libevdev_flags)

#======== TESTS ==============

libcunit_flags=-lcunit
test_sources=$(wildcard test/*.c)
test_executables=$(patsubst test/%.c, test/%, $(test_sources))

test:test/chord_test
	test/chord_test

test/%_test:test/%_test.c src/%.c
	$(CC) $(CFLAGS) $^ -o $@ $(libcunit_flags)

#======== DOCUMENTATION ======
graphviz=$(wildcard doc/*.gv)
graph=$(patsubst doc/%.gv, doc/%.svg, $(graphviz))

documentation: $(graph)

doc/%.svg:doc/%.gv
	dot $^ -o $@ -Tsvg



clean:
	rm -f asetniop
	rm -f test/*_test
	rm -f src/*.o
#	rm doc/*.svg
