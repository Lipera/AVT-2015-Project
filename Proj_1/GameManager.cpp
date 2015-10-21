#include "GameManager.h"


//---------------------------------------------variables--------------------------------------------

float orangesPos[3] = {-14.0f, 6.0f, 14.0f};

bool keyO = false, keyP = false, keyQ = false, keyA = false;

int oldTimeSinceStart;
int time = 0;

// Camera
// Camera Position
float camX, camY, camZ;
float carX = -10.0f, carY = 0.0f, carZ = -11.75f;
float carAlpha = 0.0;
float carAlphaVar = 0.0;
float carVelAct = 0.0f;
float carVelInc = 0.0f;
float carVelMax = 0.01f;
// Camera Selector
int camS = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 25.0f;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

struct MyMesh mesh[10];


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

VSShaderLib shader;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
	
unsigned int FrameCount = 0;

//window size
int _WindowHandle = 0;
int _WinX = 640, _WinY = 480;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
float lightPos[4] = {0.0f, 20.0f, 0.0f, 1.0f};

//Movement variables

//car movement
float carVec[4] = {1.0f, 0.0f, 0.0f, 0.0f};

//orange Position
float orangeX = 6.0f, orangeY = 2.0f, orangeZ = 15.5f;
float orangeVel = 0.005f;
float orangeVec[4] = {0.0f, 0.0f, -1.0f, 0.0f};

//--------------------------------------Constructor and Destructor--------------------------------------

GameManager::GameManager(){
    //to do

}

GameManager::~GameManager(){
	//to do
}

//--------------------------------------aux function-----------------------------------

int GameManager::random(int m) {
	 return rand()%m;
 }

// -----------------------------------Gets and Sets-------------------------------------
void GameManager::setWinX(int WinX){
	_WinX = WinX;
}

void GameManager::setWinY(int WinY){
	_WinY = WinY;
}

void GameManager::setWindowHandle(int WindowHandle){
	_WindowHandle = WindowHandle;
}

int GameManager::getWinX(){
	return _WinX;
}

int GameManager::getWinY(){
	return _WinY;
}

int GameManager::getWindowHandle(){
	return _WindowHandle;
}

// -----------------------------Reshape Callback Function-------------------------------

void GameManager::reshape(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	
	if(camS != 0){
		perspective(53.13f, ratio, 0.1f, 1000.0f);
	} 
	else {
		float right = 20.0f;
		float left = -20.0f;
		float top = 20.0f;
		float bottom = -20.0f;
		float ratio = (right - left) / (top - bottom);
		float aspect = (float) glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);

		if (ratio < aspect) {
			float delta = ((top - bottom) * aspect - (right - left)) / 2;
			ortho(left - delta, right + delta, bottom, top, -20.0f, 40.0f);
		}
    
		else {
			float delta = ((right - left) / aspect - (top - bottom)) / 2;
			ortho(left, right, bottom - delta, top + delta, -20.0f, 40.0f);
		}

	}
	
}

// -------------------------------------Timer--------------------------------------------

