
// Includes & Window Vars
//{{{
#include <math.h>

#ifndef EMSCRIPTEN
	// Native
	#include <GLES2/gl2.h>
	#include <SDL2/SDL.h>
#else
	// Web
	#include <stdlib.h>
	#include <SDL.h>
	#include <GLES2/gl2.h>
	#include <emscripten.h>
#endif


// FLAG delete me
#include "matrix.h"
#include "window.h"
#include "projection.h" // for resetScreen, delete when in libmain or own file
#include "shader.h"
//}}}

// Variables & Structs
//{{{
// FLAG go through these and determine if needed

typedef struct staticmodel // FLAG make public?
{
	GLuint VBO;
	GLuint IBO;
	GLsizei IBOlen;
} staticmodel;

SDL_Event event;

staticmodel cube;
staticmodel grid;
staticmodel cylinder;

static matrix camMatrix;
static matrix projMatrix;

float camx;
float camy;
float camz;
float camtheta;
float camphi;
float campsi;
float camdistance;
//}}}


// Definitions for loadstatimodel (merge with below, or make seperate files??)
//{{{


// For Load staticmodel

// Bytes in file header
#define FILEHEADLEN 2

// Bytes in header of each object in model
#define OBHEADLEN 22

// Bytes per vertex
#define BPVERT 6

// Bytes per tri
#define BPTRI 6

//}}}

