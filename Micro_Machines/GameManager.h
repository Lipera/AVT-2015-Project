//-----------------------------------includes---------------------------------

#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>

//includes of files .h from object classes of the game
#include "Butter.h"
#include "Board.h"
#include "Car.h"
#include "Camera.h"
#include "OrthogonalCamera.h"
#include "PerspectiveCamera.h"
#include "GameObject.h"
#include "Orange.h"
#include "Track.h"
#include "Table.h"

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

//-----------------------------------defines--------------------------------
#define CAPTION "AVT Light Demo"

//---------------------------------------------------------------------------

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {

private:
	int _WinX;
	int _WinY;
	int _WindowHandle;
	//std::vector<Camera*> _cameras; Camera* _cameras;

public:
    GameManager();
    ~GameManager();

	void setWinX(int WinX);
	void setWinY(int WinY);
	void setWindowHandle(int WindowHandle);
	int getWinX();
	int getWinY();
	int getWindowHandle();
	int random(int m);
	void reshape(int w, int h);
	void refresh(int value);
	void processKeys(unsigned char key, int xx, int yy);
	void processKeysUp(unsigned char key, int xx, int yy);
	void processMouseButtons(int button, int state, int xx, int yy);
	void processMouseMotion(int xx, int yy);
	void mouseWheel(int wheel, int direction, int x, int y);
	void renderScene();
    void init();
	void timer(int value);
	GLuint setupShaders();
};

#endif