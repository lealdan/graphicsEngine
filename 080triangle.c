/*
* 080triangle.c
* Danny Leal 01/11/2017
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
double getYOfLine(double x, double a[2], double slope) {
    return slope * (x - a[0]) + a[1];
}

/*
* A simple function that calculates the slope of a line given two points.
*/
double slopeOf(double a[2], double b[2]) {
    if (a[0] == b[0]) {
        printf("Infinite Error!\n\n");
    }
    return (b[1] - a[1])/(b[0] - a[0]);
}

/*
* Rearranges vertices a, b, and c, so that the a vertex is placed in
* the array 'a', and the next vertices in counter-clockwise order are placed
* in b and final. (e.g. if b is a, c is b and a is last)
*/
void rearrangeVertices(double a[], double b[], double c[], renRenderer *ren) {
    double tempA[ren->varyDim];
    double tempB[ren->varyDim];
    double tempC[ren->varyDim];
    vecCopy(ren->varyDim, a, tempA);
    vecCopy(ren->varyDim, b, tempB);
    vecCopy(ren->varyDim, c, tempC);

    if (b[0] < a[0]) {
        vecCopy(ren->varyDim, b, tempA);
        vecCopy(ren->varyDim, c, tempB);
        vecCopy(ren->varyDim, a, tempC);
        if (c[0] < b[0]) {
            vecCopy(ren->varyDim, c, tempA);
            vecCopy(ren->varyDim, a, tempB);
            vecCopy(ren->varyDim, b, tempC);
        }
    }
    else if (c[0] < a[0]) {
        vecCopy(ren->varyDim, c, tempA);
        vecCopy(ren->varyDim, a, tempB);
        vecCopy(ren->varyDim, b, tempC);
        if (b[0] < c[0]) {
            vecCopy(ren->varyDim, b, tempA);
            vecCopy(ren->varyDim, c, tempB);
            vecCopy(ren->varyDim, a, tempC);
        }
    }

    vecCopy(ren->varyDim, tempA, a);
    vecCopy(ren->varyDim, tempB, b);
    vecCopy(ren->varyDim, tempC, c);
}

/*
* Calculates the values of vary (previously chi) based on the provided vectors, and a location
* within the triangle on the screen.
*/
void getVary(double a[], double b[], double c[], double x0, double x1, renRenderer *ren, double vary[]) {

    //Set up vectors and matrices for determining values of vary (chi)
    double v_vary[ren->varyDim];
    v_vary[0] = x0;
    v_vary[1] = x1;

    double v_varyMinusA[ren->varyDim];
    double v_bMinusA[ren->varyDim];
    double v_cMinusA[ren->varyDim];
    double m_bc[2][2];
    double inv[2][2];
    double v_pq[2];
    double determinant;
    double scaledBMinusA[ren->varyDim];
    double scaledCMinusA[ren->varyDim];
    double varyMinusA[ren->varyDim];

    //Create the vectors of the triangle sides
    vecSubtract(ren->varyDim, v_vary, a, v_varyMinusA);
    vecSubtract(ren->varyDim, b, a, v_bMinusA);
    vecSubtract(ren->varyDim, c, a, v_cMinusA);

    //Find the values of p and q
    //Create a 2 x 2 matrix of the 'sub-vectors' of vary
    m_bc[0][0] = v_bMinusA[0];
    m_bc[1][0] = v_bMinusA[1];
    m_bc[0][1] = v_cMinusA[0];
    m_bc[1][1] = v_cMinusA[1];
    //Create a vector of length 2 containing vector (vary - a)
    double tempV_AttrMinusA[2] = {v_varyMinusA[0], v_varyMinusA[1]};
    //Invert the 2 x 2 matrix and multiply by vector (vary - a) to get p and q
    determinant = mat22Invert(m_bc, inv);
    mat221Multiply(inv, tempV_AttrMinusA, v_pq);

    //Determine values of vary (All values in v_bMinusA[2...n] and v_cMinusA[2...n]
    //were previously known as some combination of alpha, beta, and gamma.)
    vecScale(ren->varyDim, v_pq[0], v_bMinusA, scaledBMinusA);
    vecScale(ren->varyDim, v_pq[1], v_cMinusA, scaledCMinusA);
    vecAdd(ren->varyDim, scaledBMinusA, scaledCMinusA, varyMinusA);
    vecAdd(ren->varyDim, a, varyMinusA, vary);

    //Set coordinates of varyAttr, since varyAttr[0] and varyAttr[1] contain weird calculations from a, b, and c
    vary[0] = x0;
    vary[1] = x1;
}

