#include "GameManager.h"

//---------------------------------------------variables--------------------------------------------
#define INITIAL_LIVES 5
#define CANDLE_LIGHT_NUM 6
#define DIRECITONAL_LIGHT_INDEX 6
#define SPOT_LIGHT_INDEX 7
#define SPOT_LIGHT_NUM 2

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
float carX = -10.0f, carY = 0.0f, carZ = -12.0f;
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
GLint tex_loc, tex_loc1, tex_loc2, tex_loc3, tex_loc4, tex_loc5, tex_loc6, tex_loc7, tex_loc8, tex_loc9;
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
//float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};

//orange Position
float orangeX = 6.0f, orangeY = 2.0f, orangeZ = 15.5f;

//------------------create object class---------------------

Table* table;
Board* board;
Butter* butter;
Cheerios* cheerio;
Candle* candle;
Orange* orange;
Car* car;
GameOver* over;
Pause* pause;
Butter* butter2;
Butter* butter3;
Butter* butter4;
Cheerios* c;
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
	butter = new Butter(-5.0f, 0.0f, -7.0f);
	_gameObject.push_back(butter); //butter = 2
	butter2 = new Butter(-3.0f, 0.0f, -2.0f);
	_gameObject.push_back(butter2); //butter2 = 3
	butter3 = new Butter(-9.0f, 0.0f, 11.0f);
	_gameObject.push_back(butter3); //butter3 = 4
	butter4 = new Butter(-11.0f, 0.0f, 7.0f);
	_gameObject.push_back(butter4); //butter4 = 5
	candle = new Candle();
	_gameObject.push_back(candle); //candle = 6
	orange = new Orange(orangeX, orangeY, orangeZ);
	_gameObject.push_back(orange); //orange = 7
	car = new Car(carX, carY, carZ, 0);
	_gameObject.push_back(car); // car = 8
	over = new GameOver();
	_gameObject.push_back(over); //over = 9
	pause = new Pause();
	_gameObject.push_back(pause);// pause = 10
	cheerio = new Cheerios(0.0f, 0.0f, 0.0f);
	_gameObject.push_back(cheerio); //track = 11


	int i;
	for (i = 0; i < INITIAL_LIVES; i++) {
		_lives.push_back(new Car(0.0f, 0.0f, 0.0f, 1));
		((Car *)_lives[i])->setPosition(-20.0f + (i * 3.0f), 20.0f, 0.0f);
	}

//--------------------------------------Track------------------------------------
	track();

//--------------------------------------||----------------------------------------
	//Candle Lights
	LightSource *light0 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(8.0f, 2.70f, -3.0f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light0);
	LightSource *light1 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(-7.5f, 2.70f, -8.0f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light1);
	LightSource *light2 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(8.5f, 2.70f, 9.0f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light2);
	LightSource *light3 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(-4.0f, 2.70f, 2.75f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light3);
	LightSource *light4 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(2.0f, 2.70f, -8.0f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light4);
	LightSource *light5 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(3.0f, 2.70f, 9.0f, 1.0f), 0.02f, 0.02f, 0.02f);
	_lights.push_back(light5);

	//Sun Light
	LightSource *light6 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(0.0f, 10.0f, 0.0f, 0.0f), 0.01f, 0.01f, 0.001f);
	light6->changeState(); //Sunlight starts enabled
	_lights.push_back(light6);
	//Car Lights
	LightSource *light7 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(carX + 3.0f, carY - 0.5f, carZ + 0.5f, 1.0f), 0.02f, 0.02f, 0.02f, 45, new Vector4(carX, carY, carZ, 0.0f));
	_lights.push_back(light7);
	LightSource *light8 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(carX + 3.0f, carY - 0.5f, carZ - 0.5f, 1.0f), 0.02f, 0.02f, 0.02f, 45, new Vector4(carX, carY, carZ, 0.0f));
	_lights.push_back(light8);




}