void GameManager::timer(int value){

	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << _WinX << "x" << _WinY << ")";
	std::string s = oss.str();
	glutSetWindow(_WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;

	int deltaTime;
    
    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;

	time += deltaTime;
	//printf("%i\n",deltaTime);
	if(orangeVel <= 0.02){
		orangeVel += 0.00001f;
	}
	orangeZ -= (deltaTime * orangeVel);
	if(orangeZ <= -16.5f){
		orangeZ= 16.5f;
		orangeX= orangesPos[random(3)];
	}
	
	carAlpha -= carAlphaVar;
	if(carAlpha >= 360) {
		carAlpha = 0.0f;
	} else if(carAlpha < 0) {
		carAlpha = 360.0f;
	}

	if(keyQ && carVelAct < carVelMax) {
		carVelAct += carVelInc;
	} else if(keyA && carVelAct > -carVelMax) {
		carVelAct += carVelInc;
	} else if(!keyQ && ! keyA){
		if(carVelInc < 0 && carVelAct > 0) {
			carVelAct += carVelInc;
		} else if (carVelInc > 0 && carVelAct < 0) {
			carVelAct += carVelInc;
		} else {
			carVelAct = 0.0f;
		}
	}
	/*if(0 <carVelAct || carVelAct <carVelMax) {
		carVelAct += carVelInc;
	}*/

	carX += deltaTime * ( carVelAct * cos(-carAlpha * 3.14f / 180.0f));
	carZ += deltaTime * ( carVelAct * sin(-carAlpha * 3.14f / 180.0f));
	
}

// -------------------------------------Refresh--------------------------------------------

void GameManager::refresh(int value){
	glutPostRedisplay();
}

// -----------------------------------------------------------------------------------------
// Events from the Keyboard
//------------------------------------------------------------------------------------------

void GameManager::processKeys(unsigned char key, int xx, int yy){
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;

		case 'c': 
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
			break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;

		//case '1': camS = 0; printf("Orthogonal Camera\n"); _cameras[camS]->computeProjectionMatrix(); break;
		//case '2': camS = 1; printf("Perspective Camera without mouse\n"); _cameras[camS]->computeProjectionMatrix(); alpha = 180.0; beta =46.0; r = 37.0; break;
		//case '3': camS = 2; printf("Perspective Camera with mouse\n"); _cameras[camS]->computeProjectionMatrix(); break;
		case '1':	camS = 0; 
					r=25.0f; 
					printf("Orthogonal Camera\n"); 
					reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					break;
		case '2':	camS = 1; 
					printf("Perspective Camera without mouse\n"); 
					camX = -28.0f;
					camY = 27.0f; 
					camZ = 0.0f;
					reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					break;
		case '3':	camS = 2; 
					printf("Perspective Camera with mouse\n"); 
					reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					break;

		case 'O':
        case 'o': // mover o carro para a esquerda
			keyO = true;
			carAlphaVar = -3.0f;
            break;
        case 'P':
        case 'p': // mover o carro para a direita
			keyP = true;
			carAlphaVar = 3.0f;
            break;
        case 'Q':
        case 'q': // mover o carro para cima
			keyQ = true;
			carVelInc = 0.00075f;
            break;
        case 'A':
        case 'a': // mover o carro para baixo
			keyA = true;
			carVelInc = -0.00075f;
            break;
	}
}

void GameManager::processKeysUp(unsigned char key, int xx, int yy){
    
    switch (key) {
        case 'O':
        case 'o': 
			keyO = false;
			carAlphaVar = 0.0f;
			if(keyP) {
				carAlphaVar = 3.0f;
			}
            break;
        case 'P':
        case 'p':
            keyP = false;
			carAlphaVar = 0.0f;
			if(keyO) {
				carAlphaVar = -3.0f;
			}
            break;
        case 'Q':
        case 'q':
            keyQ = false;
			carVelInc = -0.0005f;
            break;
        case 'A':
        case 'a':
            keyA = false;
			carVelInc = 0.0005f;
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------------------
// Mouse Events
//-----------------------------------------------------------------------------------------

void GameManager::processMouseButtons(int button, int state, int xx, int yy){
	if(camS == 2){
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}
}

// -------------------------Track mouse motion while buttons are pressed-------------------------

void GameManager::processMouseMotion(int xx, int yy){
	if(camS == 2) {
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);
	}
//  uncomment this if not using an idle func
//	glutPostRedisplay();
}


void GameManager::mouseWheel(int wheel, int direction, int x, int y) {
	if(camS == 2) {
	//velocidade do zoom
	r += direction * 0.75f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);
	}
//  uncomment this if not using an idle func
//	glutPostRedisplay();
}


// ----------------------------------------------------------------------------------------
// Render stuff
//----------------------------------------------------------------------------------------

void GameManager::renderScene(void) {

	GLint loc;

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt

	if(camS == 0){
		lookAt(0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
	}else if(camS == 1){
		lookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}else if(camS == 2){
		lookAt(camX, camY, camZ, carX, carY, carZ, 0.0, 1.0, 0.0);
	}

	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates
		//luz como capacete de mineiro, que acompanha o movimento
		//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

		//luz esta sempre no mesmo ponto

		float res[4];
		multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
		glUniform4fv(lPos_uniformId, 1, res);

	//board---------------------------------------------------------------------------------------
		
	for (int i = 0 ; i < 31; ++i) {
		for (int j = 0; j < 31; ++j) {
			if((i%2==0 && j%2==0) || (i%2!=0 && j%2!=0)){objId=0;}
			else{objId=1;}
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, -15.5f + i*1.0f, -1.0f, -15.5f + j*1.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
			objId++;
		}
	}

//orange---------------------------------------------------------------------------------------
		objId=2;
		int aux;
		for(aux = 0; aux < 2; ++aux){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1,mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1,mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1,mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux%2==0){
				translate(MODEL, 1.0f, 2.0f, -1.0f);
			}
			else{
				translate(MODEL, orangeX, orangeY, orangeZ);
			}

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
//table---------------------------------------------------------------------------------------
			objId=3;
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, -16.0f, -4.0f, -16.0f);
			scale(MODEL, 32.0f, 3.0f, 32.0f);


			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);

