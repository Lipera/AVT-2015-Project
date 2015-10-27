#include "GameManager.h"

//---------------------------------------------variables--------------------------------------------
#define INITIAL_LIVES 5

int n_lives = INITIAL_LIVES;

bool keyO = false, keyP = false, keyQ = false, keyA = false;

int oldTimeSinceStart;
int time = 0;

bool play = true;
bool restart = false;
bool gameOver = false;

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

struct MyMesh mesh[15];
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
GLint tex_loc, tex_loc1, tex_loc2, tex_loc3, tex_loc4, tex_loc5;
GLint texMode_uniformId;

GLuint TextureArray[10];


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

//------------------create object class---------------------

Table* table;
Board* board;
Butter* butter;
Track* track;
Candle* candle;
Orange* orange;
Car* car;
GameOver* over;
Pause* pause;

//--------------------------------------Constructor and Destructor--------------------------------------

GameManager::GameManager(){
    //to do
	Camera* cam0 = (Camera*) new OrthogonalCamera(-20.0f,20.0f,-20.0f,20.0f,-20.0f,40.0f);
	_cameras.push_back(cam0);
	Camera* cam1 = (Camera*) new PerspectiveCamera(53.13f, 1.0f, 0.1f,1000.0f);
	_cameras.push_back(cam1);
	Camera* cam2 = (Camera*) new PerspectiveCamera(53.13f, 1.0f, 0.1f,1000.0f);
	_cameras.push_back(cam2);
	Camera* cam3 = (Camera*) new OrthogonalCamera(-20.0f,20.0f,-20.0f,20.0f,-20.0f,100.0f);
	_cameras.push_back(cam3);

	table = new Table();
	_gameObject.push_back(table); //table = 0
	board = new Board();
	_gameObject.push_back(board); //board = 1
	butter = new Butter();
	_gameObject.push_back(butter); //butter = 2
	track = new Track();
	_gameObject.push_back(track); //track = 3
	candle = new Candle();
	_gameObject.push_back(candle); //candle = 4
	orange = new Orange(orangeX, orangeY, orangeZ);
	_gameObject.push_back(orange); //orange = 5
	car = new Car(carX, carY, carZ);
	_gameObject.push_back(car); // car = 6
	over = new GameOver();
	_gameObject.push_back(over); //over = 7
	pause = new Pause();
	_gameObject.push_back(pause);// pause = 8


	int i;
	for (i = 0; i < INITIAL_LIVES; i++) {
		_lives.push_back(new Car(0.0f, 0.0f, 0.0f));
		((Car *)_lives[i])->setPosition(-20 + (i * 3), 20, 0);
	}
}

GameManager::~GameManager(){
	//to do
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

	_cameras[camS]->computeVisualizationMatrix(w,h);

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
	if(play){
		orange->update(deltaTime);
	}
	/*carAlpha -= carAlphaVar;
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
	//if(0 <carVelAct || carVelAct <carVelMax) {
		//carVelAct += carVelInc;
	//}

	carX += deltaTime * ( carVelAct * cos(-carAlpha * 3.14f / 180.0f));
	carZ += deltaTime * ( carVelAct * sin(-carAlpha * 3.14f / 180.0f));
	*/

	if (gameOver == true) {
		//Restart game
		if (restart == true) {
			n_lives = INITIAL_LIVES;
			gameOver = false;
			restart = false;
		}
	}
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

		case 'S' :
		case 's': play = !play; break;

		case 'R' :
		case 'r' : 
			if (gameOver == true){
				restart = true;
            }
			break;

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

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, TextureArray[3]);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, TextureArray[4]);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, TextureArray[5]);

		//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
		glUniform1i(tex_loc, 0);  
		glUniform1i(tex_loc1, 1); 
		glUniform1i(tex_loc2, 2);
		glUniform1i(tex_loc3, 3);
		glUniform1i(tex_loc4, 4);
		glUniform1i(tex_loc5, 5);

		int auxId;
		objId = 0;
		for(auxId=0; auxId<7; auxId++, objId++){
			_gameObject[auxId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
		}


//Select orthogonal camera and draw frogs(lives) left

    glPushMatrix();
    //glDisable(GL_LIGHTING);
    _cameras[3]->computeProjectionMatrix();
    _cameras[3]->computeVisualizationMatrix(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    for (int i = 0; i < n_lives; i++) {
            pushMatrix(MODEL);
			translate(MODEL, 18.0f, 0.0f, -17.0f + i * 1.5f);
            scale(MODEL, 0.65f, 0.65f, 0.65f);
            _lives[i]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
            popMatrix(MODEL);
    }
    
    if (gameOver){
		objId=12;
       _gameObject[7]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
    }
    
    if (!play){
		objId=12;
        _gameObject[8]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
    }
		
		glBindTexture(GL_TEXTURE_2D, 0);		
		glutSwapBuffers();

	//para voltar a repor a camara do jogo
    _cameras[camS]->computeVisualizationMatrix(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
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
	tex_loc4 = glGetUniformLocation(shader.getProgramIndex(), "texmap4");
	tex_loc5 = glGetUniformLocation(shader.getProgramIndex(), "texmap5");
	
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
	
	glGenTextures(6, TextureArray);
	TGA_Texture(TextureArray, "stone.tga", 0);
	TGA_Texture(TextureArray, "checker.tga", 1);
	TGA_Texture(TextureArray, "lightwood.tga", 2);
	TGA_Texture(TextureArray, "orange1.tga", 3);
	TGA_Texture(TextureArray, "gameover.tga", 4);
	TGA_Texture(TextureArray, "pause.tga", 5);

	int auxId;
	objId = 0;
	for(auxId=0; auxId<9; auxId++, objId++){
		_gameObject[auxId]->create(mesh, &objId);
	}


	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.529f, 0.808f,0.922f, 1.0f);

}
