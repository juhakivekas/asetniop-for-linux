#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>

#include "libevdev/libevdev.h"
#include "libevdev/libevdev-uinput.h"

static int forward_event(struct libevdev_uinput *uidev, struct input_event *ev)
{
	int err;
	printf("alter (%s, %s, %d)",
		libevdev_event_type_get_name(ev->type),
		libevdev_event_code_get_name(ev->type, ev->code),
		ev->value);

	switch(ev->code){
	case KEY_A:         ev->code = KEY_A; break;
	case KEY_S:         ev->code = KEY_S; break;
	case KEY_D:         ev->code = KEY_E; break;
	case KEY_F:         ev->code = KEY_T; break;
	case KEY_J:         ev->code = KEY_N; break;
	case KEY_K:         ev->code = KEY_I; break;
	case KEY_L:         ev->code = KEY_O; break;
	case KEY_SEMICOLON: ev->code = KEY_P; break;
	}
	
	printf(" to (%s, %s, %d)\n",
		libevdev_event_type_get_name(ev->type),
		libevdev_event_code_get_name(ev->type, ev->code),
		ev->value);

	err = libevdev_uinput_write_event(uidev, EV_KEY, ev->code  , ev->value);
	if (err < 0) {perror("Failed to write event"); return -1;}
	err = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
	if (err < 0) {perror("Failed to write event"); return -1;}
	return 0;
}

int main(int argc, char **argv)
{
	struct libevdev *dev = NULL;
	const char *file;
	int fd;
	int err = 1;

	if (argc < 2){
		fprintf(stderr, "Usage: %s <event device>\n", argv[0]);
		return 1;
	}

	//-------- open the keyboard device
	file = argv[1];
	fd = open(file, O_RDONLY);
	if (fd < 0) {perror("Failed to open device");goto out;}

	err = libevdev_new_from_fd(fd, &dev);
	if (err < 0) {perror("Failed to create libevdev device"); goto out;}
	err = libevdev_grab(dev, LIBEVDEV_GRAB);
	printf("Input device ID: bus %#x vendor %#x product %#x\n",
			libevdev_get_id_bustype(dev),
			libevdev_get_id_vendor(dev),
			libevdev_get_id_product(dev));
	printf("Evdev version: %x\n", libevdev_get_driver_version(dev));
	printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
	printf("Phys location: %s\n", libevdev_get_phys(dev));
	printf("Uniq identifier: %s\n", libevdev_get_uniq(dev));

	//------- create a uinput device based on the openend device
	struct libevdev_uinput *uidev;
	
	err = libevdev_uinput_create_from_device(
		dev,
		LIBEVDEV_UINPUT_OPEN_MANAGED,
		&uidev);

	if (err < 0) {perror("Failed to create uinput device"); goto out;}

	
	do {
		struct input_event ev;
		err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL|LIBEVDEV_READ_FLAG_BLOCKING, &ev);
		if (err == LIBEVDEV_READ_STATUS_SYNC) {
			//the status was dropped, re-sync
			printf("dropped, syncing status\n");
			while (err == LIBEVDEV_READ_STATUS_SYNC) {
				err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_SYNC, &ev);
			}
		} else if (err == LIBEVDEV_READ_STATUS_SUCCESS)
			if(ev.type == EV_KEY && ev.code == KEY_ESC){
				//quit the program in a nice way
				err = libevdev_grab(dev, LIBEVDEV_UNGRAB);
				exit(0);
			}
			if(ev.type == EV_KEY) forward_event(uidev, &ev);
	} while (err == LIBEVDEV_READ_STATUS_SYNC || err == LIBEVDEV_READ_STATUS_SUCCESS || err == -EAGAIN);

	if (err != LIBEVDEV_READ_STATUS_SUCCESS && err != -EAGAIN)
		fprintf(stderr, "Failed to handle events: %s\n", strerror(-err));

	err = 0;
	out:
	libevdev_free(dev);

	return err;
}