//butter---------------------------------------------------------------------------------------

		objId=4;
		int aux2;
		for(aux2 = 0; aux2 < 4; ++aux2){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux2%2==0){
				translate(MODEL, -5.0f + aux2 * 2.0f, 0.0f, -7.0f + aux2 * 5.0f);
				rotate(MODEL, 10.0, 0.0, 1.0, 0.0);
			}
			else{
				translate(MODEL, -12.0f + aux2 * 1.0f, 0.0f, 5.0f + aux2 * 2.0f);
				rotate(MODEL, -10.0, 0.0, 1.0, 0.0);
			}
			scale(MODEL, 4.0f, 1.5f, 3.0f);


			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

//track---------------------------------------------------------------------------------------
			
		objId=5;
		int aux3;
		for(aux3=0; aux3 < 30; ++aux3){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux3<=16){
				translate(MODEL, -11.0f + aux3 * 1.5f, 0.2f, -13.0f);
			}else if (16 < aux3 && aux3 <= 30) {
				translate(MODEL, -33.5f + aux3 * 1.5f, 0.2f, -10.0f);
			} 

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

		int aux4;
		for(aux4=0; aux4 < 30; ++aux4){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux4<=16){
				translate(MODEL, 13.0f, 0.2f, -11.5f + aux4 * 1.5f);
			}else if (16 < aux4 && aux4 <= 30) {
				translate(MODEL, 10.0f, 0.2f, -34.0f + aux4 * 1.5f);
			} 

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

		int aux5;
		for(aux5=0; aux5 < 15; ++aux5){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux5<=8){
				translate(MODEL, 0.5f + aux5 * 1.5f, 0.2f, 14.0f);
			}else if (8 < aux5 && aux5 <= 14) {
				translate(MODEL, -11.0f + aux5 * 1.5f, 0.2f, 11.0f);
			} 

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

		int aux6;
		for(aux6=0; aux6 < 17; ++aux6){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux6<=9){
				translate(MODEL, -12.5f, 0.2f, -13.0f + aux6 * 1.5f);
			}else if (9 < aux6 && aux6 <= 16) {
				translate(MODEL, -9.25f, 0.2f, -24.75f + aux6 * 1.5f);
			} 

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

		int aux7;
		for(aux7=0; aux7 < 24; ++aux7){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux7<=12){
				translate(MODEL, -12.5f + aux7 * 1.0f, 0.2f, 1.5f + aux7 * 1.0f);
			}else if (12 < aux7 && aux7 <= 23) {
				translate(MODEL, -21.5f + aux7 * 1.0f, 0.2f, -12.75f + aux7 * 1.0f);
			} 

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

