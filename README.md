Asetniop for Linux
------------------

[Asetniop](http://asetniop.com/) is an alternative input method targeted for devices with touchscreen interfaces.
I don't own a tablet, but I like the idea quite a lot even on a keyboard.
This is an implementation-in-the-making to bring the asetniop keyboard to Linux on a system-wide level.

For info on the workflow, architecture, design choises, etc. check out the files in `doc/`.
Any contributions, issues, and discussion is welcome! Raise issues or shoot me a mail.

Building
--------

At the moment [libevdev](https://www.freedesktop.org/wiki/Software/libevdev/) needs to be compiled in the root of the repo.
Clone it, build it, and make changes to the Makefile if needed.
After this compiling the asetniop userland hijacker should be as easy as this:

	make asetniop

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

What I do is i run my gdb in a separate terminal so that I can kill it with my mouse by closing the window.
That kills the program and returns the keyboard back to the system.
Another way to do this would be to have two separete keyboards, the other one for testing, the other one for actual usage.
The main point here is that you should have a way to kill the intercepting process when your keyboard vanishes.

