/*
* 090mainScene.c
* Danny Leal 01/09/2017
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "070vector.c"
#include "090matrix.c"
#include "040texture.c"
#include "090renderer.c"

#define renVERTNUMBOUND 16

/* Sets rgb, based on the other parameters, which are unaltered. vary is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double vary[], double rgb[]) {
    int varyIndexX = 0, varyIndexY = 1, varyIndexS = 2, varyIndexT = 3;
    int unifIndexR = 0, unifIndexG = 1, unifIndexB = 2;
    int texIndexR = 0, texIndexG = 1, texIndexB = 2;

    texSample(tex[0], vary[varyIndexS], vary[varyIndexT]);
    rgb[0] = tex[0]->sample[texIndexR] * unif[unifIndexR];
    rgb[1] = tex[0]->sample[texIndexG] * unif[unifIndexG];
    rgb[2] = tex[0]->sample[texIndexB] * unif[unifIndexB];
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    int renUNIFISOMETRY = 6;
    int renPLACEHOLDER = 2;

    //Set the third item in the attribute vector to '1' to create a 3D vector
    //The stored value will be remembered to replace the '1' added for 3d multiplication afterwards
    double stored = attr[renPLACEHOLDER];
    attr[renPLACEHOLDER] = 1.0;

    //Multiply the vector by the translation-rotation matrix stored in the uniforms
    mat331Multiply((double(*)[3])(&unif[renUNIFISOMETRY]), attr, vary);

    //Remove the 1 from the final vector
    attr[renPLACEHOLDER] = stored;

    //Set the remaining attributes for vary to be the same as attr.
    for (int i = 2; i < ren->varyDim; i++) {
        vary[i] = attr[i];
    }
}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    int renUNIFTHETA = 5;
    int renUNIFTRANSX = 3;
    int renUNIFTRANSY = 4;
    int renUNIFISOMETRY = 6;

    if (unifParent == NULL) {
        /* The nine uniforms for storing the matrix start at index
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
            unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
    else {
        double m[3][3];
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX],
            unif[renUNIFTRANSY], m);
        mat333Multiply((double(*)[3])(&unifParent[renUNIFISOMETRY]), m,
            (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
}

#include "090triangle.c"
#include "090mesh.c"
#include "090scene.c"

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;

//Global pointers to textures, meshes, and nodes
texTexture initTexture;
texTexture *tex[1];

renRenderer initRen;
renRenderer *ren;

meshMesh initMesh;
meshMesh *mesh;
meshMesh initMesh2;
meshMesh *mesh2;
meshMesh initMesh3;
meshMesh *mesh3;
meshMesh initMesh4;
meshMesh *mesh4;

sceneNode initNode;
sceneNode *node;
sceneNode initNode2;
sceneNode *node2;
sceneNode initNode3;
sceneNode *node3;
sceneNode initNode4;
sceneNode *node4;

//Set unif arrays for each node so that it is easy to translate them
//with respect to their parent
double unif[15] = {1.0, 1.0, 1.0, 256.0, 256.0, 0.0, 0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0};
double unif2[15] = {1.0, 1.0, 1.0, 100.0, -100.0, 0.0, 0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0};
double unif3[15] = {1.0, 1.0, 1.0, -100.0, -100.0, 0.0, 0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0};
double unif4[15] = {1.0, 1.0, 1.0, 50.0, -50.0, 0.0, 0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0,
                                                     0.0, 0.0, 0.0};

/* Draws a scene to the screen */
void draw() {
    pixClearRGB(0.0, 0.0, 0.0);
    sceneRender(node, ren, NULL);
}

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		//Alter the texture filtering of the image.
		if (textureFilter) {
			textureFilter = 0;
			texSetFiltering(tex[0], textureFilter);
			draw();
		}
		else {
			textureFilter = 1;
			texSetFiltering(tex[0], textureFilter);
			draw();
		}
	}
}

/* This callback is called once per animation frame. As parameters it receives
the time for the current frame and the time for the previous frame. Both times
are measured in seconds since some distant past time. */
void handleTimeStep(double oldTime, double newTime) {
    pixClearRGB(0.0, 0.0, 0.0);
    //Update the uniform to create an animation
    sceneSetOneUniform(node, 5, node->unif[5] + (M_PI / 600.0));
    draw();

	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(512, 512, "Scenes!") != 0)
		return 1;
	else {
        //Initialize nodes, mesh, renderer, and textures
        node = &initNode;
        node2 = &initNode2;
        node3 = &initNode3;
        node4 = &initNode4;

        mesh = &initMesh;
        mesh2 = &initMesh2;
        mesh3 = &initMesh3;
        mesh4 = &initMesh4;

        ren = &initRen;

		tex[0] = &initTexture;

        //Initialize textures
        texInitializeFile(tex[0], "winter.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(tex[0], textureFilter);
        texSetTopBottom(tex[0], 2);
        texSetLeftRight(tex[0], 2);

        //Initialize renderer
        ren->unifDim = 15;
        ren->texNum = 1;
        ren->attrDim = 4;
        ren->varyDim = 4;
        ren->colorPixel = colorPixel;
        ren->transformVertex = transformVertex;
        ren->updateUniform = updateUniform;

        //Initialize meshes
        meshInitializeEllipse(mesh, 0.0, 0.0, 200.0, 100.0, 40);
        meshInitializeEllipse(mesh2, 0.0, 0.0, 100.0, 50.0, 40);
        meshInitializeRectangle(mesh3, -100.0, 100.0, -100.0, 100.0);
        meshInitializeEllipse(mesh4, 0.0, 0.0, 100.0, 50, 40);

        //Initialize nodes
        sceneInitialize(node, ren, unif, tex, mesh, node2, NULL);
        sceneInitialize(node2, ren, unif2, tex, mesh2, node4, node3);
        sceneInitialize(node3, ren, unif3, tex, mesh3, NULL, NULL);
        sceneInitialize(node4, ren, unif4, tex, mesh4, NULL, NULL);

        //Set callback handlers
        pixSetKeyDownHandler(handleKeyDown);
        pixSetTimeStepHandler(handleTimeStep);

        //Draw scene to the screen
        draw();

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		texDestroy(tex[0]);
        meshDestroy(mesh);
        sceneDestroyRecursively(node);
		return 0;
	}
}
