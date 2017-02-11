/*
* 030triangle.c
* Danny Leal 01/09/2017
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
    return (b[1] - a[1])/(b[0] - a[0]);
}

/*
* Rearranges vertices a, b, and c, so that the a vertex is placed in
* the array 'a', and the next vertices in counter-clockwise order are placed
* in b and final. (e.g. if b is a, c is b and a is last)
*/
void rearrangeVertices(double a[2], double b[2], double c[2], double alpha[3],
                        double beta[3], double gamma[3]) {
    double tempA[2];
    double tempB[2];
    double tempC[2];
    double tempAlpha[3];
    double tempBeta[3];
    double tempGamma[3];

    vecCopy(2, a, tempA);
    vecCopy(2, b, tempB);
    vecCopy(2, c, tempC);
    vecCopy(3, alpha, tempAlpha);
    vecCopy(3, beta, tempBeta);
    vecCopy(3, gamma, tempGamma);

    if (b[0] < a[0]) {
        vecCopy(2, b, tempA);
        vecCopy(2, c, tempB);
        vecCopy(2, a, tempC);
        vecCopy(3, beta, tempAlpha);
        vecCopy(3, gamma, tempBeta);
        vecCopy(3, alpha, tempGamma);
        if (c[0] < b[0]) {
            vecCopy(2, c, tempA);
            vecCopy(2, a, tempB);
            vecCopy(2, b, tempC);
            vecCopy(3, gamma, tempAlpha);
            vecCopy(3, alpha, tempBeta);
            vecCopy(3, beta, tempGamma);
        }
    }
    else if (c[0] < a[0]) {
        vecCopy(2, c, tempA);
        vecCopy(2, a, tempB);
        vecCopy(2, b, tempC);
        vecCopy(3, gamma, tempAlpha);
        vecCopy(3, alpha, tempBeta);
        vecCopy(3, beta, tempGamma);
        if (b[0] < c[0]) {
            vecCopy(2, b, tempA);
            vecCopy(2, c, tempB);
            vecCopy(2, a, tempC);
            vecCopy(3, beta, tempAlpha);
            vecCopy(3, gamma, tempBeta);
            vecCopy(3, alpha, tempGamma);
        }
    }

    vecCopy(2, tempA, a);
    vecCopy(2, tempB, b);
    vecCopy(2, tempC, c);
    vecCopy(3, tempAlpha, alpha);
    vecCopy(3, tempBeta, beta);
    vecCopy(3, tempGamma, gamma);
}

/*
* Calculates the values of chi based on the provided vectors, and location of x
* within the triangle.
*/
void getChi(double a[2], double b[2], double c[2], double alpha[3], double beta[3],
                double gamma[3], double v_x[2], double v_xMinusA[2], double v_bMinusA[2], double v_cMinusA[2],
                double m_bc[2][2], double inv[2][2], double v_pq[2], double chi[3],
                double determinant, double betaMinusAlpha[3], double gammaMinusAlpha[3],
                double scaledBetaAlpha[3], double scaledGammaAlpha[3], double chiMinusAlpha[3]) {

    //Create the vectors of the triangle sides
    vecSubtract(2.0, v_x, a, v_xMinusA);
    vecSubtract(2.0, b, a, v_bMinusA);
    vecSubtract(2.0, c, a, v_cMinusA);
    //Find the values of p and q
    mat22Columns(v_bMinusA, v_cMinusA, m_bc);
    determinant = mat22Invert(m_bc, inv);
    mat221Multiply(inv, v_xMinusA, v_pq);

    //Determine values of chi
    vecSubtract(3, beta, alpha, betaMinusAlpha);
    vecSubtract(3, gamma, alpha, gammaMinusAlpha);
    vecScale(3, v_pq[0], betaMinusAlpha, scaledBetaAlpha);
    vecScale(3, v_pq[1], gammaMinusAlpha, scaledGammaAlpha);
    vecAdd(3, scaledBetaAlpha, scaledGammaAlpha, chiMinusAlpha);
    vecAdd(3, alpha, chiMinusAlpha, chi);
}