/*
* Renders a triangle given three vertices and an rgb color.
*/
void triRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {

    //Rename vertices to ensure that 'a' is the leftmost vertex and subsequent
    //points are arranged in a counter clockwise fasion.
    rearrangeVertices(a, b, c, ren);

    //Set up the final rgb color array for lighting pixels on the screen and
    //the vary array
    double color[3];
    double vary[renVARYDIMBOUND];

    //Rasterize the triangle
    //If the x-value of vertex 'b' is less than the x-value of vertex 'c.'
    if (c[0] >= b[0]) {
        if (a[0] == b[0]) {
            for (int i = (int)ceil(b[0]); i <= (int)floor(c[0]); i++) {
                double yLower = getYOfLine(i, b, slopeOf(b, c));
                double yUpper = getYOfLine(i, a, slopeOf(a, c));
                for (int j = (int)ceil(yLower); j <= (int)floor(yUpper); j++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)i, (double)j, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel (a == b): %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
        }
        if (b[0] == c[0]) {
            for (int i = (int)ceil(a[0]); i <= (int)floor(b[0]); i++) {
                double yLower = getYOfLine(i, a, slopeOf(a, b));
                double yUpper = getYOfLine(i, a, slopeOf(a, c));
                for (int j = (int)ceil(yLower); j <= (int)floor(yUpper); j++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)i, (double)j, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel b == c: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
        }
        else {
            for (int i = (int)ceil(a[0]); i <= (int)floor(b[0]); i++) {
                double yLower = getYOfLine(i, a, slopeOf(a, b));
                double yUpper = getYOfLine(i, a, slopeOf(a, c));
                for (int j = (int)ceil(yLower); j <= (int)floor(yUpper); j++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)i, (double)j, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel b < c left: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
            for (int k = (int)ceil(b[0]); k <= (int)floor(c[0]); k++) {
                double yLower = getYOfLine(k, b, slopeOf(b, c));
                double yUpper = getYOfLine(k, a, slopeOf(a, c));
                for (int l = (int)ceil(yLower); l <= (int)floor(yUpper); l++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)k, (double)l, ren, vary);
                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel b < c right: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
        }
    }
    //If vertex 'b' has the largest x-value.
    else if (b[0] > c[0]) {
        if (a[0] == c[0]) {
            for (int k = (int)ceil(c[0]); k <= (int)floor(b[0]); k++) {
                double yLower = getYOfLine(k, a, slopeOf(a, b));
                double yUpper = getYOfLine(k, c, slopeOf(c, b));
                for (int l = (int)ceil(yLower); l <= (int)floor(yUpper); l++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)k, (double)l, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel a == c: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
        }
        else {
            for (int i = (int)ceil(a[0]); i <= (int)floor(c[0]); i++) {
                double yLower = getYOfLine(i, a, slopeOf(a, b));
                double yUpper = getYOfLine(i, a, slopeOf(a, c));
                for (int j = (int)ceil(yLower); j <= (int)floor(yUpper); j++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)i, (double)j, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel b > c left: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
            for (int k = (int)ceil(c[0]); k <= (int)floor(b[0]); k++) {
                double yLower = getYOfLine(k, a, slopeOf(a, b));
                double yUpper = getYOfLine(k, c, slopeOf(c, b));
                for (int l = (int)ceil(yLower); l <= (int)floor(yUpper); l++) {
                    //Determine values of vary (chi/interpolation)
                    getVary(a, b, c, (double)k, (double)l, ren, vary);

                    //Set pixel colors to the texture sample and some amount of rgb
                    colorPixel(ren, unif, tex, vary, color);
                    if ((vary[0] > 350 && vary[1] > 355.0) || vary[1] < 40) {
                        printf("Coloring pixel b > c right: %f,%f\n", vary[0], vary[1]);
                    }
                    pixSetRGB(vary[0], vary[1], color[0], color[1], color[2]);
                }
            }
        }
    }
}
