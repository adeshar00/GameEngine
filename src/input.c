
// Includes
//{{{
#ifndef EMSCRIPTEN
	// Native
	#include <SDL2/SDL.h>
#else
	// Web
	#include <SDL.h>
	#include <emscripten.h>
#endif
//}}}

// Defines & Variables
//{{{

// Total number of keys tracked
#define KEYS 132

// Range in which key values same as SDL (arrow keys map SDL keys to custom values)
#define SDLKEYRANGE 128

static SDL_Event event;
static int keyhold[KEYS];		// Which keys are being held
static int keypress[KEYS];	// Which keys have been newly pressed since last tick
static int leftmousehold;
static int leftmousepress;
static int rightmousehold;
static int rightmousepress;
static int deltamousewheel;

//}}}


// Clear
//{{{
void inputClear()
{
	int i;
	for(i=0;i<KEYS;i++)
	{
		keyhold[i] = 0;
		keypress[i] = 0;
	}

	leftmousepress = 0;
	leftmousehold = 0;
	rightmousepress = 0;
	rightmousehold = 0;
	deltamousewheel = 0;
}
//}}}

// Init
//{{{
static void inputInit()
{
	inputClear();
}

void (*inputInitPtr)(void) = inputInit;
//}}}

// Update
//{{{
void inputUpdate()
{

	// Clear previous keypress data
	{
		int i;
		for(i=0;i<KEYS;i++)
		{
			keypress[i] = 0;
		}

		leftmousepress = 0;
		rightmousepress = 0;
		deltamousewheel = 0;
	}

	// Update input data
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_MOUSEMOTION:
				//cursorx = event.motion.x;
				//cursory = event.motion.y;
			break;

			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						leftmousepress = 1;
						leftmousehold = 1;
					break;

					case SDL_BUTTON_RIGHT:
						rightmousepress = 1;
						rightmousehold = 1;
					break;
				}	
			break;

			case SDL_MOUSEBUTTONUP:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						leftmousehold = 0;
					break;

					case SDL_BUTTON_RIGHT:
						rightmousehold = 0;
					break;
				}	
			break;

			case SDL_MOUSEWHEEL:
				deltamousewheel+= event.wheel.y;
				break;


			case SDL_KEYDOWN:
			{
				int key = event.key.keysym.sym;
				if(key<SDLKEYRANGE)
				{
					keypress[key] = 1;
					keyhold[key] = 1;
				}
				else if(key>=SDLK_RIGHT && key<=SDLK_UP)
				{
					int arrowkey = key - SDLK_RIGHT + SDLKEYRANGE;
					keypress[arrowkey] = 1;
					keyhold[arrowkey] = 1;
				}
				//printf("%d\n",key);
			}
			break;

			case SDL_KEYUP:
			{
				int key = event.key.keysym.sym;
				if(key<SDLKEYRANGE)
				{
					keyhold[key] = 0;
				}
				else if(key>=SDLK_RIGHT && key<=SDLK_UP)
				{
					int arrowkey = key - SDLK_RIGHT + SDLKEYRANGE;
					keyhold[arrowkey] = 0;
				}
			}
			break;

		}
	}

}
//}}}

// Is Key Down
//{{{
int isKeyDown(int key)
{
	if(key<0 || key>=KEYS)
	{
		return 0;
	}

	return keyhold[key];
}
int isKeyDownFresh(int key)
{
	if(key<0 || key>=KEYS)
	{
		return 0;
	}

	return keypress[key];
}
//}}}

