/* 080renderer.c
* Danny Leal 01/13/17
*/

#define renVARYDIMBOUND 16

/* A class meant to contain configuration information for our rendered triangles */
typedef struct renRenderer renRenderer;
struct renRenderer {
    int unifDim;
    int texNum;
    int attrDim;
    int varyDim;
};