/*
* Renders a triangle given three vertices and an rgb color.
*/
void triRender(double a[2], double b[2], double c[2], double rgb[3],
        double alpha[3], double beta[3], double gamma[3]) {

    //Rename vertices to ensure that 'a' is the leftmost vertex and subsequent
    //points are arranged in a counter clockwise fasion.
    rearrangeVertices(a, b, c, alpha, beta, gamma);

    //Set up vectors and matrices for determining values of chi
    double v_x[2];
    double v_xMinusA[2];
    double v_bMinusA[2];
    double v_cMinusA[2];
    double m_bc[2][2];
    double inv[2][2];
    double v_pq[2];
    double chi[3];
    double determinant;
    double betaMinusAlpha[3];
    double gammaMinusAlpha[3];
    double scaledBetaAlpha[3];
    double scaledGammaAlpha[3];
    double chiMinusAlpha[3];

    //Rasterize the triangle
    //If the x-value of vertex 'b' is less than the x-value of vertex 'c.'
    if (c[0] >= b[0]) {
        for (int i = (int)ceil(a[0]); i < (int)floor(b[0]); i++) {
            double yLower = getYOfLine(i, a, slopeOf(a, b));
            double yUpper = getYOfLine(i, a, slopeOf(a, c));
            for (int j = (int)ceil(yLower); j < (int)floor(yUpper); j++) {
                v_x[0] = (double)i;
                v_x[1] = (double)j;

                //Determine values of chi
                getChi(a, b, c, alpha, beta, gamma, v_x, v_xMinusA,
                        v_bMinusA, v_cMinusA, m_bc, inv, v_pq, chi, determinant,
                        betaMinusAlpha, gammaMinusAlpha, scaledBetaAlpha,
                        scaledGammaAlpha, chiMinusAlpha);

                //Set pixel colors to chi and some amount of rgb
                double color[3];
                color[0] = rgb[0] * chi[0];
                color[1] = rgb[1] * chi[1];
                color[2] = rgb[2] * chi[2];
                pixSetRGB(i, j, color[0], color[1], color[2]);
            }
        }
        for (int k = (int)ceil(b[0]); k < (int)floor(c[0]); k++) {
            double yLower = getYOfLine(k, b, slopeOf(b, c));
            double yUpper = getYOfLine(k, a, slopeOf(a, c));
            for (int l = (int)ceil(yLower); l < (int)floor(yUpper); l++) {
                v_x[0] = (double)k;
                v_x[1] = (double)l;

                //Determine values of chi
                getChi(a, b, c, alpha, beta, gamma, v_x, v_xMinusA,
                        v_bMinusA, v_cMinusA, m_bc, inv, v_pq, chi, determinant,
                        betaMinusAlpha, gammaMinusAlpha, scaledBetaAlpha,
                        scaledGammaAlpha, chiMinusAlpha);

                //Set pixel colors to chi and some amount of rgb
                double color[3];
                color[0] = rgb[0] * chi[0];
                color[1] = rgb[1] * chi[1];
                color[2] = rgb[2] * chi[2];
                pixSetRGB(k, l, color[0], color[1], color[2]);
            }
        }
    }
    //If vertex 'b' has the largest x-value.
    else if (b[0] > c[0]) {
        for (int i = (int)ceil(a[0]); i < (int)floor(c[0]); i++) {
            double yLower = getYOfLine(i, a, slopeOf(a, b));
            double yUpper = getYOfLine(i, a, slopeOf(a, c));
            for (int j = (int)ceil(yLower); j < (int)floor(yUpper); j++) {
                v_x[0] = (double)i;
                v_x[1] = (double)j;

                //Determine values of chi
                getChi(a, b, c, alpha, beta, gamma, v_x, v_xMinusA,
                        v_bMinusA, v_cMinusA, m_bc, inv, v_pq, chi, determinant,
                        betaMinusAlpha, gammaMinusAlpha, scaledBetaAlpha,
                        scaledGammaAlpha, chiMinusAlpha);

                //Set pixel colors to chi and some amount of rgb
                double color[3];
                color[0] = rgb[0] * chi[0];
                color[1] = rgb[1] * chi[1];
                color[2] = rgb[2] * chi[2];
                pixSetRGB(i, j, color[0], color[1], color[2]);
            }
        }
        for (int k = (int)ceil(c[0]); k < (int)floor(b[0]); k++) {
            double yLower = getYOfLine(k, a, slopeOf(a, b));
            double yUpper = getYOfLine(k, c, slopeOf(c, b));
            for (int l = (int)ceil(yLower); l < (int)floor(yUpper); l++) {
                v_x[0] = (double)k;
                v_x[1] = (double)l;

                //Determine values of chi
                getChi(a, b, c, alpha, beta, gamma, v_x, v_xMinusA,
                        v_bMinusA, v_cMinusA, m_bc, inv, v_pq, chi, determinant,
                        betaMinusAlpha, gammaMinusAlpha, scaledBetaAlpha,
                        scaledGammaAlpha, chiMinusAlpha);

                //Set pixel colors to chi and some amount of rgb
                double color[3];
                color[0] = rgb[0] * chi[0];
                color[1] = rgb[1] * chi[1];
                color[2] = rgb[2] * chi[2];
                pixSetRGB(k, l, color[0], color[1], color[2]);
            }
        }
    }
}
