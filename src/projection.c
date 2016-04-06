
// Includes
//{{{
#include "matrix.h"
#include "window.h"
//}}}

// Variables
//{{{
static float clipdistancenear = 1.0f;// FLAG make arguments to function, remove these
static float clipdistancefar = 100.0f;
static float viewangletangent = 0.25f;
//}}}


// Generate Projection Matrix
//{{{
static void generateProjectionMatrix(matrix* mat)
{
	float* c = mat->cell;

	float near;		// distance of inner clipping plane
	float far;		// distance of outter clipping plane
	float tangent;		// tangent of view angle
	float widthratio;	// ratio of width to proper square screen
	float heightratio;	// ratio of height to proper square screen

	// "tangent" gives how wide a square screen would be if the screen
	//  were 1 unit of distance from the eye.
	// "widthratio" and "heightratio" give how wide or high the screen
	//  is in relation to the above described square screen.
	// Either widthratio or heighratio will be equal to 1, depending
	//  on the dimensions of the screen.
	
	near = clipdistancenear;
	far = clipdistancefar;
	tangent = viewangletangent;

	c[0]  = 1.0f/tangent;
	c[1]  = 0.0f;
	c[2]  = 0.0f;
	c[3]  = 0.0f;
	c[4]  = 0.0f;
	c[5]  = 1.0f/tangent;
	c[6]  = 0.0f;
	c[7]  = 0.0f;
	c[8]  = 0.0f;
	c[9]  = 0.0f;
	c[10] = (far+near)/(near-far);
	c[11] = -1.0f;
	c[12] = 0.0f;
	c[13] = 0.0f;
	c[14] = 2.0f*far*near/(near-far);
	c[15] = 0.0f;
}

void (*generateProjectionMatrixPtr)(matrix*) = generateProjectionMatrix;
//}}}