//car-------------------------------------------------------------------------------------
		//wheels--------------------------------------------------------------------------
		objId = 6;
		int aux8;
		for(aux8=0; aux8 < 4; ++aux8){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux8==0){
				translate(MODEL, 0.75f, 0.5f, 0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			}else if (aux8==1) {
				translate(MODEL, -0.75f, 0.5f, 0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} else if (aux8==2) {
				translate(MODEL, 0.75f, 0.5f, -0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} else  {
				translate(MODEL, -0.75f, 0.5f, -0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} 
			translate(MODEL, 0.0f, 0.35f, 0.0f);
			translate(MODEL, carX, carZ, carY);
			//rotate(MODEL, -carAlpha, 1.0, 0.0, 1.0);
			//translate(MODEL, -10.0f, -11.5f, 0.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

		//base block car---------------------------
		objId = 7;
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			//translate(MODEL, -10.0f, 0.0f, -11.50f);
			translate(MODEL, carX, carY, carZ);
			translate(MODEL, -1.5f, 0.5f, 0.0f);
			//rotate(MODEL, carAlpha, 0.0, 1.0, 0.0);
			scale(MODEL, 3.0f, 0.50f, 0.75f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);

		//top block car-----------------------------
		objId = 8;
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			//translate(MODEL, -10.0f, 0.0f, -11.50f);
			translate(MODEL, carX, carY, carZ);
			translate(MODEL, -1.0f, 1.0f, 0.0f);
			scale(MODEL, 1.5f, 0.50f, 0.75f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		
			//spindles--------------------------------------------------------------------------
		objId = 9;
		int aux9;
		for(aux9=0; aux9 < 2; ++aux9){
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			if(aux9==0){
				translate(MODEL, 0.75f, 0.5f, 0.35f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			}else if (aux9==1) {
				translate(MODEL, -0.75f, 0.5f, 0.35f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} 
			//translate(MODEL, -10.0f, -11.5f, 0.0f);
			translate(MODEL, carX, carZ, carY);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}

	glutSwapBuffers();
}


// ----------------------------------------------------------------------------------------
// Shader Stuff
//---------------------------------------------------------------------------------------

GLuint GameManager::setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramValid());
}


// ----------------------------------------------------------------------------------------
// Model loading and OpenGL setup
//----------------------------------------------------------------------------------------

// ------------------------------Init Function------------------------------

void GameManager::init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	//blue
	float amb[]= {0.1f, 0.1f, 0.1f, 1.0f};
	float diff[] = {0.2f, 0.7f, 0.9f, 1.0f};
	float spec[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float emissive[] = {0.3f, 0.8f, 0.4f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;
	
	//white
	float amb1[]= {0.1f, 0.1f, 0.1f, 1.0f};
	float diff1[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float spec1[] = {1.0f, 1.0f, 1.0f, 1.0f};
	shininess=500.0;

	//orange
	float amb2[]= {0.2f, 0.2f, 0.2f, 1.0f};
	float diff2[] = {1.0f, 0.43f, 0.09f, 1.0f};
	float spec2[] = {0.633f, 0.728f, 0.633f, 1.0f};

	//brown
	float amb3[]= {0.1f, 0.1f, 0.1f, 1.0f};
	float diff3[] = {0.9f, 0.6f, 0.4f, 1.0f};
	float spec3[] = {1.0f, 0.4f, 0.2f, 1.0f};

	//yellow
	float amb4[]= {0.0f, 0.0f, 0.0f, 1.0f};
	float diff4[] = {1.0f, 0.8f, 0.1f, 1.0f};
	float spec4[] = {0.9f, 0.7f, 0.3f, 1.0f};

	//cereals
	float amb5[]= {0.02f, 0.02f, 0.02f, 1.0f};
	float diff5[] = {0.95f, 0.84f, 0.65f, 1.0f};
	float spec5[] = {0.71f, 0.66f, 0.73f, 1.0f};

	//wheels color (black rubber)
	float amb6[]= {0.02f, 0.02f, 0.02f, 1.0f};
	float diff6[] = {0.01f, 0.01f, 0.01f, 1.0f};
	float spec6[] = {0.4f, 0.4f, 0.4f, 1.0f};

	//base block car color (red plastic)
	float amb7[]= {0.3f, 0.1f, 0.1f, 1.0f};
	float diff7[] = {0.5f, 0.0f, 0.0f, 1.0f};
	float spec7[] = {0.7f, 0.6f, 0.6f, 1.0f};

	//top block car color (blue glass)
	float amb8[]= {0.0f, 0.3f, 0.4f, 1.0f};
	float diff8[] = {0.07f, 0.68f, 0.89f, 1.0f};
	float spec8[] = {0.6f, 0.45f, 0.6f, 1.0f};

	//spindles color 
	float amb9[]= {0.3f, 0.3f, 0.3f, 1.0f};
	float diff9[] = {0.5f, 0.5f, 0.5f, 1.0f};
	float spec9[] = {0.55f, 0.55f, 0.55f, 1.0f};

	
	//ctabuleiro

	//blue cube of board
		objId=0;
		memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

	//white cube of board
		objId=1;
		memcpy(mesh[objId].mat.ambient, amb1,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff1,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec1,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();	

	//orange
		objId=2;
		memcpy(mesh[objId].mat.ambient, amb2,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff2,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec2,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createSphere(2.0f, 10);

	//table
		objId=3;
		memcpy(mesh[objId].mat.ambient, amb3,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff3,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec3,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

	//butter
		objId=4;
		memcpy(mesh[objId].mat.ambient, amb4,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff4,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec4,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

	//track
		objId=5;
		memcpy(mesh[objId].mat.ambient, amb5,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff5,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec5,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(0.1f, 0.5f, 20, 20);

	//car
		//wheel
		objId=6;
		memcpy(mesh[objId].mat.ambient, amb6,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff6,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec6,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(0.1f, 0.5f, 20, 20);

		//base block
		objId=7;
		memcpy(mesh[objId].mat.ambient, amb7,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

		//top block
		objId=8;
		memcpy(mesh[objId].mat.ambient, amb8,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff8,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec8,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

		//spindle
		objId=9;
		memcpy(mesh[objId].mat.ambient, amb9,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff9,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec9,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCylinder(1.25f, 0.1f, 10);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.529f, 0.808f,0.922f, 1.0f);

}