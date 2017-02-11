/*
* 060mainEffect.c
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
    int attrIndexR = 4;
    int texIndexR = 0, texIndexG = 1, texIndexB = 2;

    texSample(tex[0], attr[attrIndexS], attr[attIndexT]);
    texSample(tex[1], attr[attrIndexS], attr[attIndexT]);
    if  (tex[1]->sample[texIndexB] >= unif[0]) {
        rgb[0] = tex[1]->sample[texIndexR];
        rgb[1] = tex[1]->sample[texIndexG];
        rgb[2] = tex[1]->sample[texIndexB];
    }
    else {
        rgb[0] = tex[0]->sample[texIndexR];
        rgb[1] = tex[0]->sample[texIndexG];
        rgb[2] = tex[0]->sample[texIndexB];
    }
}

#include "050triangle.c"

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;
texTexture initTexture;
texTexture initOverTexture;
texTexture *tex[2];
renRenderer initRen;
renRenderer *ren;

/* Draws a texture to the screen */
void draw(renRenderer *ren, texTexture *tex[]) {
    pixClearRGB(0.0, 0.0, 0.0);
    double a[4] = {20.0, 700.0, 0.0, 1.0};
    double b[4] = {20.0, 20.0, 0.0, 0.0};
    double c[4] = {1004.0, 360.0, 1.0, 0.5};
    double unif[3] = {0.7};
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
    if (pixInitialize(1024, 1024, "Texture Abstraction") != 0)
		return 1;
	else {
        //Render a triangle containing a specified texture
        //Initialize both renderer and texture objects
        ren = &initRen;
        ren->unifDim = 1;
        ren->texNum = 2;
        ren->attrDim = 4;
		tex[0] = &initTexture;
        tex[1] = &initOverTexture;
		texInitializeFile(tex[0], "winter.jpg");
        texInitializeFile(tex[1], "water.jpg");
        //Set filtering and wrapping techniques
		texSetFiltering(tex[0], textureFilter);
		texSetTopBottom(tex[0], 2);
		texSetLeftRight(tex[0], 2);
        texSetFiltering(tex[1], textureFilter);
		texSetTopBottom(tex[1], 2);
		texSetLeftRight(tex[1], 2);
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
