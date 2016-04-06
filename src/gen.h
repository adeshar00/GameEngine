
void init();
void setCamera(float,float,float,float,float,float,float);
void changeCamera(float,float,float,float,float,float,float);
float getCamTheta();
void inputUpdate();
void resetScreen();
void drawBox(float positionx, float positiony, float positionz,
		float widthx, float widthy, float widthz,
		int color);
void drawGrid();
void drawGridBox(float, float, float, float, float, float);
void swapBuffers();
int isKeyDown(int keyCode);
int isKeyDownFresh(int keyCode);
void waitForNextFrame();
void drawLine();

typedef struct staticmodel* model;
model loadModel(char*);
void drawModel(model,float,float,float,float,int);

enum
{
KEY_A=97,
KEY_B=98,
KEY_C=99,
KEY_D=100,
KEY_E=101,
KEY_F=102,
KEY_G=103,
KEY_H=104,
KEY_I=105,
KEY_J=106,
KEY_K=107,
KEY_L=108,
KEY_M=109,
KEY_N=110,
KEY_O=111,
KEY_P=112,
KEY_Q=113,
KEY_R=114,
KEY_S=115,
KEY_T=116,
KEY_U=117,
KEY_V=118,
KEY_W=119,
KEY_X=120,
KEY_Y=121,
KEY_Z=122,
KEY_0=48,
KEY_1=49,
KEY_2=50,
KEY_3=51,
KEY_4=52,
KEY_5=53,
KEY_6=54,
KEY_7=55,
KEY_8=56,
KEY_9=57,
KEY_SPACEBAR=32,
KEY_ENTER=13,
KEY_RIGHT=128,
KEY_LEFT=129,
KEY_DOWN=130,
KEY_UP=131,
KEY_ESCAPE=27
};
