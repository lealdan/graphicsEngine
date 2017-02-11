/*
* 020mainRasterizing.c
* Danny Leal 01/08/2017
*/
#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "020triangle.c"

int main(void) {

    //Set values for vertices of multiple test triangles.
    double a0 = 100.0;
    double a1 = 180.0;
    double b0 = 140.0;
    double b1 = 160.0;
    double c0 = 120.0;
    double c1 = 220.0;

    double a10 = 230.0;
    double a11 = 170.0;
    double b10 = 270.0;
    double b11 = 170.0;
    double c10 = 270.0;
    double c11 = 220.0;

    double a20 = 360.0;
    double a21 = 170.0;
    double b20 = 410.0;
    double b21 = 190.0;
    double c20 = 380.0;
    double c21 = 220.0;

    double a30 = 100.0;
    double a31 = 370.0;
    double b30 = 160.0;
    double b31 = 350.0;
    double c30 = 110.0;
    double c31 = 410.0;

    double a40 = 350.0;
    double a41 = 370.0;
    double b40 = 380.0;
    double b41 = 380.0;
    double c40 = 400.0;
    double c41 = 420.0;

    double a50 = 200.0;
    double a51 = 390.0;
    double b50 = 250.0;
    double b51 = 350.0;
    double c50 = 250.0;
    double c51 = 390.0;

    //Initialize a draw window
    if (pixInitialize(512, 512, "Triangle Render") != 0)
		return 1;
	else {
        //Render 6 triangles
        triRender(a0, a1, b0, b1, c0, c1, 1.0, 1.0, 0.0);
        triRender(a10, a11, b10, b11, c10, c11, 0.0, 1.0, 1.0);
        triRender(a20, a21, b20, b21, c20, c21, 1.0, 0.0, 0.0);
        triRender(a30, a31, b30, b31, c30, c31, 1.0, 0.0, 1.0);
        triRender(a40, a41, b40, b41, c40, c41, 0.0, 0.9, 0.1);
        triRender(a50, a51, b50, b51, c50, c51, 0.8, 0.9, 1.0);

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		return 0;
	}
}
