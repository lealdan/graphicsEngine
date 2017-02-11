/*
* 140clipping.c
* Danny Leal 01/28/2017
*/

/* Clips out the two vertices that are out of clipping space and forms two new triangles */
void clipTwo(renRenderer *ren, double unif[], texTexture *tex[], double first[],
            double second[], double unclipped[]) {
    double firstMinusUnclipped[renVARYDIM];
    double secondMinusUnclipped[renVARYDIM];
    double firstScaled[renVARYDIM];
    double secondScaled[renVARYDIM];
    double newFirst[renVARYDIM];
    double newSecond[renVARYDIM];
    double firstDiv[4];
    double secondDiv[4];
    double unclippedDiv[4];

    // Calculate 't', the scaling factor to the clipping plane
    double t = (first[varyIndexW] - first[varyIndexZ]) /
            (first[varyIndexW] - first[varyIndexZ] + unclipped[varyIndexZ] - unclipped[varyIndexW]);

    double t2 = (second[varyIndexW] - second[varyIndexZ]) /
            (second[varyIndexW] - second[varyIndexZ] + unclipped[varyIndexZ] - unclipped[varyIndexW]);

    // Find the new vertices for the clipped vertices: a + t (b - a)
    vecSubtract(renVARYDIM, unclipped, first, firstMinusUnclipped);
    vecSubtract(renVARYDIM, unclipped, second, secondMinusUnclipped);
    vecScale(renVARYDIM, t, firstMinusUnclipped, firstScaled);
    vecScale(renVARYDIM, t2, secondMinusUnclipped, secondScaled);
    vecAdd(renVARYDIM, first, firstScaled, newFirst);
    vecAdd(renVARYDIM, second, secondScaled, newSecond);

    vecScale(4, 1.0 / first[varyIndexW], first, firstDiv);
    vecScale(4, 1.0 / second[varyIndexW], second, secondDiv);
    vecScale(4, 1.0 / unclipped[varyIndexW], unclipped, unclippedDiv);

    //Perform viewport transformation
    mat441Multiply(ren->viewport, firstDiv, first);
    mat441Multiply(ren->viewport, secondDiv, second);
    mat441Multiply(ren->viewport, unclippedDiv, unclipped);
}

/* Clips out the vertex that is out of clipping space and forms a new triangle */
void clipOne(renRenderer *ren, double unif[], texTexture *tex[], double clipped[],
            double second[], double third[], double new[]) {
    double clippedMinusSecond[renVARYDIM];
    double clippedMinusThird[renVARYDIM];
    double secondScaled[renVARYDIM];
    double thirdScaled[renVARYDIM];
    double newFirst[renVARYDIM];
    double newSecond[renVARYDIM];
    double newFirstDiv[4];
    double newSecondDiv[4];
    double secondDiv[4];
    double thirdDiv[4];

    // Calculate 't', the scaling factor to the clipping plane  t = (a3 - a2) / (a3 - a2 + b2 - b3),
    // where a is clipped and b is unclipped.
    double t = (clipped[varyIndexW] - clipped[varyIndexZ]) /
            (clipped[varyIndexW] - clipped[varyIndexZ] + second[varyIndexZ] - second[varyIndexW]);

    double t2 = (clipped[varyIndexW] - clipped[varyIndexZ]) /
            (clipped[varyIndexW] - clipped[varyIndexZ] + third[varyIndexZ] - third[varyIndexW]);

    // Find the new vertices formed from following the clipped vertex: a + t (b - a)
    vecSubtract(renVARYDIM, second, clipped, clippedMinusSecond);
    vecSubtract(renVARYDIM, third, clipped, clippedMinusThird);
    vecScale(renVARYDIM, t, clippedMinusSecond, secondScaled);
    vecScale(renVARYDIM, t2, clippedMinusThird, thirdScaled);
    vecAdd(renVARYDIM, clipped, secondScaled, newFirst);
    vecAdd(renVARYDIM, clipped, thirdScaled, newSecond);

    vecScale(4, 1.0 / newFirst[varyIndexW], newFirst, newFirstDiv);
    vecScale(4, 1.0 / newSecond[varyIndexW], newSecond, newSecondDiv);
    vecScale(4, 1.0 / second[varyIndexW], second, secondDiv);
    vecScale(4, 1.0 / third[varyIndexW], third, thirdDiv);

    //Perform viewport transformation
    mat441Multiply(ren->viewport, newFirstDiv, clipped);
    mat441Multiply(ren->viewport, newSecondDiv, new);
    mat441Multiply(ren->viewport, secondDiv, second);
    mat441Multiply(ren->viewport, thirdDiv, third);
}

/*
* Performs clipping for the projection
*/
void clipRender(renRenderer *ren, double unif[], texTexture *tex[], double a[],
        double b[], double c[]) {
    double aDiv[4];
    double bDiv[4];
    double cDiv[4];

    if ((a[varyIndexZ] > a[varyIndexW] || a[varyIndexW] <= 0) &&
        (b[varyIndexZ] > b[varyIndexW] || b[varyIndexW] <= 0) &&
        (c[varyIndexZ] > c[varyIndexW] || c[varyIndexW] <= 0)) {
        return;
    }
    else if ((a[varyIndexZ] > a[varyIndexW] || a[varyIndexW] <= 0) &&
        (b[varyIndexZ] > b[varyIndexW] || b[varyIndexW] <= 0)) {
        clipTwo(ren, unif, tex, a, b, c);
        triRender(ren, unif, tex, a, b, c);
    }
    else if ((b[varyIndexZ] > b[varyIndexW] || b[varyIndexW] <= 0) &&
        (c[varyIndexZ] > c[varyIndexW] || c[varyIndexW] <= 0)) {
        clipTwo(ren, unif, tex, b, c, a);
        triRender(ren, unif, tex, a, b, c);
    }
    else if ((a[varyIndexZ] > a[varyIndexW] || a[varyIndexW] <= 0) &&
        (c[varyIndexZ] > c[varyIndexW] || c[varyIndexW] <= 0)) {
        clipTwo(ren, unif, tex, a, c, b);
        triRender(ren, unif, tex, a, b, c);
    }
    else if (a[varyIndexZ] > a[varyIndexW] || a[varyIndexW] <= 0) {
        double new[renVARYDIM];
        clipOne(ren, unif, tex, a, b, c, new);
        triRender(ren, unif, tex, a, new, c);
        triRender(ren, unif, tex, new, b, c);
    }
    else if (b[varyIndexZ] > b[varyIndexW] || b[varyIndexW] <= 0) {
        double new[renVARYDIM];
        clipOne(ren, unif, tex, b, c, a, new);
        triRender(ren, unif, tex, b, new, a);
        triRender(ren, unif, tex, new, c, a);
    }
    else if (c[varyIndexZ] > c[varyIndexW] || c[varyIndexW] <= 0) {
        double new[renVARYDIM];
        clipOne(ren, unif, tex, c, a, b, new);
        triRender(ren, unif, tex, c, new, b);
        triRender(ren, unif, tex, new, a, b);
    }
    else {
        vecScale(4, 1.0 / a[varyIndexW], a, aDiv);
        vecScale(4, 1.0 / b[varyIndexW], b, bDiv);
        vecScale(4, 1.0 / c[varyIndexW], c, cDiv);

        //Perform viewport transformation
        mat441Multiply(ren->viewport, aDiv, a);
        mat441Multiply(ren->viewport, bDiv, b);
        mat441Multiply(ren->viewport, cDiv, c);

        //Rasterize triangles
        triRender(ren, unif, tex, a, b ,c);
    }
}
