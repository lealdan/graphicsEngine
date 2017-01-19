/*
* 050mainAbstracted.c
* Danny Leal 01/09/2017
*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "030vector.c"
#include "030matrix.c"
#include "040texture.c"
#include "050renderer.c"

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double attr[], double rgb[]) {
    int attrIndexX = 0, attrIndexY = 1, attrIndexS = 2, attIndexT = 3;
    int attrIndexR = 4, attrIndexG = 5, attrIndexB = 6;
    int unifIndexR = 0, unifIndexG = 1, unifIndexB = 2;
    int texIndexR = 0, texIndexG = 1, texIndexB = 2;

    texSample(tex[0], attr[attrIndexS], attr[attIndexT]);
    rgb[0] = tex[0]->sample[texIndexR] * unif[unifIndexR] * attr[attrIndexR];
    rgb[1] = tex[0]->sample[texIndexG] * unif[unifIndexG] * attr[attrIndexG];
    rgb[2] = tex[0]->sample[texIndexB] * unif[unifIndexB] * attr[attrIndexB];
}

#include "050triangle.c"

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;
texTexture initTexture;
texTexture *tex[1];
renRenderer initRen;
renRenderer *ren;

/* Draws a texture to the screen */
void draw(renRenderer *ren, texTexture *tex[]) {
    pixClearRGB(0.0, 0.0, 0.0);
    double a[7] = {20.0, 490.0, 0.0, 1.0, 1.0, 1.0, 1.0};
    double b[7] = {20.0, 20.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    double c[7] = {490.0, 256.0, 1.0, 0.5, 1.0, 1.0, 1.0};
    double unif[3] = {1.0, 1.0, 1.0};
    triRender(ren, unif, tex, a, b, c);
}

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		//Alter the texture filtering of the image.
		if (textureFilter) {
			textureFilter = 0;
			texSetFiltering(tex[0], textureFilter);
			draw(ren, tex);
		}
		else {
			textureFilter = 1;
			texSetFiltering(tex[0], textureFilter);
			draw(ren, tex);
		}
	}
}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(512, 512, "Texture Abstraction") != 0)
		return 1;
	else {
        //Render a triangle containing a specified texture
        //Initialize both renderer and texture objects
        ren = &initRen;
        ren->unifDim = 3;
        ren->texNum = 1;
        ren->attrDim = 7;
		tex[0] = &initTexture;
		texInitializeFile(tex[0], "dominos.png");

        //Set filtering and wrapping techniques
		texSetFiltering(tex[0], textureFilter);
		texSetTopBottom(tex[0], 2);
		texSetLeftRight(tex[0], 2);

        //Set callback handlers
		pixSetKeyDownHandler(handleKeyDown);

        //Draw the textures to the screen
        draw(ren, tex);

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		texDestroy(tex[0]);
		return 0;
	}
}
