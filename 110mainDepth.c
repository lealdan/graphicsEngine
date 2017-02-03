/*
* 110mainDepth.c
* Danny Leal 01/22/2017
*/

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "100vector.c"
#include "100matrix.c"
#include "040texture.c"
#include "110depth.c"
#include "110renderer.c"

#define renVERTNUMBOUND 16

#include "110triangle.c"
//#include "triangle.h"
#include "100mesh.c"
#include "090scene.c"

#define winDIM 512

#define attrIndexX 0
#define attrIndexY 1
#define attrIndexZ 2
#define attrIndexS 3
#define attrIndexT 4

#define varyIndexX 0
#define varyIndexY 1
#define varyIndexZ 2
#define varyIndexS 3
#define varyIndexT 4

#define texIndexR 0
#define texIndexG 1
#define texIndexB 2

#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTRANSX 3
#define renUNIFTRANSY 4
#define renUNIFTRANSZ 5
#define renUNIFALPHA 6
#define renUNIFPHI 7
#define renUNIFTHETA 8
#define renUNIFRADIUS 9
#define renUNIFISOMETRY 10
#define renPLACEHOLDER 3

#define renATTRDIM (3 + 2 + 3)
#define renVARYDIM 5
#define renTEXNUM 1
#define renUNIFDIM (3 + 3 + 3 + 1 + 16)

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;

//Global pointers to textures, meshes, and nodes
texTexture initTextureA;
texTexture *texA[1];
texTexture initTextureB;
texTexture *texB[1];

renRenderer initRen;
renRenderer *ren;

meshMesh initMeshA;
meshMesh *meshA;
meshMesh initMeshB;
meshMesh *meshB;
meshMesh initMeshC;
meshMesh *meshC;

sceneNode initNodeA;
sceneNode *nodeA;
sceneNode initNodeB;
sceneNode *nodeB;
sceneNode initNodeC;
sceneNode *nodeC;

depthBuffer initDepthBuffer;
depthBuffer *depth;

//Set unif arrays for each node so that it is easy to translate them
//with respect to their parent
double unifA[renUNIFDIM] = {0.8, 0.8, 0.8,
                            256.0, 256.0, 0.0,
                            0.0, M_PI / 4.0, M_PI / 2.0,
                            100.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0};

double unifB[renUNIFDIM] = {1.0, 1.0, 1.0,
                            200.0, 50.0, 300.0,
                            0.0, 0.0, M_PI / 3.0,
                            40.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0};

double unifC[renUNIFDIM] = {1.0, 1.0, 1.0,
                            -200.0, -125.0, -150.0,
                            0.0, M_PI / 2.0, M_PI / 3.0,
                            30.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0};


/* Sets rgb, based on the other parameters, which are unaltered. vary is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double vary[], double rgbz[]) {
    texSample(tex[0], vary[varyIndexS], vary[varyIndexT]);
    rgbz[0] = tex[0]->sample[texIndexR] * unif[renUNIFR];
    rgbz[1] = tex[0]->sample[texIndexG] * unif[renUNIFG];
    rgbz[2] = tex[0]->sample[texIndexB] * unif[renUNIFB];
    rgbz[3] = vary[2];
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    //Set the third item in the attribute vector to '1' to create a 3D vector
    //The stored value will be remembered to replace the '1' added for 3d multiplication afterwards
    double stored = attr[renPLACEHOLDER];
    attr[renPLACEHOLDER] = 1.0;

    //Multiply the vector by the translation-rotation matrix stored in the uniforms
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), attr, vary);

    //Remove the 1 from the final vector
    attr[renPLACEHOLDER] = stored;

    //Set the remaining attributes for vary to be the same as attr.
    vary[varyIndexS] = attr[attrIndexS];
    vary[varyIndexT] = attr[attrIndexT];
}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    double rot[3][3];
    double axis[3];
    double unitAxis[3];
    double length;

    vec3Spherical(unif[renUNIFRADIUS], unif[renUNIFPHI], unif[renUNIFTHETA], axis);
    length = vecUnit(3, axis, unitAxis);
    mat33AngleAxisRotation(unif[renUNIFALPHA], unitAxis, rot);

    if (unifParent == NULL) {
        /* The nine uniforms for storing the matrix start at index
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat44Isometry(rot, &unif[renUNIFTRANSX], (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
    else {
        double m[4][4];
        mat44Isometry(rot, &unif[renUNIFTRANSX], m);
        mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m,
            (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
}

/* Draws a scene to the screen */
void draw() {
    pixClearRGB(0.0, 0.0, 0.0);
    depthClearZs(depth, -99999.0);
    sceneRender(nodeA, ren, NULL);
}