// Load staticmodel function
//{{{ TODO FLAG fixme Make static and use function pointer
//void loadstaticmodel(GLfloat** averts, GLuint** aindis, GLint* avertcount, GLsizei* aindlen)
staticmodel* loadModel(char* filename)
{

	staticmodel* m = (staticmodel*)malloc(sizeof(staticmodel));

	uint8_t* ba;	// byte array
	int balen;	// length of ba
	GLfloat* verts;
	GLuint* indis;
	GLint vertcount;
	GLsizei indlen;
	
	// Copy file in bytearray
	//{{{
	{
		// source: http://stackoverflow.com/questions/22059189/read-a-file-as-byte-array

		FILE* file;
		//char filename[] = "../../verts.test";
		file = fopen(filename,"rb");
		if(file==0)
		{
			printf("Unable to open file \"%s\"\n", filename);
			exit(1);
		}
		fseek(file, 0, SEEK_END);
		balen = ftell(file);
		rewind(file);

		ba = (uint8_t*)malloc(balen);
		fread(ba, balen, 1, file);
		fclose(file);
	}
	//}}}

	// Build staticmodel data (cpuside) based on data in byte array
	//{{{
	{
		int obCount;
		obCount = 256*ba[1] + ba[0];
		if(obCount) obCount = 1; // "staticmodels" are only single objects atm FLAG
		int pi = FILEHEADLEN;  // Parse Index

		int ob;
		for(ob=0; ob<obCount; ob++)
		{
			int vertCount = 256*ba[pi+1] + ba[pi+0];
			int triCount = 256*ba[pi+3] + ba[pi+2];
			// load trans/rot/scale data

			if(pi+OBHEADLEN+vertCount*BPVERT+triCount*BPTRI > balen)
			{
				printf("deerp"); // FLAG
				return 0; // FLAG populate an empty staticmodel and give "invalid file" error
			}

			// Generate Verts
			{

				vertcount = vertCount*3; // FLAG 3 multiple unintuitive, redo
				verts = (GLfloat*)malloc(sizeof(GLfloat)*vertCount*3);
				GLfloat* vd = verts; // vert data pointer
				int v;
				int vstart = pi+OBHEADLEN;
				for(v=0; v<vertCount; v++)
				{
					int bi = vstart + v*BPVERT; // byte array index
					// X component
					vd[v*3+0] = (GLfloat)ba[bi+1] - 128.0f + ((GLfloat)ba[bi+0]/256.0f);
					// Y component
					vd[v*3+1] = (GLfloat)ba[bi+3] - 128.0f + ((GLfloat)ba[bi+2]/256.0f);
					// z component
					vd[v*3+2] = (GLfloat)ba[bi+5] - 128.0f + ((GLfloat)ba[bi+4]/256.0f);
				}
			}

			// Generate Tris
			{
				indlen = triCount*3;
				indis = (GLuint*)malloc(sizeof(GLuint)*triCount*3);
				GLuint* td = indis; // tri data pointer
				int t;
				int tstart = pi+OBHEADLEN+vertCount*BPVERT;
				for(t=0; t<triCount; t++)
				{
					int bi = tstart + t*BPTRI;
					// Index 1
					td[3*t+0] = 256*ba[bi+1] + ba[bi+0];
					// Index 2
					td[3*t+1] = 256*ba[bi+3] + ba[bi+2];
					// Index 3
					td[3*t+2] = 256*ba[bi+5] + ba[bi+4];
				}
			}

			// Update parse index
			pi += OBHEADLEN+vertCount*BPVERT+triCount*BPTRI;

		}
	}
	//}}}

	// Send staticmodel data to gpu
	//{{{
	{
		m->IBOlen = indlen;
		glGenBuffers(1, &(m->VBO)); // Create new pointer to (yet unmade) buffer
		glBindBuffer(GL_ARRAY_BUFFER, m->VBO); // Establish buffer type for pointer
		glBufferData(GL_ARRAY_BUFFER,
				vertcount * sizeof(GLfloat),
				verts,
				GL_STATIC_DRAW); // Create buffer
		glGenBuffers(1, &(m->IBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				m->IBOlen * sizeof(GLuint),
				indis,
				GL_STATIC_DRAW);
	}
	//}}}

	free(ba);

	return m;

}
//}}}


// Init
//{{{
static void rinit()
{
	camx = 0;
	camy = 0;
	camz = 0;
	camtheta = M_PI/2.0f;
	camphi = -M_PI/3.0f;;
	campsi = 0;
	camdistance = 30;

	// Create cube VBO and IBO
	//{{{
	{
		// Data to pass to buffers
		/*
		GLfloat cpuverts[] = {1.0,-1.0,0.0, 0.0,1.0,0.0, -1.0,-1.0,0.0};
		GLint cpuvertcount = 3*3;
		GLuint cpuindis[] = {0,1,2};
		icount = 3;
		*/
		GLfloat cpuverts[] = {  0.0,0.0,0.0,
					0.0,1.0,0.0,
					1.0,1.0,0.0,
					1.0,0.0,0.0,
					0.0,0.0,1.0,
					0.0,1.0,1.0,
					1.0,1.0,1.0,
					1.0,0.0,1.0};
		GLint cpuvertcount = 8*3;
		GLuint cpuindis[] = {	0,1,2,
					0,2,3,
					0,4,5,
					0,5,1,
					1,5,6,
					1,6,2,
					2,6,7,
					2,7,3,
					3,7,4,
					3,4,0,
					6,5,4,
					6,4,7};
		cube.IBOlen = 12*3;


		// Generate buffers
		glGenBuffers(1, &cube.VBO); // Create new pointer to (yet unmade) buffer
		glBindBuffer(GL_ARRAY_BUFFER, cube.VBO); // Establish buffer type for pointer
		glBufferData(GL_ARRAY_BUFFER,
				cpuvertcount * sizeof(GLfloat),
				cpuverts,
				GL_STATIC_DRAW); // Create buffer
		glGenBuffers(1, &cube.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				cube.IBOlen * sizeof(GLuint),
				cpuindis,
				GL_STATIC_DRAW);
	}
	//}}}

	// Create cylinder VBO and IBO
	//{{{
	{
		// Data to pass to buffers
		int divs = 8;
		GLint cpuvertcount = 3* 2*(divs+1);
		GLfloat cpuverts[cpuvertcount];
		cylinder.IBOlen = 3* 4*divs;
		GLuint cpuindis[cylinder.IBOlen];

		cpuverts[0] = 0.0f; cpuverts[1] = 0.0f; cpuverts[2] = 0.0f;
		cpuverts[3] = 0.0f; cpuverts[4] = 0.0f; cpuverts[5] = 1.0f;
		int i;
		for(i=0;i<divs;i++)
		{
			const float pi = 3.14159265359f;
			int v = 6*i+6;
			float c = cos(((float)i)/divs*2.0f*pi);
			float s = sin(((float)i)/divs*2.0f*pi);
			cpuverts[v+0] = c; cpuverts[v+1] = s; cpuverts[v+2] = 0.0f;
			cpuverts[v+3] = c; cpuverts[v+4] = s; cpuverts[v+5] = 1.0f;
		}
		int last = (cpuvertcount)/3;
		cpuindis[0]=0; cpuindis[1]=2; cpuindis[2]=last-2;
		cpuindis[3]=1; cpuindis[4]=last-1; cpuindis[5]=3;
		cpuindis[6] = last-2;
		cpuindis[7] = 2;
		cpuindis[8] = 3;
		cpuindis[9] = last-2;
		cpuindis[10] = 3;
		cpuindis[11] = last-1;
		for(i=0;i<divs-1;i++)
		{
			int t = 12*i+12;
			cpuindis[t+0] = 0;
			cpuindis[t+1] = i*2+4;
			cpuindis[t+2] = i*2+2;

			cpuindis[t+3] = 1;
			cpuindis[t+4] = i*2+3;
			cpuindis[t+5] = i*2+5;

			cpuindis[t+6] = i*2+2;
			cpuindis[t+7] = i*2+4;
			cpuindis[t+8] = i*2+5;
			cpuindis[t+9] = i*2+2;
			cpuindis[t+10]= i*2+5;
			cpuindis[t+11]= i*2+3;
		}


		// Generate buffers
		glGenBuffers(1, &cylinder.VBO); // Create new pointer to (yet unmade) buffer
		glBindBuffer(GL_ARRAY_BUFFER, cylinder.VBO); // Establish buffer type for pointer
		glBufferData(GL_ARRAY_BUFFER,
				cpuvertcount * sizeof(GLfloat),
				cpuverts,
				GL_STATIC_DRAW); // Create buffer
		glGenBuffers(1, &cylinder.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				cylinder.IBOlen * sizeof(GLuint),
				cpuindis,
				GL_STATIC_DRAW);
	}
	//}}}

	// Create grid VBO and IBO
	//{{{
	{
		// Data to pass to buffers
		GLfloat hw = 5.0f; // half width of grid square
		/*
		GLfloat cpuverts[] = {
			 hw, hw, 0.0f,
			-hw, hw, 0.0f,
			-hw,-hw, 0.0f,
			 hw,-hw, 0.0f
		};
		GLint cpuvertcount = 4*3;
		GLuint cpuindis[] = {0,1,2, 0,2,3};
		grid.IBOlen = 2*3;
		//*/
		//*
		GLfloat cpuverts[] = {  -hw,-hw,-1.0,
					-hw,hw,-1.0,
					hw,hw,-1.0,
					hw,-hw,-1.0,
					-hw,-hw,0.0,
					-hw,hw,0.0,
					hw,hw,0.0,
					hw,-hw,0.0};
		GLint cpuvertcount = 8*3;
		GLuint cpuindis[] = {	0,1,2,
					0,2,3,
					0,4,5,
					0,5,1,
					1,5,6,
					1,6,2,
					2,6,7,
					2,7,3,
					3,7,4,
					3,4,0,
					6,5,4,
					6,4,7};
		grid.IBOlen = 12*3;
		//*/

		// Generate buffers
		glGenBuffers(1, &grid.VBO); // Create new pointer to (yet unmade) buffer
		glBindBuffer(GL_ARRAY_BUFFER, grid.VBO); // Establish buffer type for pointer
		glBufferData(GL_ARRAY_BUFFER,
				cpuvertcount * sizeof(GLfloat),
				cpuverts,
				GL_STATIC_DRAW); // Create buffer
		glGenBuffers(1, &grid.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				grid.IBOlen * sizeof(GLuint),
				cpuindis,
				GL_STATIC_DRAW);
	}
	//}}}

	// FLAG moveme
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



}

void (*rinitPtr)(void) = rinit;
//}}}

// Render staticmodel
//{{{
void renderModel(staticmodel* mod, matrix mat, int color)
{

	glUseProgram(objprogram);

	glEnableVertexAttribArray(objvertpointer); // pointer in env prog to incoming verts
	glBindBuffer(GL_ARRAY_BUFFER, mod->VBO);
	glVertexAttribPointer(objvertpointer, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod->IBO);

	glUniformMatrix4fv(objmatrixpointer, 1, 0, mat.cell);
	float red = ((float)((color&0xff0000)>>16))/255.0f;
	float green = ((float)((color&0xff00)>>8))/255.0f;
	float blue = ((float)(color&0xff))/255.0f;
	glUniform3f(objcolorpointer, red, green, blue);

	glDrawElements(GL_TRIANGLES, mod->IBOlen, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(objvertpointer);

}
//}}}

// Draw Model
//{{{
void drawModel(staticmodel* mod, float posx, float posy, float posz, float angle, int color)
{
	matrix mat;
	matrix_identity(&mat);
	matrix_translate(&mat, posx,posy,posz);
	matrix_rotatez(&mat, cos(angle), sin(angle));
	matrix_multiply(&mat,&camMatrix,&mat);
	matrix_multiply(&mat,&projMatrix,&mat);

	renderModel(mod, mat, color);
}
//}}}

// Drawbox
//{{{
void drawBox(float positionx, float positiony, float positionz,
		float widthx, float widthy, float widthz,
		int color)
{
	matrix Tmatrix;
	matrix_identity(&Tmatrix);//cammatrix; // FLAGTE delete once env s
	matrix_translate(&Tmatrix, positionx,positiony,positionz);
	matrix_scale(&Tmatrix, widthx, widthy, widthz);
	matrix_multiply(&Tmatrix,&camMatrix,&Tmatrix);
	matrix_multiply(&Tmatrix,&projMatrix,&Tmatrix);

	renderModel(&cube, Tmatrix, color);

}
//}}}

// Draw cylinder
//{{{
void drawCylinder(float positionx, float positiony, float positionz,
		float widthx, float widthy, float widthz,
		int color)
{
	matrix Tmatrix;
	matrix_identity(&Tmatrix);//cammatrix; // FLAGTE delete once env s
	matrix_translate(&Tmatrix, positionx,positiony,positionz);
	matrix_scale(&Tmatrix, widthx, widthy, widthz);
	matrix_multiply(&Tmatrix,&camMatrix,&Tmatrix);
	matrix_multiply(&Tmatrix,&projMatrix,&Tmatrix);

	renderModel(&cylinder, Tmatrix, color);

}
//}}}

// Draw Grid
//{{{
void drawGrid()
{
	matrix mat;
	matrix_multiply(&mat,&projMatrix,&camMatrix);

	glUseProgram(envprogram);

	glEnableVertexAttribArray(envvertpointer); // pointer in env prog to incoming verts
	glBindBuffer(GL_ARRAY_BUFFER, grid.VBO);
	glVertexAttribPointer(envvertpointer, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid.IBO);

	glUniformMatrix4fv(envmatrixpointer, 1, 0, mat.cell);

	glDrawElements(GL_TRIANGLES, grid.IBOlen, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(envvertpointer);

}
void drawGridBox(float x, float y, float z, float sx, float sy, float sz)
{
	matrix Tmatrix;
	matrix_identity(&Tmatrix);
	matrix_translate(&Tmatrix, x, y, z);
	matrix_scale(&Tmatrix, sx, sy, sz);
	matrix_multiply(&Tmatrix,&camMatrix,&Tmatrix);
	matrix_multiply(&Tmatrix,&projMatrix,&Tmatrix);

	glUseProgram(envprogram);

	glEnableVertexAttribArray(envvertpointer); // pointer in env prog to incoming verts
	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);
	glVertexAttribPointer(envvertpointer, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.IBO);

	glUniformMatrix4fv(envmatrixpointer, 1, 0, Tmatrix.cell);

	glDrawElements(GL_TRIANGLES, cube.IBOlen, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(envvertpointer);

}
//}}}


// FLAG move all below functions
// Clear Screen
//{{{
static void clearScreen()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}
//}}}

// Camera
//{{{
void changeCamera(float x, float y, float z, float theta, float phi, float psi, float distance)
{
	// Has to be called before resetScreen atm, for changes to affect current draw
	camx+= x;
	camy+= y;
	camz+= z;
	camtheta+= theta;
	camphi+= phi;
	campsi+= psi;
	camdistance+= distance;
}
void setCamera(float x, float y, float z, float theta, float phi, float psi, float distance)
{
	// Has to be called before resetScreen atm, for changes to affect current draw
	camx = x;
	camy = y;
	camz = z;
	camtheta = theta;
	camphi = phi;
	campsi = psi;
	camdistance = distance;
}
float getCamTheta()
{
	return camtheta;
}
//}}}

// "Reset" Screen
//{{{
void resetScreen()
{
	clearScreen();
	matrix_identity(&camMatrix);
	matrix_translate(&camMatrix,0,0,-camdistance);
	matrix_rotatez(&camMatrix, cos(campsi), sin(campsi));
	matrix_rotatex(&camMatrix, cos(-M_PI/2.0f-camphi), sin(-M_PI/2.0f-camphi));
	matrix_rotatez(&camMatrix, cos(M_PI/2.0f-camtheta), sin(M_PI/2.0f-camtheta));
	matrix_translate(&camMatrix, -camx, -camy, -camz);
	//printf("%f %f %f  %f %f %f  %f\n",camx,camy,camz,camtheta,camphi,campsi,camdistance);
	generateProjectionMatrixPtr(&projMatrix); // FLAG rename projmatrix somethign else? make ar*proj*cam one global matrix, "base matrix" or something like that??
	{
		matrix arMatrix; // aspect ratio matrix
		matrix_identity(&arMatrix);
		matrix_scale(&arMatrix,1.0f/windowWidthRatio,1.0f/windowHeightRatio,1.0f);
		matrix_multiply(&projMatrix,&arMatrix,&projMatrix);
	}
	// FLAG put in iskeydown check iskeydown
}
//}}}


