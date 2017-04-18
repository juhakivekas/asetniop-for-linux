ASETNIOP for Linux
------------------

[ASETNIOP](http://asetniop.com/) is an alternative input method targeted for devices with touchscreen interfaces.
I don't own a tablet, but I like the idea quite a lot even on a keyboard.
This is a basic implementation of the ASETNIOP keyboard to Linux on a system-wide level.
Only the english language letters are supported at this point.

Currently the program steals the keyboard from all clients (including the X server) and filters the kystrokes you make.
By default all keystrokes are passed throught the filter unmodified.
The `a`, `s`, `d`, `f`, `j`, `k`, `l`, and `;` keys (in a US layout) are not passed through but are instead aggregated into chords that make up the ASETNIOP typing system.
This means that you can use ASETNIOP and still have most of the keyboard unaltered, such as `ctrl-c` to kill the program.

For info on the workflow, architecture, design choices, etc. check out the files in `doc`.
Any contributions, issues, and discussion is welcome! Raise github issues or shoot me a mail on guth.smash@gmail.com.

Building
--------

Install [libevdev](https://www.freedesktop.org/wiki/Software/libevdev/) development files.

	apt-get install libevdev-dev

After this compiling the asetniop userland hijacker should be as easy as this:

	make asetniop

There are probably some packages that are needed to have all the building tools.
I'll add these whenever I make a test on a clean installation of debian.

Running
-------

Run the asetniop executable with a keyboard device as argument.
Knowing which one is the right device is not ovbious, but often the names in `/dev/input/by-path/` are descriptive enough.
For me the keyboard turns up as `/dev/input/event0` and `/dev/input/by-path/platform-i8042-serio-0-event-kbd`.
You can find out by doing `cat <device>` and typing something, if garbage turns up on the screen, that device is your keyboard.
Now just do:

	./asetniop <your keyboard device>

You might need to have super user priviledges:

	sudo ./asetniop <your keyboard device>

Debugging
---------

Obviously debugging something that is altering your keystrokes will be risky and might force you to reboot.
No damage will be permanent, but since we are stealing some resources from the system it is a good idea to have a plan for when your keyboard stops working.

What I do is i asetniop in a separate terminal so that I can kill it with my mouse by closing the window.
Killing the program and returns the keyboard back to the system.
Another way to do this would be to have two separate keyboards, the other one for testing, the other one for actual usage.
The main point here is that you should have a way to kill the intercepting process when your keyboard vanishes.

