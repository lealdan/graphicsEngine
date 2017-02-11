/*
* 030mainInterpolating.c
* Danny Leal 01/09/2017
*/
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "030vector.c"
#include "030matrix.c"
#include "030triangle.c"

double a[2] = {50.0, 260.0};
double b[2] = {320.0, 320.0};
double c[2] = {430.0, 480.0};
double rgb[3] = {0.4, 0.3, 0.5};
double alpha[3] = {1.0, 0.0, 1.0};
double beta[3] = {0.0, 1.0, 1.0};
double _gamma[3] = {1.0, 1.0, 0.0};

double a2[2] = {50.0, 10.0};
double b2[2] = {450.0, 80.0};
double c2[2] = {300.0, 240.0};
double rgb2[3] = {1.0, 1.0, 1.0};
double alpha2[3] = {0.0, 0.0, 1.0};
double beta2[3] = {1.0, 0.0, 0.0};
double _gamma2[3] = {0.0, 1.0, 0.0};
int top = 1;
int plus = 1;

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_N) {
		pixClearRGB(0.0, 0.0, 0.0);
	}
	if (key == GLFW_KEY_D) {
        rgb[0] = 1.0;
        rgb[1] = 1.0;
        rgb[2] = 1.0;
        vecCopy(3, rgb, rgb2);
        triRender(a, b, c, rgb, alpha, beta, _gamma);
        triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
	}
    if (key == GLFW_KEY_R && top) {
        if (plus) {
            rgb[0] += 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
        else {
            rgb[0] -= 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
    }
    if (key == GLFW_KEY_G && top) {
        if (plus) {
            rgb[1] += 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
        else {
            rgb[1] -= 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
    }
    if (key == GLFW_KEY_B && top) {
        if (plus) {
            rgb[2] += 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
        else {
            rgb[2] -= 0.1;
            triRender(a, b, c, rgb, alpha, beta, _gamma);
        }
    }
    if (key == GLFW_KEY_R && !top) {
        if (plus) {
            rgb2[0] += 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
        else {
            rgb2[0] -= 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
    }
    if (key == GLFW_KEY_G && !top) {
        if (plus) {
            rgb2[1] += 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
        else {
            rgb2[1] -= 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
    }
    if (key == GLFW_KEY_B && !top) {
        if (plus) {
            rgb2[2] += 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
        else {
            rgb2[2] -= 0.1;
            triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        }
    }
    if (key == GLFW_KEY_1) {
        top = 1;
    }
    if (key == GLFW_KEY_2) {
        top = 0;
    }
    if (key == GLFW_KEY_S) {
        if (plus) {
            plus = 0;
        }
        else {
            plus = 1;
        }
    }
}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(512, 512, "Triangle Interpolating") != 0)
		return 1;
	else {
        //Render a triangles
        triRender(a, b, c, rgb, alpha, beta, _gamma);
        triRender(c2, a2, b2, rgb2, alpha2, beta2, _gamma2);
        pixSetKeyDownHandler(handleKeyDown);
        //Clear the window to black

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		return 0;
	}
}
