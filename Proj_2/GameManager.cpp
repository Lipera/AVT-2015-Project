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

int WindowHandle = 0;
//int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;

struct MyMesh mesh[6];
int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
GLint tex_loc, tex_loc1, tex_loc2, tex_loc3;
GLint texMode_uniformId;

GLuint TextureArray[3];


// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};

//orange Position
float orangeX = 6.0f, orangeY = 2.0f, orangeZ = 15.5f;
float orangeVel = 0.005f;
float orangeVec[4] = {0.0f, 0.0f, -1.0f, 0.0f};

//------------------create object class---------------------

Table* table;
Board* board;
Butter* butter;
Track* track;

//--------------------------------------Constructor and Destructor--------------------------------------

GameManager::GameManager(){
    //to do
	table = new Table();
	_gameObject.push_back(table); //table = 0
	board = new Board();
	_gameObject.push_back(board); //board = 1
	butter = new Butter();
	_gameObject.push_back(butter); //butter = 2
	track = new Track();
	_gameObject.push_back(track); //track = 3
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
//---------------------------------------------------------------------------------------

void GameManager::renderScene(void) {

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

		//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

		float res[4];
		multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so it is converted to eye space
		glUniform4fv(lPos_uniformId, 1, res);

		//Associar os Texture Units aos Objects Texture
		//stone.tga loaded in TU0; checker.tga loaded in TU1;  lightwood.tga loaded in TU2

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureArray[0]);  

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

		//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
		glUniform1i(tex_loc, 0);  
		glUniform1i(tex_loc1, 1); 
		glUniform1i(tex_loc2, 2);
		glUniform1i(tex_loc3, 3);

		int auxId;
		for(auxId=0; auxId<4; auxId++){
			objId=auxId;
			_gameObject[auxId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
		}
			
		glBindTexture(GL_TEXTURE_2D, 0);		
		glutSwapBuffers();
}

// ----------------------------------------------------------------------------------------
// Shader Stuff
//--------------------------------------------------------------------------------------

GLuint GameManager::setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/texture_demo.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/texture_demo.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode"); // different modes of texturing
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	tex_loc3 = glGetUniformLocation(shader.getProgramIndex(), "texmap3");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramValid());
}

// ----------------------------------------------------------------------------------------
// Model loading and OpenGL setup
//----------------------------------------------------------------------------------------

// ------------------------------Init Function------------------------------

void GameManager::init(){

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	//Texture Object definition
	
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "stone.tga", 0);
	TGA_Texture(TextureArray, "blue_checkboard.tga", 1);
	TGA_Texture(TextureArray, "lightwood.tga", 2);
	TGA_Texture(TextureArray, "orange1.tga", 3);

	int auxId;
	for(auxId=0; auxId<4; auxId++){
		objId=auxId;
		_gameObject[auxId]->create(mesh);
	}

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.529f, 0.808f,0.922f, 1.0f);

}
