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
#include "Table.h"
#include "Candle.h"
#include "Pause.h"
#include "GameOver.h"
#include "Cheerios.h"
#include "LightSource.h"
#include "Billboard.h"
#include "Glass.h"
#include "Coaster.h"
#include "Juice.h"

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"

#include "TGA.h"
//#include "Fonts.h"

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

//-----------------------------------defines--------------------------------
#define CAPTION "MicroMachine"
#define FPS 60
//---------------------------------------------------------------------------

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {

private:
	int _WinX;
	int _WinY;
	int _WindowHandle;
	bool _isFogActive;
	std::vector<GameObject*> _gameObject;
	std::vector<Camera*> _cameras;
	std::vector<GameObject*> _lives;
	std::vector<GameObject*> _track;
	std::vector<LightSource*> _lights;

public:
    GameManager();
    ~GameManager();

	void renderSpacedBitmapString(float x, float y, int spacing, void *font, char *string);
	void setWinX(int WinX);
	void setWinY(int WinY);
	void setWindowHandle(int WindowHandle);
	int getWinX();
	int getWinY();
	int getWindowHandle();
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
	bool collision(GameObject *obj1, GameObject *obj2, Vector3 *obj1_position, Vector3 *obj2_position);
	void track();
	void reorganizeGame();

};

#endif