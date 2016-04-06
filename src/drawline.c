
// Includes
//{{{
#ifndef EMSCRIPTEN
	// Native
	#include <GLES2/gl2.h>
#else
	// Web
	#include <stdlib.h>
	#include <GLES2/gl2.h>
	#include <emscripten.h>
#endif

#include "shader.h"
#include "matrix.h"
//}}}

// Variables
//{{{
static GLuint lineVBO;
static GLuint lineIBO;
static GLsizei icount;
//}}}


// Init
//{{{
static void drawLineInit()
{
	// Create VBO and IBO
	{
		GLfloat cpuverts[] = {   0.0f, 0.0f, 0.5f,
								 0.5f, 0.0f, 0.0f,
								 0.5f, 1.0f, 0.0f,
								 0.0f, 1.0f, 0.5f,
								-0.5f, 1.0f, 0.0f,
								-0.5f, 0.0f, 0.0f};
		GLint cpuvertcount = 6*3;
		GLuint cpuindis[] = {	0, 1, 2,
								0, 2, 3,
								0, 3, 4,
								0, 4, 5};
		icount = 4*3;

		// Generate buffers
		glGenBuffers(1, &lineVBO); // Create new pointer to (yet unmade) buffer
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO); // Establish buffer type for pointer
		glBufferData(GL_ARRAY_BUFFER,
				cpuvertcount * sizeof(GLfloat),
				cpuverts,
				GL_STATIC_DRAW); // Create buffer
		glGenBuffers(1, &lineIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				icount * sizeof(GLuint),
				cpuindis,
				GL_STATIC_DRAW);
	}
}

void (*drawLineInitPtr)(void) = drawLineInit;
//}}}

// Draw Line
//{{{
void drawLine() //eventually static, just to be used for texture generation
{
	matrix m;
	matrix_identity(&m);
	matrix_translate(&m, 0.0f, 0.0f, 1.0f);
	matrix_scale(&m, 1.0f, 1.0f, -1.0f);

	glUseProgram(objprogram);

	glEnableVertexAttribArray(objvertpointer); // pointer in env prog to incoming verts
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glVertexAttribPointer(objvertpointer, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIBO);

	glUniformMatrix4fv(objmatrixpointer, 1, 0, m.cell);
	glUniform3f(objcolorpointer, 1.0, 0.0, 0.0);

	glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);


	matrix_rotatez(&m, 0.0, 1.0f);

	glUniformMatrix4fv(objmatrixpointer, 1, 0, m.cell);
	glUniform3f(objcolorpointer, 1.0, 0.0, 0.0);

	glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(envvertpointer);

}
//}}}

