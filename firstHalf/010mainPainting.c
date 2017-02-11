static int off = 0;
static int on = 1;

static int *mouseClick;
static int *red;
static int *green;
static int *blue;
static int *cyan;
static int *magenta;
static int *yellow;
static int *black;
static int *white;

/* In this tutorial our compiler invocation becomes more complicated. On macOS,
we compile with...
    clang 000linking.c 000pixel.o -lglfw -framework OpenGL
This invocation causes our compiled code to be 'linked' with three other pieces
of software, that have been compiled elsewhere: OpenGL, GLFW, and pixel.o.
OpenGL is a cross-platform, standardized library for interacting with graphics
hardware. It comes pre-installed on macOS and Linux, and is easily installed on
Windows. GLFW is a cross-platform, minimalist user interface toolkit, that we
can use to build simple OpenGL applications. Often it is not pre-installed, but
it is easy to install. 000pixel.o is a library that I have written, to provide
a simple foundation for this graphics course. You must have 000pixel.o and
000pixel.h in the same directory as 000linking.c, for the linking to work. */

/* Notice that we include 000pixel.h. The file name is enclosed in quotation
marks rather than angle brackets, to tell the C compiler to search for the file
in the working directory. Feel free to read 000pixel.h. It documents all of the
public functionality of the library 000pixel.o. */
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_N) {
		pixClearRGB(0.0, 0.0, 0.0);
	}
	if (key == GLFW_KEY_R) {
		red = &on;
		green = &off;
		blue = &off;
		cyan = &off;
		magenta = &off;
		yellow = &off;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_G) {
		red = &off;
		green = &on;
		blue = &off;
		cyan = &off;
		magenta = &off;
		yellow = &off;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_B) {
		red = &off;
		green = &off;
		blue = &on;
		cyan = &off;
		magenta = &off;
		yellow = &off;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_C) {
		red = &off;
		green = &off;
		blue = &off;
		cyan = &on;
		magenta = &off;
		yellow = &off;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_M) {
		red = &off;
		green = &off;
		blue = &off;
		cyan = &off;
		magenta = &on;
		yellow = &off;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_Y) {
		red = &off;
		green = &off;
		blue = &off;
		cyan = &off;
		magenta = &off;
		yellow = &on;
		black = &off;
		white = &off;
	}
	if (key == GLFW_KEY_K) {
		red = &off;
		green = &off;
		blue = &off;
		cyan = &off;
		magenta = &off;
		yellow = &off;
		black = &on;
		white = &off;
	}
	if (key == GLFW_KEY_W) {
		red = &off;
		green = &off;
		blue = &off;
		cyan = &off;
		magenta = &off;
		yellow = &off;
		black = &off;
		white = &on;
	}
}

/* Similarly, the following callbacks handle some mouse interactions. */
void handleMouseUp(int button, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		mouseClick = &off;
	}
}

void handleMouseDown(int button, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		mouseClick = &on;
	}
}

void handleMouseMove(double x, double y) {
	if (*mouseClick == 1) {
		if (*red == on) {
			pixSetRGB(x, y, 1.0, 0.0, 0.0);
			pixSetRGB(x + 1, y, 1.0, 0.0, 0.0);
			pixSetRGB(x, y + 1, 1.0, 0.0, 0.0);
			pixSetRGB(x + 1, y + 1, 1.0, 0.0, 0.0);
		}
		else if (*green == on) {
			pixSetRGB(x, y, 0.0, 1.0, 0.0);
			pixSetRGB(x + 1, y, 0.0, 1.0, 0.0);
			pixSetRGB(x, y + 1, 0.0, 1.0, 0.0);
			pixSetRGB(x + 1, y + 1, 0.0, 1.0, 0.0);		}
		else if (*blue == on) {
			pixSetRGB(x, y, 0.0, 0.0, 1.0);
			pixSetRGB(x + 1, y, 0.0, 0.0, 1.0);
			pixSetRGB(x, y + 1, 0.0, 0.0, 1.0);
			pixSetRGB(x + 1, y + 1, 0.0, 0.0, 1.0);		}
		else if (*cyan == on) {
			pixSetRGB(x, y, 0.0, 1.0, 1.0);
			pixSetRGB(x + 1, y, 0.0, 1.0, 1.0);
			pixSetRGB(x, y + 1, 0.0, 1.0, 1.0);
			pixSetRGB(x + 1, y + 1, 0.0, 1.0, 1.0);		}
		else if (*magenta == on) {
			pixSetRGB(x, y, 1.0, 0.0, 1.0);
			pixSetRGB(x + 1, y, 1.0, 0.0, 1.0);
			pixSetRGB(x, y + 1, 1.0, 0.0, 1.0);
			pixSetRGB(x + 1, y + 1, 1.0, 0.0, 1.0);		}
		else if (*yellow == on) {
			pixSetRGB(x, y, 1.0, 1.0, 0.0);
			pixSetRGB(x + 1, y, 1.0, 1.0, 0.0);
			pixSetRGB(x, y + 1, 1.0, 1.0, 0.0);
			pixSetRGB(x + 1, y + 1, 1.0, 1.0, 0.0);		}
		else if (*black == on) {
			pixSetRGB(x, y, 0.0, 0.0, 0.0);
			pixSetRGB(x + 1, y, 0.0, 0.0, 0.0);
			pixSetRGB(x, y + 1, 0.0, 0.0, 0.0);
			pixSetRGB(x + 1, y + 1, 0.0, 0.0, 0.0);		}
		else if (*white == on) {
			pixSetRGB(x, y, 1.0, 1.0, 1.0);
			pixSetRGB(x + 1, y, 1.0, 1.0, 1.0);
			pixSetRGB(x, y + 1, 1.0, 1.0, 1.0);
			pixSetRGB(x + 1, y + 1, 1.0, 1.0, 1.0);		}
		else {
			pixSetRGB(x, y, 1.0, 1.0, 1.0);
			pixSetRGB(x + 1, y, 1.0, 1.0, 1.0);
			pixSetRGB(x, y + 1, 1.0, 1.0, 1.0);
			pixSetRGB(x + 1, y + 1, 1.0, 1.0, 1.0);		}
	}
}

int main(void) {
	mouseClick = &off;
	red = &off;
	green = &off;
	blue = &off;
	cyan = &off;
	magenta = &off;
	yellow = &off;
	black = &off;
	white = &off;

	/* Make a 512 x 512 window with the title 'Pixel Graphics'. This function
	returns 0 if no error occurred. */
	if (pixInitialize(512, 512, "MS Paint") != 0)
		return 1;
	else {
		/* Register the callbacks (defined above) with the user interface, so
		that they are called as needed during pixRun (invoked below). */
		pixSetMouseDownHandler(handleMouseDown);
		pixSetKeyDownHandler(handleKeyDown);
		pixSetMouseUpHandler(handleMouseUp);
		pixSetMouseMoveHandler(handleMouseMove);
		/* Clear the window to white. */
		pixClearRGB(0.0, 0.0, 0.0);
		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		return 0;
	}
}
