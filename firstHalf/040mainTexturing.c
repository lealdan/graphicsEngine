/*
* 040mainTexturing.c
* Danny Leal 01/09/2017
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "030vector.c"
#include "030matrix.c"
#include "040texture.c"
#include "040triangle.c"

double a[2] = {20.0, 490.0};
double b[2] = {20.0, 20.0};
double c[2] = {490.0, 256.0};
double rgb[3] = {1.5, 1.5, 1.5};
double alpha[2] = {0.0, 2.0};
double beta[2] = {0.0, 0.0};
double _gamma[2] = {2.0, 0.5};

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;
texTexture initTexture;
texTexture *tex;

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		//Alter the texture filtering of the image.
		if (textureFilter) {
			textureFilter = 0;
			texSetFiltering(tex, textureFilter);
			triRender(a, b, c, rgb, tex, alpha, beta, _gamma);
		}
		else {
			textureFilter = 1;
			texSetFiltering(tex, textureFilter);
			triRender(a, b, c, rgb, tex, alpha, beta, _gamma);
		}
	}
}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(512, 512, "Texture Mapping") != 0)
		return 1;
	else {
        //Render a triangle containing a specified texture
		tex = &initTexture;
		texInitializeFile(tex, "dominos.png");
		texSetFiltering(tex, textureFilter);
		texSetTopBottom(tex, 2);
		texSetLeftRight(tex, 2);
        triRender(a, b, c, rgb, tex, alpha, beta, _gamma);
		pixSetKeyDownHandler(handleKeyDown);
		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		texDestroy(tex);
		return 0;
	}
}