void GameManager::track(){
	int aux;
	int aux2 = 0;
	int aux3 = 0;
	int aux4 = 0;
	int aux5 = 0;
	for(aux=1; aux < 69; ++aux){
		if(aux<=9){
			c = new Cheerios(-11.0f + aux * 2.5f, 0.2f, -13.5f);
			_track.push_back(c);
		}else if (9 < aux && aux <= 17) {
			c = new Cheerios(-33.5f + aux * 2.5f, 0.2f, -10.0f);
			_track.push_back(c);
		}else if(17 < aux && aux<=28){
			c = new Cheerios(13.0f, 0.2f, -11.5f + aux2 * 2.5f);
			_track.push_back(c);
			aux2++;
		}else if (28 < aux && aux <= 36) {
			c = new Cheerios(10.0f, 0.2f, -36.5f + aux2 * 2.5f);
			_track.push_back(c);
			aux2++;
		//direita
		}else if(36 < aux && aux<=41){
			c = new Cheerios(0.5f + aux3 * 2.5f, 0.2f, 14.0f);
			_track.push_back(c);
			aux3++;
		}else if (41 < aux && aux <= 44) {
			c = new Cheerios(-8.0f + aux3 * 2.5f, 0.2f, 11.0f);
			_track.push_back(c);
			aux3++;
		//baixo
		}else if(44 < aux && aux <= 50){
			c = new Cheerios(-12.5f, 0.2f, -13.0f + aux4 * 2.5f);
			_track.push_back(c);
			aux4++;
		}else if (50 < aux && aux <= 54) {
			c = new Cheerios(-9.25f, 0.2f, -23.75f + aux4 * 2.5f);
			_track.push_back(c);
			aux4++;
		//diagonal
		}else if(54 < aux && aux <= 61) {
			c = new Cheerios(-12.5f + aux5 * 1.85f, 0.2f, 1.5f + aux5 * 1.85f);
			_track.push_back(c);
			aux5++;
		}else if (61 < aux && aux <= 68) {
			c = new Cheerios(-21.5f + aux5 * 1.85f, 0.2f, -12.75f + aux5 * 1.85f);
			_track.push_back(c);
			aux5++;
		} 
	}
}

