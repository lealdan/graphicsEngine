


/* On macOS, compile with...
    clang 140mainClipping.c 080pixel.o -lglfw -framework OpenGL
*/

#define renVERTNUMBOUND 1000
#define renVARYDIMBOUND 16
#define renHUGEDEPTH -10000000.0
#define renATTRDIM (3 + 2 + 3)
#define renVARYDIM (4 + 2 + 3 + 3)
#define varyIndexX 0
#define varyIndexY 1
#define varyIndexZ 2
#define varyIndexW 3

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "000pixel.h"
#include <GLFW/glfw3.h>
#include "100vector.c"
#include "130matrix.c"
#include "040texture.c"
#include "110depth.c"
#include "130renderer.c"

#include "110triangle.c"
//#include "triangle.h"
#include "140clipping.c"
#include "140mesh.c"
#include "090scene.c"

#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRN 5
#define renATTRO 6
#define renATTRP 7

#define varyIndexX 0
#define varyIndexY 1
#define varyIndexZ 2
#define varyIndexW 3
#define varyIndexS 4
#define varyIndexT 5
#define varyIndexWorldX 6
#define varyIndexWorldY 7
#define varyIndexWorldZ 8
#define varyIndexN 9
#define varyIndexO 10
#define varyIndexP 11

#define renPLACEHOLDER 3

#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTRANSX 3
#define renUNIFTRANSY 4
#define renUNIFTRANSZ 5
#define renUNIFALPHA 6
#define renUNIFPHI 7
#define renUNIFTHETA 8
#define renUNIFISOMETRY 9
#define renUNIFVIEWING (renUNIFISOMETRY + 16)

#define texIndexR 0
#define texIndexG 1
#define texIndexB 2

#define renLIGHTPOSX (renUNIFVIEWING + 16)
#define renLIGHTPOSY (renLIGHTPOSX + 1)
#define renLIGHTPOSZ (renLIGHTPOSY + 1)
#define renLIGHTR (renLIGHTPOSZ + 1)
#define renLIGHTG (renLIGHTR + 1)
#define renLIGHTB (renLIGHTG + 1)
#define renCAMERAPOSX (renLIGHTB + 1)
#define renCAMERAPOSY (renCAMERAPOSX + 1)
#define renCAMERAPOSZ (renCAMERAPOSY + 1)

depthBuffer depthBuf;
texTexture textureHoriz, textureVert, textureWater, textureTrunk, textureTree;
texTexture *texHoriz, *texVert, *texWater, *texTrunk, *texTree;
renRenderer ren;
meshMesh meshLand, meshHoriz, meshVert, meshWater, meshTrunk, meshTree;
double unifA[3 + 3 + 3 + 16 + 16 + 3 + 3 + 3] = {
	1.0, 1.0, 1.0,
	0.0, 0.0, 0.0,
	0.0, M_PI / 4.0, M_PI / 4.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	0.0, 0.0, 0.0, //Lighting location
	0.7, 0.7, 0.7, //Lighting RGB
	0.0, 0.0, 0.0};
double unifB[3 + 3 + 3 + 16 + 16 + 3 + 3 + 3] = {
	1.0, 1.0, 1.0,
	10.0, 30.0, 5.0,
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	0.0, 0.0, 0.0, //Lighting location
	0.7, 0.7, 0.7, //Lighting RGB
	0.0, 0.0, 0.0};
double unifC[3 + 3 + 3 + 16 + 16 + 3 + 3 + 3] = {
	1.0, 1.0, 1.0,
	0.0, 0.0, 7.0,
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
	0.0, 0.0, 0.0, //Lighting location
	0.7, 0.7, 0.7, //Lighting RGB
	0.0, 0.0, 0.0};
sceneNode nodeHoriz, nodeVert, nodeWater, nodeTrunk, nodeTree;
double cameraTarget[3] = {30.0, 30.0, 5.0};
double cameraRho = 30.0, cameraPhi = M_PI / 4.0, cameraTheta = M_PI / 4.0;

