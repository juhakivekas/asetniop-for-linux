//some of these are probably not necessary!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "chord.h"
#include "lookup.h"

static int forward_event(struct libevdev_uinput *uidev, struct input_event *ev, struct chord* state)
{
	int ret;
	enum touch t = NUM_TOUCHES;
	switch(ev->code){
	case KEY_A:         t = TOUCH_A; break;
	case KEY_S:       	t = TOUCH_S; break;
	case KEY_D:         t = TOUCH_E; break;
	case KEY_F:         t = TOUCH_T; break;
	case KEY_J:         t = TOUCH_N; break;
	case KEY_K:         t = TOUCH_I; break;
	case KEY_L:         t = TOUCH_O; break;
	case KEY_SEMICOLON: t = TOUCH_P; break;
	}
	if(t < NUM_TOUCHES){
		//fprintf(stderr, "current chord: 0x%02x\n", chord_state_bitmap(state));
		switch(ev->value){
		case 1: chord_touch_start(state, t); break;
		case 0: chord_touch_end(state, t);   break;
		}
		if(chord_state_is_empty(state)){
			lookup_key k = chord_accumulator_bitmap(state);
			int keycode = lookup_keycode(k);
			if(keycode != KEY_RESERVED){
				fprintf(stderr, "chord 0x%02x %s\n", k, libevdev_event_code_get_name(EV_KEY, keycode));
				ret = libevdev_uinput_write_event(uidev, EV_KEY, keycode   , 1);
				if (ret < 0) {perror("Failed to write event"); return -1;}
				ret = libevdev_uinput_write_event(uidev, EV_KEY, keycode   , 0);
				if (ret < 0) {perror("Failed to write event"); return -1;}
				ret = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
				if (ret < 0) {perror("Failed to write event"); return -1;}
			}
			chord_reset(state);
		}
	} else {
//		printf("%s, %s, %d\n",
//			libevdev_event_type_get_name(ev->type),
//			libevdev_event_code_get_name(ev->type, ev->code),
//			ev->value);

		ret = libevdev_uinput_write_event(uidev, EV_KEY, ev->code  , ev->value);
		if (ret < 0) {perror("Failed to write event"); return -1;}
		ret = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
		if (ret < 0) {perror("Failed to write event"); return -1;}
	}
	return 0;
}

int intercept(struct libevdev_uinput *uidev, struct libevdev *dev){
	int ret;
	struct input_event ev;
	struct chord state;
	chord_reset(&state);
	lookup_init("dat/test-keymap-minimal.dat");

	do {
		ret = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL|LIBEVDEV_READ_FLAG_BLOCKING, &ev);
		if (ret == LIBEVDEV_READ_STATUS_SUCCESS){
			//kill the interceptor with the escape key!
			if(ev.type == EV_KEY && ev.code == KEY_ESC)
				return 0;
			//forward key down and key up events
			if(ev.type == EV_KEY && (ev.value == 1 || ev.value == 0))
				forward_event(uidev, &ev, &state);
		}
	} while (ret == LIBEVDEV_READ_STATUS_SYNC || ret == LIBEVDEV_READ_STATUS_SUCCESS || ret == -EAGAIN);

	if (ret != LIBEVDEV_READ_STATUS_SUCCESS && ret != -EAGAIN)
		fprintf(stderr, "Failed to handle events: %s\n", strerror(-ret));

	return 0;}

int main(int argc, char **argv)
{
	struct libevdev *dev = NULL;
	struct libevdev_uinput *uidev = NULL;
	int fd, ret;

	if (argc < 2){
		fprintf(stderr, "Usage: %s <event device>\n", argv[0]);
		return 1;
	}
	
	//open the event device file descriptor
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {perror("Failed to open device"); return fd;}

	//create a libevdev device from the event file descriptor
	ret = libevdev_new_from_fd(fd, &dev);
	if (ret < 0) {perror("Failed to create libevdev device"); return ret;}

	//create a device connected to the virtual device interface /dev/uinput
	ret = libevdev_uinput_create_from_device(
		dev,
		LIBEVDEV_UINPUT_OPEN_MANAGED,
		&uidev);
	if (ret < 0) {perror("Failed to create uinput device"); return ret;}

	sleep(1);
	//EVIOCGRAB the event, this means no other client will receive the events from the device, including your window manager.
	// "This is generally a bad idea. Don't do this." - libevdev documentation
	//We need to do this to avoid douplicate keyboard events.
	ret = libevdev_grab(dev, LIBEVDEV_GRAB);

	//start the interceptor and never return
	intercept(uidev, dev);

	libevdev_uinput_destroy(uidev);
	libevdev_grab(dev, LIBEVDEV_UNGRAB);
	libevdev_free(dev);
	return ret;
}
