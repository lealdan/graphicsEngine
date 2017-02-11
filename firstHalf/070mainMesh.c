/*
* 070mainMesh.c
* Danny Leal 01/09/2017
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "070vector.c"
#include "030matrix.c"
#include "040texture.c"
#include "050renderer.c"

/* Sets rgb, based on the other parameters, which are unaltered. attr is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double attr[], double rgb[]) {
    int attrIndexX = 0, attrIndexY = 1, attrIndexS = 2, attrIndexT = 3;
    int attrIndexR = 4, attrIndexG = 5, attrIndexB = 6;
    int unifIndexR = 0, unifIndexG = 1, unifIndexB = 2;
    int texIndexR = 0, texIndexG = 1, texIndexB = 2;

    texSample(tex[0], attr[attrIndexS], attr[attrIndexT]);
    rgb[0] = tex[0]->sample[texIndexR] * unif[unifIndexR] * attr[attrIndexR];
    rgb[1] = tex[0]->sample[texIndexG] * unif[unifIndexG] * attr[attrIndexG];
    rgb[2] = tex[0]->sample[texIndexB] * unif[unifIndexB] * attr[attrIndexB];
}

#include "050triangle.c"
#include "070mesh.c"

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;
texTexture initTexture;
texTexture *tex[1];
renRenderer initRen;
renRenderer *ren;
meshMesh initMesh;
meshMesh *mesh;

/* Draws a texture to the screen */
void draw(meshMesh *mesh, renRenderer *ren, texTexture *tex[]) {
    pixClearRGB(0.0, 0.0, 0.0);
    //Initialize mesh
    meshInitialize(mesh, 4, 6, 7);
    //meshInitializeEllipse(mesh, 512.0, 360.0, 400.0, 200.0, 20);
    //meshInitializeRectangle(mesh, 20.0, 1004.0, 20.0, 700.0);
    //Initialize vertices
    double a[7] = {10.0, 25.0, 0.0, 0.0, 0.0, 1.0, 1.0};
    double b[7] = {256.0, 250.0, 0.5, 1.0, 1.0, 0.0, 1.0};
    double c[7] = {25.0, 300.0, 0.0, 1.0, 1.0, 1.0, 0.0};
    double d[7] = {256.0, 10.0, 0.5, 0.0, 1.0, 0.0, 0.0};
    double e[7] = {512.0, 35.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    double f[7] = {475.0, 350.0, 1.0, 1.0, 0.0, 1.0, 0.0};
    meshSetVertex(mesh, 0, a);
    meshSetVertex(mesh, 1, b);
    meshSetVertex(mesh, 2, c);
    meshSetVertex(mesh, 3, d);
    meshSetVertex(mesh, 4, e);
    meshSetVertex(mesh, 5, f);
    //Initialize triangles
    meshSetTriangle(mesh, 0, 0, 1, 2);
    meshSetTriangle(mesh, 1, 0, 3, 1);
    meshSetTriangle(mesh, 2, 3, 5, 1);
    meshSetTriangle(mesh, 3, 3, 4, 5);
    //Initialize renderer
    ren->unifDim = 3;
    ren->texNum = 1;
    ren->attrDim = 7;
    //Initialize textures
    texInitializeFile(tex[0], "winter.jpg");
    //Set filtering and wrapping techniques
    texSetFiltering(tex[0], textureFilter);
    texSetTopBottom(tex[0], 2);
    texSetLeftRight(tex[0], 2);
    //Initialize unif
    double unif[3] = {1.0, 1.0, 1.0};
    //Draw the mesh to the screen
    meshRender(mesh, ren, unif, tex);
}

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		//Alter the texture filtering of the image.
		if (textureFilter) {
			textureFilter = 0;
			texSetFiltering(tex[0], textureFilter);
			draw(mesh, ren, tex);
		}
		else {
			textureFilter = 1;
			texSetFiltering(tex[0], textureFilter);
			draw(mesh, ren, tex);
		}
	}
}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(512, 512, "Meshes") != 0)
		return 1;
	else {
        //Render a triangle containing a specified texture
        //Initialize mesh, renderer, and textures
        mesh = &initMesh;
        ren = &initRen;
		tex[0] = &initTexture;

        //Set callback handlers
		pixSetKeyDownHandler(handleKeyDown);

        //Draw the textures to the screen
        draw(mesh, ren, tex);

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		texDestroy(tex[0]);
		return 0;
	}
}