void draw(void) {
	pixClearRGB(0.7, 0.7, 0.7);
	depthClearZs(&depthBuf, renHUGEDEPTH);
	renLookAt(&ren, cameraTarget, cameraRho, cameraPhi, cameraTheta);
	renUpdateViewing(&ren);
	sceneRender(&nodeHoriz, &ren, NULL);
}

void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		if (textureHoriz.filtering == texQUADRATIC)
			texSetFiltering(&textureHoriz, texNEAREST);
		else
			texSetFiltering(&textureHoriz, texQUADRATIC);
	} else if (key == GLFW_KEY_L) {
		if (ren.projectionType == renPERSPECTIVE)
			renSetFrustum(&ren, renORTHOGRAPHIC, M_PI / 3.0, cameraRho, 10.0);
		else
			renSetFrustum(&ren, renPERSPECTIVE, M_PI / 3.0, cameraRho, 10.0);
	} else if (key == GLFW_KEY_LEFT)
		cameraTarget[0] -= 1.0;
	else if (key == GLFW_KEY_RIGHT)
		cameraTarget[0] += 1.0;
	else if (key == GLFW_KEY_DOWN)
		cameraTarget[1] -= 1.0;
	else if (key == GLFW_KEY_UP)
		cameraTarget[1] += 1.0;
	else if (key == GLFW_KEY_O)
		cameraTheta -= 0.1;
	else if (key == GLFW_KEY_P)
		cameraTheta += 0.1;
	else if (key == GLFW_KEY_I)
		cameraPhi -= 0.1;
	else if (key == GLFW_KEY_K)
		cameraPhi += 0.1;
	else if (key == GLFW_KEY_U)
		cameraRho -= 1.0;
	else if (key == GLFW_KEY_J)
		cameraRho += 1.0;
	draw();
}

void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	//sceneSetOneUniform(&nodeA, renUNIFALPHA,
	//	nodeA.unif[renUNIFALPHA] + (newTime - oldTime) / 4.0);
	//draw();
}

