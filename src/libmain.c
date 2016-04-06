#include "model.h"
#include "clock.h"
#include "window.h"
#include "input.h"
#include "shader.h"
#include "drawline.h"

void init()
{
	createWindowPtr();
	rinitPtr();
	inputInitPtr();
	initClockPtr();	// FLAG change these to nameInit instead of InitName
	initShaderPtr();
	drawLineInitPtr();
}

// De-init
/*
static void render_deinit()
{
	glDeleteProgram(envprogram);
	glDeleteProgram(objprogram);
	glDeleteProgram(hudprogram);
	//render_freemodelbuffers(tetratest);

#ifndef EMSCRIPTEN
	SDL_DestroyWindow(window);
#endif
	//SDL_Quit();
}
*/
