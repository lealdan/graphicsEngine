/*
* 120mainCamera.c
* Danny Leal 01/26/2017
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
#include "120renderer.c"

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
#define renCAMERA 26

#define renPLACEHOLDER 3

#define renATTRDIM (3 + 2 + 3)
#define renVARYDIM (3 + 2)
#define renTEXNUM 1
#define renUNIFDIM (3 + 3 + 3 + 1 + 16 + 16)

//Global variables intended as flags for callbacks to alter
int textureFilter = 1;

//Global pointers to textures, meshes, and nodes
texTexture initTextureA;
texTexture *texA[1];
texTexture initTextureB;
texTexture *texB[1];
texTexture initTextureC;
texTexture *texC[1];

renRenderer initRen;
renRenderer *ren;

meshMesh initMeshA;
meshMesh *meshA;
meshMesh initMeshB;
meshMesh *meshB;
meshMesh initMeshC;
meshMesh *meshC;
meshMesh initMeshD;
meshMesh *meshD;
meshMesh initMeshE;
meshMesh *meshE;

sceneNode initNodeA;
sceneNode *nodeA;
sceneNode initNodeB;
sceneNode *nodeB;
sceneNode initNodeC;
sceneNode *nodeC;
sceneNode initNodeD;
sceneNode *nodeD;
sceneNode initNodeE;
sceneNode *nodeE;

depthBuffer initDepthBuffer;
depthBuffer *depth;

//Set unif arrays for each node so that it is easy to translate them
//with respect to their parent
double unifA[renUNIFDIM] = {2.5, 2.5, 2.5,
                            0.0, 0.0, 0.0,
                            0.0, 0.0, M_PI / 2.0,
                            60.0,
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0};

double unifB[renUNIFDIM] = {1.5, 1.5, 1.5,
                            200.0, 150.0, 5.0,
                            0.0, 0.0, M_PI,
                            15.0,
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0};

double unifC[renUNIFDIM] = {1.0, 1.0, 1.0,
                            -50.0, -70.0, -5.0,
                            0.0, M_PI / 2.0, M_PI / 3.0,
                            7.5,
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0};

double unifD[renUNIFDIM] = {0.9, 0.9, 0.9,
                            70.0, 95.0, -1.0,
                            0.0, M_PI / 2.0, M_PI / 3.0,
                            12.0,
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0};

double unifE[renUNIFDIM] = {1.0, 1.0, 1.0,
                            -5.0, -30.0, 0.0,
                            0.0, M_PI / 2.0, M_PI / 3.0,
                            2.5,
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0};

double target[3] = {0.0, 0.0, 0.0};
double camAngleY = 0.0;
double camAngleX = 0.0;
double camDistance = 600.0;


/* Sets rgb, based on the other parameters, which are unaltered. vary is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double vary[], double rgbz[]) {
    texSample(tex[0], vary[varyIndexS], vary[varyIndexT]);
    rgbz[0] = tex[0]->sample[texIndexR] * unif[renUNIFR];
    rgbz[1] = tex[0]->sample[texIndexG] * unif[renUNIFG];
    rgbz[2] = tex[0]->sample[texIndexB] * unif[renUNIFB];
    //Set the depth buffer (texture z-value)
    rgbz[3] = vary[2];
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[],
        double vary[]) {
    //Set the third item in the attribute vector to '1' to create a 3D vector
    //The stored value will be remembered to replace the '1' added for 3d multiplication afterwards
    double stored = attr[renPLACEHOLDER];
    attr[renPLACEHOLDER] = 1.0;

    //Multiply the vector by the translation-rotation matrix stored in the uniforms to
    //achieve the model transformations
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), attr, vary);

    //Multiply the vector by the inverse of the camera matrix to position the camera
    double tmp[4];
    mat441Multiply((double(*)[4])(&unif[renCAMERA]), vary, tmp);

    //Remove the placeHolder '1' from the final vector
    attr[renPLACEHOLDER] = stored;

    //Set the remaining attributes for vary to be the same as attr.
    vary[varyIndexX] = tmp[varyIndexX];
    vary[varyIndexY] = tmp[varyIndexY];
    vary[varyIndexZ] = tmp[varyIndexZ];
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
    unif[renCAMERA] = ren->viewing[0][0];
    unif[renCAMERA + 1] = ren->viewing[0][1];
    unif[renCAMERA + 2] = ren->viewing[0][2];
    unif[renCAMERA + 3] = ren->viewing[0][3];
    unif[renCAMERA + 4] = ren->viewing[1][0];
    unif[renCAMERA + 5] = ren->viewing[1][1];
    unif[renCAMERA + 6] = ren->viewing[1][2];
    unif[renCAMERA + 7] = ren->viewing[1][3];
    unif[renCAMERA + 8] = ren->viewing[2][0];
    unif[renCAMERA + 9] = ren->viewing[2][1];
    unif[renCAMERA + 10] = ren->viewing[2][2];
    unif[renCAMERA + 11] = ren->viewing[2][3];
    unif[renCAMERA + 12] = ren->viewing[3][0];
    unif[renCAMERA + 13] = ren->viewing[3][1];
    unif[renCAMERA + 14] = ren->viewing[3][2];
    unif[renCAMERA + 15] = ren->viewing[3][3];

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
    renUpdateViewing(ren);
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
            texSetFiltering(texC[0], textureFilter);
			draw();
		}
		else {
			textureFilter = 1;
			texSetFiltering(texA[0], textureFilter);
            texSetFiltering(texB[0], textureFilter);
            texSetFiltering(texC[0], textureFilter);
			draw();
		}
	}
    if (key == GLFW_KEY_A) {
        //Alter the texture filtering of the image.
        target[1] += 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_S) {
        //Alter the texture filtering of the image.
        target[2] += 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_D) {
        //Alter the texture filtering of the image.
        target[1] -= 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_W) {
        //Alter the texture filtering of the image.
        target[2] -= 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_Z) {
        //Alter the texture filtering of the image.
        target[0] += 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_X) {
        //Alter the texture filtering of the image.
        target[0] -= 10;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_I) {
        camAngleY += 0.1;
        //Alter the texture filtering of the image.
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_K) {
        //Alter the texture filtering of the image.
        camAngleY -= 0.1;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_J) {
        //Alter the texture filtering of the image.
        camAngleX += 0.1;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
    }
    if (key == GLFW_KEY_L) {
        //Alter the texture filtering of the image.
        camAngleX -= 0.1;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);
        draw();
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
    sceneSetOneUniform(nodeB, renUNIFALPHA, nodeB->unif[renUNIFALPHA] + (M_PI / 100.0));
    sceneSetOneUniform(nodeC, renUNIFALPHA, nodeC->unif[renUNIFALPHA] - (M_PI / 100.0));
    sceneSetOneUniform(nodeD, renUNIFALPHA, nodeD->unif[renUNIFALPHA] - (M_PI / 300.0));
    sceneSetOneUniform(nodeE, renUNIFALPHA, nodeE->unif[renUNIFALPHA] - (M_PI / 100.0));
    draw();

	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));

}

int main(void) {
    //Initialize a draw window
    if (pixInitialize(winDIM, winDIM, "3D!") != 0)
		return 1;
	else {
        //Initialize depth buffer
        depth = &initDepthBuffer;
        depthInitialize(depth, winDIM, winDIM);

        //Initialize renderer
        ren = &initRen;
        ren->unifDim = renUNIFDIM;
        ren->texNum = renTEXNUM;
        ren->attrDim = renATTRDIM;
        ren->varyDim = renVARYDIM;
        ren->colorPixel = colorPixel;
        ren->transformVertex = transformVertex;
        ren->updateUniform = updateUniform;
        ren->depth = depth;
        renLookAt(ren, target, camDistance, camAngleY, camAngleX);

        //Initialize textures
		texA[0] = &initTextureA;
        texB[0] = &initTextureB;
        texC[0] = &initTextureC;
        texInitializeFile(texA[0], "venus.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(texA[0], textureFilter);
        texSetTopBottom(texA[0], 2);
        texSetLeftRight(texA[0], 2);
        texInitializeFile(texB[0], "world.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(texB[0], textureFilter);
        texSetTopBottom(texB[0], 2);
        texSetLeftRight(texB[0], 2);
        texInitializeFile(texC[0], "moon.jpg");
        //Set filtering and wrapping techniques
        texSetFiltering(texC[0], textureFilter);
        texSetTopBottom(texC[0], 2);
        texSetLeftRight(texC[0], 2);

        //Initialize meshes
        meshA = &initMeshA;
        meshB = &initMeshB;
        meshC = &initMeshC;
        meshD = &initMeshD;
        meshE = &initMeshE;
        //meshInitializeBox(meshA, -100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
        // double zs[3][4] = {
        // 	{100.0, 90.0, 70.0, 60.0},
        // 	{60.0, 50.0, 30.0, 10.0},
        // 	{40.0, 30.0, -10.0, -20.0}};
        // int error = meshInitializeLandscape(meshA, 3, 4, 100.0, (double *)zs);
        meshInitializeSphere(meshA, unifA[renUNIFRADIUS], 80, 40);
        meshInitializeSphere(meshB, unifB[renUNIFRADIUS], 20, 40);
        meshInitializeSphere(meshC, unifC[renUNIFRADIUS], 20, 40);
        meshInitializeSphere(meshD, unifD[renUNIFRADIUS], 20, 40);
        meshInitializeSphere(meshE, unifE[renUNIFRADIUS], 20, 40);

        //Initialize nodes
        nodeA = &initNodeA;
        nodeB = &initNodeB;
        nodeC = &initNodeC;
        nodeD = &initNodeD;
        nodeE = &initNodeE;
        sceneInitialize(nodeA, ren, unifA, texA, meshA, nodeB, NULL);
        sceneInitialize(nodeB, ren, unifB, texB, meshB, nodeE, nodeC);
        sceneInitialize(nodeC, ren, unifC, texC, meshC, NULL, NULL);
        sceneInitialize(nodeD, ren, unifD, texA, meshD, NULL, NULL);
        sceneInitialize(nodeE, ren, unifE, texC, meshE, NULL, NULL);
        sceneAddSibling(nodeB, nodeD);

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
        texDestroy(texC[0]);
        meshDestroy(meshA);
        meshDestroy(meshB);
        meshDestroy(meshC);
        meshDestroy(meshD);
        meshDestroy(meshE);
        sceneDestroyRecursively(nodeA);
		return 0;
	}
}