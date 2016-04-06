
// Includes
//{{{
#ifndef EMSCRIPTEN
	// Native
	#include <GL/glew.h>
	#include <GLES2/gl2.h>
	#include <SDL2/SDL.h>
#else
	// Web
	#include <stdlib.h>
	#include <SDL.h>
	#include <GLES2/gl2.h>
	#include <emscripten.h>
#endif
//}}}

// Variables
//{{{
#ifndef EMSCRIPTEN
	// Native
	SDL_Window* window;
	SDL_GLContext glcontext;
#else
	// Web
	SDL_Surface* screen;
#endif

// Proj matrix properties
int windowWidth;
int windowHeight;
float windowWidthRatio;
float windowHeightRatio;
//}}}


// Create Window
//{{{
void createWindow()
{
	// NOTE FLAG this is suuuuper temporary, just works atm, organize better
	//  and put into various functions and shit where necessary

	// FLAG find link to that tut i used and cite it here until rewrite
	// FLAG look at all flags, leave those unaddressed flagged as "TODO"


	// Init Video
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		//debug_errormessage("Unable to initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}


	// Create Window, and setup OpenGL
#ifndef EMSCRIPTEN
	{
		// Set window dimensions
		SDL_DisplayMode dm;
		if(SDL_GetDesktopDisplayMode(0,&dm))
		{
			//debug_errormessage("SDL_GetDesktopDisplayMode failed: %s",SDL_GetError());
			exit(1);
		}
		windowWidth = dm.w;
		windowHeight = dm.h;

		// temp code for resizable
		int w = 600;
		int h = 400;
		windowWidth = w;
		windowHeight = h;
		//FLAGTE clean this up and consider globals.c
		if(windowWidth>windowHeight)
		{
			windowWidthRatio = ((float)windowWidth)/windowHeight;
			windowHeightRatio = 1.0f;
		}
		else
		{
			windowHeightRatio = ((float)windowHeight)/windowWidth;
			windowWidthRatio = 1.0f;
		}
	}
	window = SDL_CreateWindow(
			"Test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
			//SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
			//SDL_WINDOW_OPENGL
			);
	glcontext = SDL_GL_CreateContext(window);
	if(!glcontext)
	{
		//debug_errormessage("GL Context creation failed.\n");
		exit(1);
	}
	SDL_GL_MakeCurrent(window, glcontext);

	// Set to use OpenGL 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Init Glew
	{
		glewExperimental = GL_TRUE;
		GLenum g = glewInit();
		if(g)
		{
			//debug_errormessage("GLEW init failure: %s\n", glewGetErrorString(g));
			exit(1);
		}
	}

	// Set swap interval
	//  (Try for late swap tearing first, if no-go then use synchronized updates)
	if(SDL_GL_SetSwapInterval(-1)==-1) SDL_GL_SetSwapInterval(1);
#else
	if(!(screen = SDL_SetVideoMode(windowWidth,windowHeight,16,SDL_OPENGL | SDL_RESIZABLE)))
	{
		//debug_errormessage("SDL SetVideoMode failure: ");
		exit(1);
	}
#endif
	
	// Hide Cursor
	SDL_ShowCursor(SDL_DISABLE);

}

void (*createWindowPtr)() = createWindow;
//}}}

// Swap Buffers
//{{{
void swapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void (*swapBuffersPtr)(void) = swapBuffers;
//}}}

