#!/usr/bin/env python

import gen as g

PI = 3.141592653589793


def main():
	g.init()
	g.setCamera(0,0,0, PI/2.0,-PI/6.0,0, 30);

	x = 0.0;
	y = 0.0;
	vel = 0.1;

	testmodel = g.loadModel("../models/test.csmf");
			

	while True:

		g.inputUpdate()

		if g.isKeyDown(g.KEY_UP):
			y+= vel
		if g.isKeyDown(g.KEY_DOWN):
			y-= vel
		if g.isKeyDown(g.KEY_LEFT):
			x-= vel
		if g.isKeyDown(g.KEY_RIGHT):
			x+= vel
		if g.isKeyDown(g.KEY_ESCAPE):
			exit()


		g.resetScreen()

		g.drawGrid()
		g.drawModel(testmodel, x, y, 0.0, 0.0, 0x0000ff);

		g.swapBuffers()
		g.waitForNextFrame()

if __name__ == "__main__":
	main()