void handleKeyDown(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		//Alter the texture filtering of the image.
		if (textureFilter) {
			textureFilter = 0;
			texSetFiltering(texA[0], textureFilter);
            texSetFiltering(texB[0], textureFilter);
			draw();
		}
		else {
			textureFilter = 1;
			texSetFiltering(texA[0], textureFilter);
            texSetFiltering(texB[0], textureFilter);
			draw();
		}
	}
}

/* This callback is called once per animation frame. As parameters it receives
the time for the current frame and the time for the previous frame. Both times
are measured in seconds since some distant past time. */
void handleTimeStep(double oldTime, double newTime) {
    pixClearRGB(0.0, 0.0, 0.0);
    depthClearZs(depth, -99999.0);
    //Update the uniform to create an animation
    sceneSetOneUniform(nodeA, renUNIFALPHA, nodeA->unif[renUNIFALPHA] + (M_PI / 300.0));
    sceneSetOneUniform(nodeB, renUNIFALPHA, nodeB->unif[renUNIFALPHA] - (M_PI / 300.0));
    sceneSetOneUniform(nodeC, renUNIFALPHA, nodeC->unif[renUNIFALPHA] + (M_PI / 100.0));
    draw();

	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(winDIM, winDIM, "3D!") != 0)
		return 1;
	else {
        //Initialize nodes, mesh, renderer, and textures
        nodeA = &initNodeA;
        nodeB = &initNodeB;
        nodeC = &initNodeC;

        meshA = &initMeshA;
        meshB = &initMeshB;
        meshC = &initMeshC;

        ren = &initRen;

		texA[0] = &initTextureA;
        texB[0] = &initTextureB;

        depth = &initDepthBuffer;

        depthInitialize(depth, winDIM, winDIM);

        //Initialize textures
        texInitializeFile(texA[0], "world.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(texA[0], textureFilter);
        texSetTopBottom(texA[0], 2);
        texSetLeftRight(texA[0], 2);

        texInitializeFile(texB[0], "moon.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(texB[0], textureFilter);
        texSetTopBottom(texB[0], 2);
        texSetLeftRight(texB[0], 2);

        //Initialize renderer
        ren->unifDim = renUNIFDIM;
        ren->texNum = renTEXNUM;
        ren->attrDim = renATTRDIM;
        ren->varyDim = renVARYDIM;
        ren->colorPixel = colorPixel;
        ren->transformVertex = transformVertex;
        ren->updateUniform = updateUniform;
        ren->depth = depth;

        //Initialize meshes
        //meshInitializeBox(meshA, -100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
        meshInitializeSphere(meshA, unifA[renUNIFRADIUS], 80, 40);
        meshInitializeSphere(meshB, unifB[renUNIFRADIUS], 20, 40);
        meshInitializeSphere(meshC, unifC[renUNIFRADIUS], 20, 40);

        //Initialize nodes
        sceneInitialize(nodeA, ren, unifA, texA, meshA, nodeB, NULL);
        sceneInitialize(nodeB, ren, unifB, texB, meshB, NULL, nodeC);
        sceneInitialize(nodeC, ren, unifC, texB, meshC, NULL, NULL);

        //Set callback handlers
        pixSetKeyDownHandler(handleKeyDown);
        pixSetTimeStepHandler(handleTimeStep);

        //Draw scene to the screen
        draw();

		/* Run the event loop. The callbacks that were registered above are
		invoked as needed. At the end, the resources supporting the window are
		deallocated. */
		pixRun();
		texDestroy(texA[0]);
        texDestroy(texB[0]);
        meshDestroy(meshA);
        meshDestroy(meshB);
        sceneDestroyRecursively(nodeA);
		return 0;
	}
}
