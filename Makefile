CC=gcc
CFLAGS=-Wall


#======== EXECUTABLES ========
libevdev_flags=`pkg-config --libs --cflags libevdev`
asetniop:src/asetniop.c
	$(CC) $(CFLAGS) $^ -o $@ $(libevdev_flags)

#======== TESTS ==============

libcunit_flags=-lcunit
test_sources=$(wildcard test/*.c)
test_executables=$(patsubst test/%.c, test/%, $(test_sources))

test:test/chord_test

test/%_test:test/%_test.c src/%.c
	$(CC) $(CFLAGS) $^ -o $@ $(libcunit_flags)

#======== DOCUMENTATION ======
graphviz=$(wildcard doc/*.gv)
graph=$(patsubst doc/%.gv, doc/%.svg, $(graphviz))

documentation: $(graph)

doc/%.svg:doc/%.gv
	dot $^ -o $@ -Tsvg



clean:
	rm asetniop
	rm test/*_test
#	rm doc/*.svg
