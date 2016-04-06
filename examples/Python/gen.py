import ctypes

g = ctypes.CDLL("./libgendyn.so")

def init():
	g.init()

def setCamera(x,y,z,theta,phi,psi,distance):
	g.setCamera(ctypes.c_float(x),ctypes.c_float(y),ctypes.c_float(z), ctypes.c_float(theta),ctypes.c_float(phi),ctypes.c_float(psi), ctypes.c_float(distance))

def inputUpdate():
	g.inputUpdate()

def isKeyDown(k):
	return g.isKeyDown(ctypes.c_int(k))

def resetScreen():
	g.resetScreen()

def drawGrid():
	g.drawGrid()

def drawBox(x,y,z, widthx,widthy,height, color):
	g.drawBox(ctypes.c_float(x),ctypes.c_float(y),ctypes.c_float(z), ctypes.c_float(widthx),ctypes.c_float(widthy),ctypes.c_float(height), ctypes.c_int(color))

def swapBuffers():
	g.swapBuffers()

def waitForNextFrame():
	g.waitForNextFrame()

def loadModel(filename):
	return g.loadModel(ctypes.c_char_p(filename))

def drawModel(model, x, y, z, angle, color):
	g.drawModel(ctypes.c_voidp(model), ctypes.c_float(x),ctypes.c_float(y),ctypes.c_float(z), ctypes.c_float(angle), ctypes.c_int(color))

KEY_A=97
KEY_B=98
KEY_C=99
KEY_D=100
KEY_E=101
KEY_F=102
KEY_G=103
KEY_H=104
KEY_I=105
KEY_J=106
KEY_K=107
KEY_L=108
KEY_M=109
KEY_N=110
KEY_O=111
KEY_P=112
KEY_Q=113
KEY_R=114
KEY_S=115
KEY_T=116
KEY_U=117
KEY_V=118
KEY_W=119
KEY_X=120
KEY_Y=121
KEY_Z=122
KEY_0=48
KEY_1=49
KEY_2=50
KEY_3=51
KEY_4=52
KEY_5=53
KEY_6=54
KEY_7=55
KEY_8=56
KEY_9=57
KEY_SPACEBAR=32
KEY_ENTER=13
KEY_RIGHT=128
KEY_LEFT=129
KEY_DOWN=130
KEY_UP=131
KEY_ESCAPE=27
