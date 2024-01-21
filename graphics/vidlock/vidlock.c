#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <pthread.h>



#define CONFIG_FILE "config.txt"
#define VIDEO_COMMAND "ffplay -loop 0 -autoexit -i %s"


char video_path[256];

void *playVideo(void *arg) {
	char command[512];
	sprintf(command, VIDEO_COMMAND, video_path);
	system(command);
	pthread_exit(NULL);
}


int main() {
	// Read video path
	FILE *configFile = fopen(CONFIG_FILE, "r");
	if (configFile == NULL) {
		perror("Error opening config file");
		exit(EXIT_FAILURE);
	}

	if (fscanf(configFile, "%s", video_path) != 1) {
		perror("Error reading video path");
		fclose(configFile);
		exit(EXIT_FAILURE);
	}
	
	fclose(configFile);

	// X11 display init
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(EXIT_FAILURE);
	}

	Window root = DefaultRootWindow(display);

	// Create fullscreen context
	
	XWindowAttributes attr;
	XGetWindowAttributes(display, root, &attr);
	Window window = XCreateSimpleWindow(display, root, 0, 0, attr.width, attr.height, 0, 0, 0);

	XSelectInput(display, window, ExposureMask | KeyPressMask);
	XMapWindow(display, window);

	// Play video on other thread
	pthread_t videoThread;
	if (pthread_create(&videoThread, NULL, playVideo, NULL) != 0) {
		fprintf(stderr, "Error creating video thread\n");
		exit(EXIT_FAILURE);
	}


	// Events
	XEvent ev;
	KeySym key;
	char buffer[1];
	Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wm_delete_window, 1);


	while(1) {
		XNextEvent(display, &ev);

		if (ev.type == KeyPress) {
			XLookupString(&ev.xkey, buffer, sizeof(buffer), &key, 0);
			if (key == XK_Escape) {
				break;
			} else if (ev.type == ClientMessage) {
				if (ev.xclient.data.l[0] == wm_delete_window) {
					break;
				}
			}
		}

	// Clean

		pthread_cancel(videoThread);
		pthread_join(videoThread, NULL);
		XDestroyWindow(display, window);
		XCloseDisplay(display);

		return 0;
	
	}


}
