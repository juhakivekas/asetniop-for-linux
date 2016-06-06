asetniop for Linux
------------------

[Asetniop](http://asetniop.com/) is an experimental and innovative input method. 
Originally designed for wearables and tablets, it's usefullness and ingenyity should not go unnoticed for desktop users.
It is possibly a good alternative for QWERTY and regular keyboards.
While usability of alternative user interfaces is hard to evaluate, developing them is a load of fun.

I want asetniop on my debian desktop.

I also want to learn about Linux input internals, the X server, and USB HID devices.
In addition to the things I want to learn, I'd like to encourage a development philosophy where as little code as possible is written.
This is why I'll be adding notions about the data flows, software architectures and general software design ideas as I go on.

Data flow
---------

In the applocation we want data to flow from single keys on the keyboard to text fields in our desktop appliaitons such as our browser or text editor.
There are many points at which we can interrupt and alter the flow of data so that asetniop can be implemented.
Here are some possible methods.

 - Build a USB keyboard device which does all asetniop processing.
 - Alter the firmware on an existing keyboard to do the processing.
 - Write a kernel driver that interprets and modifies normal keyboard input as asetniop.
 - Write an X window manager plugin/patch that will do the alteration/interpreting.
 - Make an appliccation that uses X events and emulates an asetniop keyboard.

All of these methods have pros and cons.
The first ones are very much generally applicable and OS independent.
If we make a physical asetniop keyboard it will work with all systems that can understand a USB keyboard, ie. all the systems.
This is labour intensive and requires us to have hardware to prototype with.
It is also hard to deploy on a large scale since we'd have to manufacture the hardware.

Using existing keyboards is a bit easier, since we might get away with minor hardware modifications and code.
We'd still be bound to a certain make and model of keyboards, so the solution is still not easily scaled.

Writing kernel drivers is a software-only solution.
This means anyone with enough skills to compile and load the code can use the tool.
But since we are detached from the hardware we now have to make assumptions on the keyboard being used.
There are way too many vendor and laguage specific layouts to support all of them that we'd have to stick with only one for the development.
In addition to this, since we are poking the kernel the tool is now specific to one operating system.

Implementing the asetniop keyboard in the X server will be slightly messy, since X already does some processing.
This means we are changing a large codebase and recompiling software without which many users could not use their computer.
This sounds risky.

Making an application-level keyboard would be quite simple since the tools for this are realdily available from eg. gaming frameworks.
This is a lot like what the original JavaScript asetniop keyboard does.
It's not a bad idea, on the contrary, browser applications are extremely portable and fast to develop.
The problem is that we do lose the generality of the approach when doing it on an applocation level.
This is because now we are bound to use the tool in that one applicaiton alone.

What if I'd like to write a document in any text editor with asetniop?
This is the problem that I want to solve.

The first hijack
----------------
04.06.16 23:33

I feel that hijacking the data flow is the right term for what I'll be doing.
The idea is that I take an existing data flow, redirect it ,alter it and feed it forward to where is was going in the first place.
Transparency is a nice idea to think about here, since it will lead to less bugs.
If we can do this between the kernel and the X server, so that neither needs modification, then we're all good to go.
In my first approach I  will not alter any software other than that which I am writing.
I'm going to create a new event device.
A day ago this wouldn't have meant anything to me, so now I'll explain it.

The Linux input data flow looks pretty much like this:

	keyboard (USB hardware)
	  |
	  v
	kernel/drivers
	  |
	  v
	evdev (/dev/input/eventNN)
	  |
	  v
	libevdev
	  |
	  v
	X-server
	  |
	  v
	X-client (applications; browser, terminal, office)

By default the X server listens to some set of devices:

	/dev/input/event0  /dev/input/event1  /dev/input/event2
	      |                  |                  |
		  +------------------+------------------+
	                         |
	                         v
	                     X-server

Let's say that event0 is a keyboard.
To implement the asetniop keyboard we want to grab the keypress events and do the chording logic.
After that logic is done we want to write some events to another event device.
We will be using the uinput device, which is meant for user-defined virtual events.
Now we'll have something like this:

	/dev/input/event0  /dev/input/event1  /dev/input/event2
	      |                  |                  |
	asetniop logic           |                  |
	      |                  |                  |
	 /dev/uinput             |                  |
	      |                  |                  |
		  +------------------+------------------+
	                         |
	                         v
	                     X-server

In Linux jargon, I'm going to do this:
I'll EVIOCGRAB the /den/input/eventNN device corresponding to the keyboard and use those events as input to implement the asetniop logic. 
The output of the asetniop keyboard will then be written to the virtual input device /dev/uinput.
By doing the hijack at this point I dont need to do anything keyboard specific other than assume/figure out a keyboard layout.

What to do with the other keys
------------------------------
05.06.16 10:02

Asetniop only needs ten keys, so there will be plenty to use still after the implementation.
There's a lot that we can do with the rest of the keys, but for the sake of sanity and usability I'll pass those key events through unaltered.
What we gain by doing this is that all the special functions that users might be using in their keyboard, such as media keys, arrow keys, ESC, etc. will still work.
This is important since it will enable desktop computer usability.
While asetniop is might be good for writing, the keyboard is used for many more things in a desktop environment.
It will also increase the familiarity of the keyboard layout, and thus help adapation.

There will be things that one wants to have other keys than alphanumerics for.
Reflexes like backspace and the arrow keys might be things that the user initially needs in order not to get too frustrated.
Things like special characters only appear seldomly, so typing those from the number row would be totally reasonable, especially for programmers who would otherwise have to memorize twenty more chords.
While ambitious and eager users might slowly move towards a pure asetniop typing style, the option of having the familiar keyboard elements available is a valid usability choice.

In general passing non-home-row keys through the driver gives more flexibility to the users, eases adaptation and doesn't interfere with regular use of the desktop keyboard.

I'm wiriting C
--------------

The bigger borther C++ has a lot of nice added functionality, sometimes a bit too much.
Since I'm working with the kernel and input devices I decided that C is the best language to use.
It compiles even to the most low end devices, which is good in case someone wants to use this implementation of the ASETNIOP state machine for something different, like a hardware device.
Preferably I'll be separating functionalities enough to make the code _actually_ reusable.
This includes compiling as C99 and excludes the use of malloc(3).

This might not be the case for all the code in this project, but at least those parts that work purely with the ASETNIOP logic.