void GameManager::reorganizeGame(){

	//butter
	butter->setPosition(-5.0f, 0.0f, -7.0f);
	butter2->setPosition(-3.0f, 0.0f, -2.0f);
	butter3->setPosition(-9.0f, 0.0f, 11.0f);
	butter4->setPosition(-11.0f, 0.0f, 7.0f);

	//cheerios
	int cheerios=0;
	int aux;
	int aux2 = 0;
	int aux3 = 0;
	int aux4 = 0;
	int aux5 = 0;
	for(aux=1; aux < 69; ++aux){
		if(aux<=9){
			_track[cheerios]->setPosition(-11.0f + aux * 2.5f, 0.2f, -13.5f);
			cheerios++;
		}else if (9 < aux && aux <= 17) {
			_track[cheerios]->setPosition(-33.5f + aux * 2.5f, 0.2f, -10.0f);
			cheerios++;
		}else if(17 < aux && aux<=28){
			_track[cheerios]->setPosition(13.0f, 0.2f, -11.5f + aux2 * 2.5f);
			cheerios++;
			aux2++;
		}else if (28 < aux && aux <= 36) {
			_track[cheerios]->setPosition(10.0f, 0.2f, -36.5f + aux2 * 2.5f);
			cheerios++;
			aux2++;
		//direita
		}else if(36 < aux && aux<=41){
			_track[cheerios]->setPosition(0.5f + aux3 * 2.5f, 0.2f, 14.0f);
			cheerios++;
			aux3++;
		}else if (41 < aux && aux <= 44) {
			_track[cheerios]->setPosition(-8.0f + aux3 * 2.5f, 0.2f, 11.0f);
			cheerios++;
			aux3++;
		//baixo
		}else if(44 < aux && aux <= 50){
			_track[cheerios]->setPosition(-12.5f, 0.2f, -13.0f + aux4 * 2.5f);
			cheerios++;
			aux4++;
		}else if (50 < aux && aux <= 54) {
			_track[cheerios]->setPosition(-9.25f, 0.2f, -23.75f + aux4 * 2.5f);
			cheerios++;
			aux4++;
		//diagonal
		}else if(54 < aux && aux <= 61) {
			_track[cheerios]->setPosition(-12.5f + aux5 * 1.85f, 0.2f, 1.5f + aux5 * 1.85f);
			cheerios++;
			aux5++;
		}else if (61 < aux && aux <= 68) {
			_track[cheerios]->setPosition(-21.5f + aux5 * 1.85f, 0.2f, -12.75f + aux5 * 1.85f);
			cheerios++;
			aux5++;
		} 
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
	
	int deltaTime;
    
    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;

	time += deltaTime;
	if(play){
		orange->update(deltaTime);
		car->updateAux(keyQ, keyA, keyO, keyP, deltaTime);

	}

	
	std::ostringstream oss;
	oss << CAPTION << ": " << (int)(FrameCount/(deltaTime*0.001f)) << " FPS @ (" << _WinX << "x" << _WinY << ")   PONTOS: 0";
	std::string s = oss.str();
	glutSetWindow(_WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;


//------------------------------------------colisoes--------------------------------------------------------------

	float o1_xmin = car->getPosition()->getX() + car->getbottomLeft()->getX();
	float o1_xmax = car->getPosition()->getX() + car->gettopRight()->getX();
    float o1_zmin = car->getPosition()->getZ() + car->getbottomLeft()->getZ();
    float o1_zmax = car->getPosition()->getZ() + car->gettopRight()->getZ();

	//colisao com as margens
	if(o1_xmin<-16.5f || o1_xmin > 16.5f || o1_zmin<-16.5f || o1_zmin > 16.5f || o1_xmax<-16.5f || o1_xmax > 16.5f || o1_zmax<-16.5f || o1_zmax > 16.5f){
		car->setPosition(-10.0f, 0.0f, -11.75);
		car->setAngle(0.0f);
		n_lives--;
		if(n_lives < 1) {
			play = false;
			gameOver = true;
		}
	}

	//colisao com a laranja
	if(collision(car,orange,car->getPosition(), orange->getPosition())) {
		car->setPosition(-10.0f, 0.0f, -11.75);
		car->setAngle(0.0f);
		n_lives--;
		if(n_lives < 1) {
			play = false;
			gameOver = true;
		}
	}

	//colisao com as manteigas
	for(int i = 2; i<6; i++){
		Butter* butteraux = (Butter*)_gameObject[i];
		
		if(collision(car,_gameObject[i],car->getPosition(), _gameObject[i]->getPosition())) {
			
			butteraux->setVelocidade(car->getVelocidade());
			butteraux->setAngle(car->getAngle());

			car->setVelocidade(0.0f);
			car->setAceleracao(0.0f);
			
			//_gameObject[i]->setPosition(_gameObject[i]->getPosition()->getX() + 0.1f, _gameObject[i]->getPosition()->getY(), _gameObject[i]->getPosition()->getZ()+0.1f);
		}
		butteraux->update(deltaTime);
	}

	//colisao com os cheerios
	for(int i = 0; i<68; i++){
		Cheerios* cheerioaux = (Cheerios*)_track[i];
		
		if(collision(car,_track[i],car->getPosition(), _track[i]->getPosition())) {
			cheerioaux->setVelocidade(car->getVelocidade());
			cheerioaux->setAngle(car->getAngle());
			car->setVelocidade(0.0f);
			car->setAceleracao(0.0f);
			//_track[i]->setPosition(_track[i]->getPosition()->getX() + 0.1f, _track[i]->getPosition()->getY(), _track[i]->getPosition()->getZ()+0.1f);
		}
		cheerioaux->update(deltaTime);
	}

	if (gameOver == true) {
		//Restart game
		if (restart == true) {
			n_lives = INITIAL_LIVES;
			gameOver = false;
			restart = false;
			car->setAceleracao(0.0f);
			car->setVelocidade(0.0f);
			car->setAngle(0.0f);
			reorganizeGame();
			orange->setSpeed(0.0f, 0.0f, 0.0f);
			orange->setPosition(orangeX,orangeY,orangeZ);
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

		case 'x': 
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
			break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'b': glDisable(GL_MULTISAMPLE); break;

		case 'S' :
		case 's': 
			if(gameOver == true) {
				break;
			}
			play = !play; 
			break;

		case 'R' :
		case 'r' : 
			if (gameOver == true){
				restart = true;
				play = true;
            }
			break;

		case 'C' :
		case 'c' :	//Enables candle lighting
			if(play) {
				for(int i = 0; i < CANDLE_LIGHT_NUM; i++) {
					_lights[i]->changeState();
				}
			}
			break;

		case 'N' :
		case 'n' :
			if(play){
				_lights[DIRECITONAL_LIGHT_INDEX]->changeState();
			}
			break;

		case 'H' :
		case 'h' :
			if(play){
				for(int j = 0; j < SPOT_LIGHT_NUM; j++) {
					_lights[SPOT_LIGHT_INDEX]->changeState();
				}
			}
			break;

		case '1':	if(play){
						camS = 0; 
						r=25.0f; 
						printf("Orthogonal Camera\n"); 
						reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					}
					break;
		case '2':	if(play){
						camS = 1; 
						printf("Perspective Camera without mouse\n"); 
						camX = -28.0f;
						camY = 27.0f; 
						camZ = 0.0f;
						reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					}
					break;
		case '3':	if(play){
						camS = 2; 
						printf("Perspective Camera with mouse\n"); 
						reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					}
					break;

		case 'O':
        case 'o': // mover o carro para a esquerda
			keyO = true;
			car->setVarAngle(-3.0f);
            break;
        case 'P':
        case 'p': // mover o carro para a direita
			keyP = true;
			car->setVarAngle(3.0f);
            break;
        case 'Q':
        case 'q': // mover o carro para cima
			keyQ = true;
			car->setAceleracao(0.00075f);
            break;
        case 'A':
        case 'a': // mover o carro para baixo
			keyA = true;
			car->setAceleracao(-0.00075f);
            break;
	}
}

void GameManager::processKeysUp(unsigned char key, int xx, int yy){
    
    switch (key) {
        case 'O':
        case 'o': 
			keyO = false;
			car->setVarAngle(0.0f);
			if(keyP) {
				car->setVarAngle(3.0f);
			}
            break;
        case 'P':
        case 'p':
            keyP = false;
			car->setVarAngle(0.0f);
			if(keyO) {
				car->setVarAngle(-3.0f);
			}
            break;
        case 'Q':
        case 'q':
            keyQ = false;
			car->setAceleracao(-0.0005f);
            break;
        case 'A':
        case 'a':
            keyA = false;
			car->setAceleracao(0.0005f);
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
			float auxCarX = _gameObject[8]->getPosition()->getX();
			float auxCarY = _gameObject[8]->getPosition()->getY();
			float auxCarZ = _gameObject[8]->getPosition()->getZ();
			float auxCenterX = auxCarX + (5.0f * cos((-_gameObject[8]->getAngle() * 3.14f / 180.0f) + 3.14));
			float auxCenterY = 3.0f;
			float auxCenterZ = auxCarZ + (5.0f * sin((-_gameObject[8]->getAngle() * 3.14f / 180.0f) + 3.14));

			camX = auxCenterX;
			camY = auxCenterY;
			camZ = auxCenterZ;
			lookAt(camX, camY, camZ, auxCarX, auxCarY, auxCarZ, 0.0, 1.0, 0.0);
		}
		// use our shader
		glUseProgram(shader.getProgramIndex());

		//send the light position in eye coordinates

		//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

		//float res[4];
		//multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so it is converted to eye space
		//glUniform4fv(lPos_uniformId, 1, res);

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

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, TextureArray[6]);

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, TextureArray[7]);

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, TextureArray[8]);
		
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, TextureArray[9]);


		//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
		glUniform1i(tex_loc, 0);  
		glUniform1i(tex_loc1, 1); 
		glUniform1i(tex_loc2, 2);
		glUniform1i(tex_loc3, 3);
		glUniform1i(tex_loc4, 4);
		glUniform1i(tex_loc5, 5);
		glUniform1i(tex_loc6, 6);
		glUniform1i(tex_loc7, 7);
		glUniform1i(tex_loc8, 8);
		glUniform1i(tex_loc9, 9);


		//Draw the lights
		for(size_t i = 0; i < _lights.size(); i++) {
			_lights[i]->draw(i, shader);
		}

		int auxId;
		objId = 0;
		for(auxId=0; auxId<9; auxId++, objId++){
			//movimento do carro
			if (auxId == 8){
				pushMatrix(MODEL);
				translate(MODEL, _gameObject[auxId]->getPosition()->getX(), _gameObject[auxId]->getPosition()->getY(), _gameObject[auxId]->getPosition()->getZ());
				rotate(MODEL, _gameObject[auxId]->getAngle(), 0.0f, 1.0f, 0.0f);
				scale(MODEL, 0.9f, 0.9f, 0.9f);
				_gameObject[auxId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
				computeDerivedMatrix(PROJ_VIEW_MODEL);
				glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
				glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
				computeNormalMatrix3x3();
				glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	            popMatrix(MODEL);

			}else if(auxId==3 || auxId==4 || auxId==5){
				objId=2;
				_gameObject[auxId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			}
			else {
				_gameObject[auxId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			}
		}

		int cheerioId;
		for(cheerioId=0; cheerioId<68; cheerioId++){
			objId=12;
			_track[cheerioId]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
		}

//Select orthogonal camera and draw cars(lives) left

    glPushMatrix();
    //glDisable(GL_LIGHTING);
    _cameras[3]->computeProjectionMatrix();
    _cameras[3]->computeVisualizationMatrix(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    for (int i = 0; i < n_lives; i++) {
            pushMatrix(MODEL);
			translate(MODEL, 18.0f, 0.0f, -17.0f + i * 1.5f);
            scale(MODEL, 0.75f, 0.65f, 0.75f);
            _lives[i]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
            popMatrix(MODEL);
    }
    
    if (gameOver){
		objId=11;
       _gameObject[9]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
    }
    
    if (!play){
		objId=11;
        _gameObject[10]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
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
	//lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	tex_loc3 = glGetUniformLocation(shader.getProgramIndex(), "texmap3");
	tex_loc4 = glGetUniformLocation(shader.getProgramIndex(), "texmap4");
	tex_loc5 = glGetUniformLocation(shader.getProgramIndex(), "texmap5");
	tex_loc6 = glGetUniformLocation(shader.getProgramIndex(), "texmap6");
	tex_loc7 = glGetUniformLocation(shader.getProgramIndex(), "texmap7");
	tex_loc8 = glGetUniformLocation(shader.getProgramIndex(), "texmap8");
	tex_loc9 = glGetUniformLocation(shader.getProgramIndex(), "texmap9");
	
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
	
	glGenTextures(10, TextureArray);
	TGA_Texture(TextureArray, "stone.tga", 0);
	TGA_Texture(TextureArray, "checker.tga", 1);
	TGA_Texture(TextureArray, "lightwood.tga", 2);
	TGA_Texture(TextureArray, "orange.tga", 3);
	TGA_Texture(TextureArray, "gameover.tga", 4);
	TGA_Texture(TextureArray, "pause.tga", 5);
	TGA_Texture(TextureArray, "manteiga.tga", 6);
	TGA_Texture(TextureArray, "cheerio.tga", 7);
	TGA_Texture(TextureArray, "candle.tga", 8);
	TGA_Texture(TextureArray, "candle.tga", 9);

	int auxId;
	objId = 0;
	for(auxId=0; auxId<12; auxId++, objId++){
		if(auxId==3 || auxId==4 || auxId==5){
			objId=2;
			_gameObject[auxId]->create(mesh, &objId);
		}
		_gameObject[auxId]->create(mesh, &objId);
	}


	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.529f, 0.808f,0.922f, 1.0f);

}


//---------------------------------------------auxiliar das colisoes-----------------------------------------

bool GameManager::collision(GameObject *obj1, GameObject *obj2, Vector3 *obj1_position, Vector3 *obj2_position) {
    float o1_xmin = obj1_position->getX() + obj1->getbottomLeft()->getX();
    float o1_xmax = obj1_position->getX() + obj1->gettopRight()->getX();
    float o1_zmin = obj1_position->getZ() + obj1->getbottomLeft()->getZ();
    float o1_zmax = obj1_position->getZ() + obj1->gettopRight()->getZ();
    float o2_xmin = obj2_position->getX() + obj2->getbottomLeft()->getX();
    float o2_xmax = obj2_position->getX() + obj2->gettopRight()->getX();
    float o2_zmin = obj2_position->getZ() + obj2->getbottomLeft()->getZ();
    float o2_zmax = obj2_position->getZ() + obj2->gettopRight()->getZ();
    
    if(o2_xmin < o1_xmax && o1_xmax < o2_xmax && o2_zmin < o1_zmax && o1_zmax < o2_zmax){
        return true;
    }else if(o2_xmin < o1_xmin && o1_xmin < o2_xmax && o2_zmin < o1_zmax && o1_zmax < o2_zmax){
        return true;
    }else if (o2_xmin < o1_xmax && o1_xmax < o2_xmax && o2_zmin < o1_zmin && o1_zmin < o2_zmax){
        return true;
    }else if (o2_xmin < o1_xmin && o1_xmin < o2_xmax && o2_zmin < o1_zmin && o1_zmin < o2_zmax){
        return true;
    }else{
        return false;
    }
    
}

