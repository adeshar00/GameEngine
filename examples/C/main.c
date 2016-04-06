#include <stdio.h>
#include <math.h>

#include "gen.h"


int main(int argc, char* argv[])
{

	init();

	float x = 0;
	float y = 0;
	float vel = 0.1;

	model testmodel = loadModel("../models/test.csmf");
		
	setCamera(0,0,0, M_PI/2.0f,-M_PI/6.0f,0, 30);

	while(1)
	{

		inputUpdate();

		if(isKeyDown(KEY_UP))
		{
			y+= vel;
		}
		if(isKeyDown(KEY_DOWN))
		{
			y-= vel;
		}
		if(isKeyDown(KEY_LEFT))
		{
			x-= vel;
		}
		if(isKeyDown(KEY_RIGHT))
		{
			x+= vel;
		}
		if(isKeyDown(KEY_ESCAPE))
		{
			return 0;
		}

		resetScreen();

		drawGrid();

		drawModel(testmodel, x, y, 0.0f, 0.0f, 0x0000ff);

		swapBuffers();
		waitForNextFrame();

	}

}