/* Sets rgb, based on the other parameters, which are unaltered. vary is an
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture *tex[],
        double vary[], double rgbz[]) {
    double n[3] = {vary[varyIndexN], vary[varyIndexO], vary[varyIndexP]};
    double l[3] = {unif[renLIGHTPOSX], unif[renLIGHTPOSY], unif[renLIGHTPOSZ]};
    double camera[3] = {unif[renCAMERAPOSX], unif[renCAMERAPOSY], unif[renCAMERAPOSZ]};
    double cameraUnit[3];
    double nUnit[3];
    double lUnit[3];
    double p[3];
    double twoTimesP[3];
    double r[3];
    double nDotL;
    double diffInt;
    double rDotC;
    double specInt;
    double finalRGB[3];
    double scaledFinalRGB[3];
    double scaledFog[3];
    double tempFinalFog[3];

    double shininess = 30.0;
    double coat[3] = {0.7, 0.7, 0.7};
    double ambience = 0.2;
    double fog[3] = {0.7, 0.7, 0.7};

    texSample(tex[0], vary[varyIndexS], vary[varyIndexT]);

    vecUnit(3, n, nUnit);
    vecUnit(3, l, lUnit);
    vecUnit(3, camera, cameraUnit);
    nDotL = vecDot(3, nUnit, lUnit);
    diffInt = fmax(0.0, nDotL);
    diffInt += ambience;

    vecScale(3, nDotL, nUnit, p);
    vecScale(3, 2.0, p, twoTimesP);
    vecSubtract(3, twoTimesP, lUnit, r);
    rDotC = vecDot(3, r, cameraUnit);
    specInt = pow(fmax(0.0, rDotC), shininess);

    finalRGB[0] = diffInt * tex[0]->sample[texIndexR] * unif[renUNIFR] * unif[renLIGHTR] + (specInt * unif[renLIGHTR] * coat[0]);
    finalRGB[1] = diffInt * tex[0]->sample[texIndexG] * unif[renUNIFG] * unif[renLIGHTG] + (specInt * unif[renLIGHTG] * coat[1]);
    finalRGB[2] = diffInt * tex[0]->sample[texIndexB] * unif[renUNIFB] * unif[renLIGHTB] + (specInt * unif[renLIGHTB] * coat[2]);

    //Implement fog
    vecScale(3, (vary[varyIndexZ] + 1.0) / 2.0, finalRGB, scaledFinalRGB);
    vecScale(3, 1.0 - (vary[varyIndexZ] + 1.0) / 2.0, fog, scaledFog);
    vecAdd(3, scaledFinalRGB, scaledFog, tempFinalFog);

    rgbz[0] = tempFinalFog[0];
    rgbz[1] = tempFinalFog[1];
    rgbz[2] = tempFinalFog[2];

    //Set the depth buffer (texture z-value)
    rgbz[3] = vary[varyIndexZ];
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
    double world[4];
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), attr, world);

    double normal[4] = {attr[renATTRN], attr[renATTRO], attr[renATTRP], 0.0};
    double newNormal[4];
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), normal, newNormal);

    vary[varyIndexWorldX] = world[varyIndexX];
    vary[varyIndexWorldY] = world[varyIndexY];
    vary[varyIndexWorldZ] = world[varyIndexZ];
    vary[varyIndexN] = newNormal[0];
    vary[varyIndexO] = newNormal[1];
    vary[varyIndexP] = newNormal[2];

    //Multiply the vector by the viewing matrix in the renderer to place the camera
    mat441Multiply(ren->viewing, world, vary);

    //Remove the placeHolder '1' from the final vector
    attr[renPLACEHOLDER] = stored;

    //Set the remaining attributes for vary to be the same as attr.
    vary[varyIndexS] = attr[renATTRS];
    vary[varyIndexT] = attr[renATTRT];
}

/* If unifParent is NULL, then sets the uniform matrix to the
rotation-translation M described by the other uniforms. If unifParent is not
NULL, but instead contains a rotation-translation P, then sets the uniform
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
	double axis[3] = {0.0, 0.0, 1.0};
	vec3Spherical(1.0, unif[renUNIFPHI], unif[renUNIFTHETA], axis);
	double rot[3][3];
	mat33AngleAxisRotation(unif[renUNIFALPHA], axis, rot);
	double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY],
		unif[renUNIFTRANSZ]};

	unif[renLIGHTPOSX] = 100.0;
    unif[renLIGHTPOSY] = 100.0;
    unif[renLIGHTPOSZ] = 30.0;
    unif[renCAMERAPOSX] = 0.0;
    unif[renCAMERAPOSY] = 0.0;
    unif[renCAMERAPOSZ] = 100.0;
	if (unifParent == NULL)
		mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));
	else {
		double m[4][4];
		mat44Isometry(rot, trans, m);
		mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m,
			(double(*)[4])(&unif[renUNIFISOMETRY]));
	}
	vecCopy(16, (double *)(ren->viewing), &unif[renUNIFVIEWING]);
}

int main(void) {
	texHoriz = &textureHoriz;
	texVert = &textureVert;
	texWater = &textureWater;
	texTrunk = &textureTrunk;
	texTree = &textureTree;
	ren.unifDim = 3 + 3 + 3 + 16 + 16 + 3 + 3 + 3;
	ren.attrDim = 3 + 2 + 3;
	ren.varyDim = 4 + 2 + 3 + 3;
	ren.texNum = 1;
	ren.transformVertex = transformVertex;
	ren.colorPixel = colorPixel;
	ren.updateUniform = updateUniform;
	ren.depth = &depthBuf;
	double zs[12][12] = {
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0},
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0},
		{5.0, 5.0, 10.0, 12.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0},
		{5.0, 5.0, 10.0, 10.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0, 27.0},
		{0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 25.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 25.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0},
		{5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 20.0},
		{10.0, 10.0, 5.0, 5.0, 0.0, 0.0, 0.0, 5.0, 10.0, 15.0, 20.0, 25.0}};
	double ws[12][12] = {
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
	if (pixInitialize(512, 512, "Pixel Graphics") != 0)
		return 1;
	else if (depthInitialize(&depthBuf, 512, 512) != 0)
		return 2;
	else if (texInitializeFile(&textureHoriz, "grass.jpg") != 0)
		return 3;
	else if (texInitializeFile(&textureVert, "granite.jpeg") != 0)
		return 4;
	else if (texInitializeFile(&textureWater, "water.jpg") != 0)
		return 4;
	else if (texInitializeFile(&textureTrunk, "trunk.jpg") != 0)
		return 4;
	else if (texInitializeFile(&textureTree, "tree.jpg") != 0)
		return 4;
	else if (meshInitializeLandscape(&meshLand, 12, 12, 5.0, (double *)zs) != 0)
		return 5;
	else if (meshInitializeDissectedLandscape(&meshHoriz, &meshLand, M_PI / 3.0, 1) != 0)
		return 6;
	else if (meshInitializeDissectedLandscape(&meshVert, &meshLand, M_PI / 3.0, 0) != 0)
		return 6;
	else if (meshInitializeLandscape(&meshWater, 12, 12, 5.0, (double *)ws) != 0)
		return 7;
	else if (meshInitializeSphere(&meshTree, 5.0, 4, 8) != 0)
		return 7;
	else if (meshInitializeCapsule(&meshTrunk, 1.0, 10.0, 1, 8) != 0)
		return 7;
	else if (sceneInitialize(&nodeTree, &ren, unifC, &texTree, &meshTree, NULL, NULL) != 0)
		return 5;
	else if (sceneInitialize(&nodeTrunk, &ren, unifB, &texTrunk, &meshTrunk, &nodeTree, NULL) != 0)
		return 5;
	else if (sceneInitialize(&nodeWater, &ren, unifA, &texWater, &meshWater, NULL, &nodeTrunk) != 0)
		return 5;
	else if (sceneInitialize(&nodeVert, &ren, unifA, &texVert, &meshVert, NULL, &nodeWater) != 0)
		return 7;
	else if (sceneInitialize(&nodeHoriz, &ren, unifA, &texHoriz, &meshHoriz, &nodeVert, NULL) != 0)
		return 8;
	else {
		meshDestroy(&meshLand);
		int i;
		double *vert, normal[2];
		for (i = 0; i < meshVert.vertNum; i += 1) {
			vert = meshGetVertexPointer(&meshVert, i);
			normal[0] = -vert[renATTRO];
			normal[1] = vert[renATTRN];
			vert[renATTRS] = (vert[renATTRX] * normal[0] + vert[renATTRY] * normal[1]) / 20.0;
			vert[renATTRT] = vert[renATTRZ] / 20.0;
		}
		textureHoriz.leftRight = texREPEAT;
		textureHoriz.topBottom = texREPEAT;
		textureVert.leftRight = texREPEAT;
		textureVert.topBottom = texREPEAT;
		textureWater.leftRight = texREPEAT;
		textureWater.topBottom = texREPEAT;
		renSetFrustum(&ren, renORTHOGRAPHIC, M_PI / 3.0, cameraRho, 10.0);
		draw();
		pixSetKeyUpHandler(handleKeyUp);
		pixSetTimeStepHandler(handleTimeStep);
		pixRun();
		meshDestroy(&meshTrunk);
		meshDestroy(&meshTree);
		meshDestroy(&meshHoriz);
		meshDestroy(&meshVert);
		meshDestroy(&meshWater);
		texDestroy(&textureTrunk);
		texDestroy(&textureTree);
		texDestroy(&textureHoriz);
		texDestroy(&textureVert);
		texDestroy(&textureWater);
		sceneDestroyRecursively(&nodeHoriz);
		depthDestroy(&depthBuf);
		return 0;
	}
}
