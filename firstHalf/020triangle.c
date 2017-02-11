/*
* 020triangle.c
* Danny Leal 01/08/2017
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>

/*
* A function that returns the y coordinate of a line for a given x.
* This function takes an x-coordinate, a known point on a line, and
* the slope for that line
*/
double getYOfLine(double x, double x0, double y0, double slope) {
    return slope * (x - x0) + y0;
}

/*
* A simple function that calculates the slope of a line given two points.
*/
double slopeOf(double x0, double y0, double x1, double y1) {
    return (y1 - y0)/(x1 - x0);
}

/*
* Renders a triangle given three vertices and an rgb color.
*/
void triRender(double a0, double a1, double b0, double b1, double c0,
        double c1, double r, double g, double b) {

    //Rearrange points to ensure 'a' is the leftmost vertex and subsequent
    //points are rearranged in a counter clockwise fasion.
    double leftmostX = a0;
    double leftmostY = a1;
    double secondX = b0;
    double secondY = b1;
    double finalX = c0;
    double finalY = c1;
    if (b0 < a0) {
        leftmostX = b0;
        leftmostY = b1;
        secondX = c0;
        secondY = c1;
        finalX = a0;
        finalY = a1;
        if (c0 < b0) {
            leftmostX = c0;
            leftmostY = c1;
            secondX = a0;
            secondY = a1;
            finalX = b0;
            finalY = b1;
        }
    }
    else if (c0 < a0) {
        leftmostX = c0;
        leftmostY = c1;
        secondX = a0;
        secondY = a1;
        finalX = b0;
        finalY = b1;
        if (b0 < c0) {
            leftmostX = b0;
            leftmostY = b1;
            secondX = c0;
            secondY = c1;
            finalX = a0;
            finalY = a1;
        }
    }

    //Rasterize the triangle
    //If the second point has the second largest x-value.
    if (finalX >= secondX) {
        for (int i = (int)ceil(leftmostX); i < (int)floor(secondX); i++) {
            double y0 = getYOfLine(i, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, secondX, secondY));
            double y1 = getYOfLine(i, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, finalX, finalY));
            for (int j = (int)ceil(y0); j < (int)floor(y1); j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
        for (int k = (int)ceil(secondX); k < (int)floor(finalX); k++) {
            double y0 = getYOfLine(k, secondX, secondY, slopeOf(secondX, secondY, finalX, finalY));
            double y1 = getYOfLine(k, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, finalX, finalY));
            for (int l = (int)ceil(y0); l < (int)floor(y1); l++) {
                pixSetRGB(k, l, r, g, b);
            }
        }
    }
    //If the second point has the largest x-value.
    else if (secondX > finalX) {
        for (int i = (int)ceil(leftmostX); i < (int)floor(finalX); i++) {
            double y0 = getYOfLine(i, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, secondX, secondY));
            double y1 = getYOfLine(i, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, finalX, finalY));
            for (int j = (int)ceil(y0); j < (int)floor(y1); j++) {
                pixSetRGB(i, j, r, g, b);
            }
        }
        for (int k = (int)ceil(finalX); k < (int)floor(secondX); k++) {
            double y0 = getYOfLine(k, leftmostX, leftmostY, slopeOf(leftmostX, leftmostY, secondX, secondY));
            double y1 = getYOfLine(k, finalX, finalY, slopeOf(finalX, finalY, secondX, secondY));
            for (int l = (int)ceil(y0); l < (int)floor(y1); l++) {
                pixSetRGB(k, l, r, g, b);
            }
        }
    }
}
