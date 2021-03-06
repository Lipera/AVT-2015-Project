#include "GameManager.h"

//---------------------------------------------variables--------------------------------------------
#define INITIAL_LIVES 5
#define CANDLE_LIGHT_NUM 6
#define DIRECITONAL_LIGHT_INDEX 6
#define SPOT_LIGHT_INDEX 7
#define SPOT_LIGHT_NUM 2
#define PI 3.14
#define FLARE_SCALE 0.8f
#define FLARE_MAX_SIZE 0.8f

int n_lives = INITIAL_LIVES;

bool keyO = false, keyP = false, keyQ = false, keyA = false;

int oldTimeSinceStart;
int time = 0;

bool play = true;
bool restart = false;
bool gameOver = false;
bool particles = false;
float particle_color[4];

// Camera
// Camera Position
float camX, camY, camZ;
float atX, atY, atZ;
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

struct MyMesh mesh[25];
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
GLint tex_loc, tex_loc1, tex_loc2, tex_loc3, tex_loc4, tex_loc5, tex_loc6, tex_loc7, tex_loc8, tex_loc9, tex_loc10;
GLint tex_loc11, tex_loc12, tex_loc13, tex_loc14, tex_loc15, tex_loc16, tex_loc17;
GLint texMode_uniformId;

GLuint TextureArray[20];


// Mouse Tracking Variables
int startX, startY, tracking = 0;
int endX, endY;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 6.0f;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
//float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};

//orange Position
float orangeX = 6.0f, orangeY = 1.25f, orangeZ = 15.5f;

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
Billboard* b;
Glass* glass;
Coaster* coaster;
Juice* juice;
Rubik* rubik;
Rubik* rubik2;
Pot* pot;
Sun* sun;
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
	Camera* cam4 = (Camera*) new PerspectiveCamera(53.13f, 1.0f, 0.1f,1000.0f);
	_cameras.push_back(cam4);
	Camera* cam5 = (Camera*) new OrthogonalCamera(-20.0f,20.0f,-20.0f,20.0f,-20.0f,100.0f);
	_cameras.push_back(cam5);
	
	//---------auxId-------------

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
	glass = new Glass();
	_gameObject.push_back(glass); //glass = 12
	juice = new Juice();
	_gameObject.push_back(juice); //juice = 13
	coaster = new Coaster();
	_gameObject.push_back(coaster); //glass = 14
	rubik = new Rubik(0);
	_gameObject.push_back(rubik); //rubik = 15
	rubik2 = new Rubik(1);
	_gameObject.push_back(rubik2); //rubik = 16
	//billboard
	b = new Billboard();
	_gameObject.push_back(b); //billboard = 17

	pot = new Pot();
	_gameObject.push_back(pot); //pot = 18
	sun = new Sun();
	_gameObject.push_back(sun); //sun  = 19


	//Lives
	int i;
	for (i = 0; i < INITIAL_LIVES; i++) {
		_lives.push_back(new Car(0.0f, 0.0f, 0.0f, 1));
		((Car *)_lives[i])->setPosition(-20.0f + (i * 3.0f), 20.0f, 0.0f);
	}

	//Particles
	int j;
	for (j = 0; j<MAX_PARTICULAS; j++){
		_particles.push_back(new Particle());
	}

	//Lens Flares
	int k;
	for (k = 0; k<MAX_LENSFLARES; k++) {
		//_lensFlare.push_back(new LensFlare(0));
	if(k==0 || k==7){
			_lensFlare.push_back(new LensFlare(1));
			//((LensFlare*)_lensFlare[i])->setPosition(-20.0f + (i * 3.0f), 10.0f, 0.0f);
		}else if(k==1 || k==6){
			_lensFlare.push_back(new LensFlare(0));
			//((LensFlare*)_lensFlare[i])->setPosition(-20.0f + (i * 3.0f), 10.0f, 0.0f);
		}else if(k==2 || k==5){
			_lensFlare.push_back(new LensFlare(2));
			//((LensFlare*)_lensFlare[i])->setPosition(-20.0f + (i * 3.0f), 10.0f, 0.0f);
		}else if(k==3 || k==4){
			_lensFlare.push_back(new LensFlare(3));
			//((LensFlare*)_lensFlare[i])->setPosition(-20.0f + (i * 3.0f), 10.0f, 0.0f);
		}
	}

//--------------------------------------Track------------------------------------
	track();

//--------------------------------------||----------------------------------------
	//Candle Lights
	LightSource *light0 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(8.0f, 2.70f, -3.0f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light0);
	LightSource *light1 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(-7.5f, 2.70f, -8.0f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light1);
	LightSource *light2 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(8.5f, 2.70f, 9.0f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light2);
	LightSource *light3 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(-4.0f, 2.70f, 2.75f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light3);
	LightSource *light4 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(2.0f, 2.70f, -8.0f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light4);
	LightSource *light5 = (LightSource*) new LightSource(new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(1.0f,0.35f,0.11f,1.0f), new Vector4(3.0f, 2.70f, 9.0f, 1.0f), 0.02f, 0.2f, 0.02f);
	_lights.push_back(light5);

	//Sun Light
	LightSource *light6 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(-0.28f, 0.27f, 0.0f, 0.0f), 0.01f, 0.01f, 0.001f);
	light6->changeState(); //Sunlight starts enabled
	_lights.push_back(light6);
	//Car Lights
	float spotDirX = cos((-_gameObject[8]->getAngle() * PI / 180.0f));
	float spotDirY = 0.0f;
	float spotDirZ = sin((-_gameObject[8]->getAngle() * PI / 180.0f));

	LightSource *light7 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(carX + 0.5f, carY + 0.2f, carZ - 0.2f, 1.0f), 0.02f, 0.02f, 0.02f, 25.0f, 0.0f, new Vector4(spotDirX, spotDirY, spotDirZ, 0.0f));
	_lights.push_back(light7);
	LightSource *light8 = (LightSource*) new LightSource(new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(1.0f,1.0f,1.0f,1.0f), new Vector4(carX + 0.5f, carY + 0.2f, carZ + 0.3f, 1.0f), 0.02f, 0.02f, 0.02f, 25.0f, 0.0f, new Vector4(spotDirX, spotDirY, spotDirZ, 0.0f));
	_lights.push_back(light8);

	//Fog
	_isFogActive = false;

	_isFlaresActive = false;
}

//Organize the track
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

//-------------------------------------Iterator of particles---------------------------

void GameManager::iterate(int value){
	int i;
	float h;

	/* M�todo de Euler de integra��o de eq. diferenciais ordin�rias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */
	h = 0.125f;
	for (i = 0; i<MAX_PARTICULAS; i++) {

		float life = _particles[i]->getLife();
		float fade = _particles[i]->getFade();
		Vector3 position = _particles[i]->getPosition();
		Vector3 speed = _particles[i]->getSpeed();
		Vector3 acceleration = _particles[i]->getAcceleration();

		float x = position.getX();
		x += h*(speed.getX());
		float y = position.getY();
		y += h*(speed.getY());
		float z = position.getZ();
		z += h*(speed.getZ());

		_particles[i]->setPosition(x, y, z);

		float vx = speed.getX();
		vx += h*(acceleration.getX());
		float vy = speed.getY();
		vy += h*(acceleration.getY());
		float vz = speed.getZ();
		vz += h*(acceleration.getZ());

		_particles[i]->setSpeed(vx, vy, vz);
		
		float l = life;
		l -= fade;

		_particles[i]->setLife(l);

	}
	
}

//--------------------------------------Check Sun ---------------------------------------

void GameManager::checkSun() {

}

// -------------------------------------Timer--------------------------------------------

void GameManager::timer(int value){
	
	iterate(0);

	int deltaTime;
    
    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;

	time += deltaTime;
	if(play){
		orange->update(deltaTime);
		car->updateAux(keyQ, keyA, keyO, keyP, deltaTime);

		//update spolight position
		float spotDirX = cos((-_gameObject[8]->getAngle() * PI / 180.0f));
		float spotDirY = 0.0f;
		float spotDirZ = sin((-_gameObject[8]->getAngle() * PI / 180.0f));

		_lights[SPOT_LIGHT_INDEX]->setPosition(new Vector4(_gameObject[8]->getPosition()->getX() + (0.6f * spotDirX) + (0.15f * spotDirZ), _gameObject[8]->getPosition()->getY() + 0.2f, _gameObject[8]->getPosition()->getZ() + (0.6f * spotDirZ) + (-0.15f * spotDirX), 1.0f));
		_lights[SPOT_LIGHT_INDEX+1]->setPosition(new Vector4(_gameObject[8]->getPosition()->getX() + (0.6f * spotDirX) + (-0.25f * spotDirZ), _gameObject[8]->getPosition()->getY() + 0.2f, _gameObject[8]->getPosition()->getZ() + (0.6f * spotDirZ) + (0.25f * spotDirX), 1.0f));

		_lights[SPOT_LIGHT_INDEX]->setSpotDirection(new Vector4(spotDirX, spotDirY, spotDirZ, 0.0f));
		_lights[SPOT_LIGHT_INDEX+1]->setSpotDirection(new Vector4(spotDirX, spotDirY, spotDirZ, 0.0f));

		checkSun();
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
					_lights[SPOT_LIGHT_INDEX+j]->changeState();
				}
			}
			break;
		case 'F':
		case 'f':
			if(play){
				_isFogActive = !_isFogActive;
			}
			break;
		case 'K':
		case 'k':
			particles = true;
			initParticles();
			break;

		case 'L' :
		case 'l':   
			if(camS == 2 && play) {
				_isFlaresActive = !_isFlaresActive; 
			}
		   break;

		case '1':	if(play){
						camS = 0; 
						r=25.0f; 
						camX=0.0f;
						camY=20.0f;
						camZ=0.0f;
						atX=0.0f;
						atY=0.0f;
						atZ=0.0f;
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
						atX=0.0f;
						atY=0.0f;
						atZ=0.0f;
						reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					}
					break;
		case '3':	if(play){
						camS = 2; 
						printf("Perspective Camera with mouse\n"); 
						reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); 
					}
					break;
		case '4':	if(play){
						camS = 4; 
						printf("Perspective Camera in car\n"); 
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
	
		case 'T':
		case 't':
			int aux = b->getType();
			aux = aux++;
			b->setType(aux);
			if (b->getType() == 5){ b->setType(0);}
				switch(b->getType()) {
					case 0: printf("Cheating Spherical (matrix reset)\n"); break;
					case 1: printf("Cheating Cylindrical (matrix reset)\n"); break;
					case 2: printf("True Spherical\n"); break;
					case 3: printf("True Cylindrical\n"); break;
					case 4: printf("No billboarding\n"); break;
				}
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
			alpha = _gameObject[8]->getAngle() + 90;
			beta = 0;
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
	if(camS == 2 && play) {
		int deltaX, deltaY;
		float alphaAux, betaAux;
		float rAux;

		endX = xx;
		endY = yy;

		deltaX =  - xx + startX;
		deltaY =    yy - startY;

		// left mouse button: move camera
		if (tracking == 1) {


			alphaAux = alpha + deltaX;
			betaAux = beta - deltaY;

			/*if (betaAux > 85.0f)
				betaAux = 85.0f;
			else if (betaAux < -85.0f)
				betaAux = -85.0f;*/
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

		atX = camX + rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
		atZ = camZ + rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
		atY = camY + rAux *   						       sin(betaAux * 3.14f / 180.0f);

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
void GameManager::flare_render(float lx, float ly, float cx, float cy) {
	float dx, dy;		   // Screen coordinates of "destination"
	float px, py;          // Screen coordinates of flare element
	float maxflaredist, flaredist, flaremaxsize, flarescale, distanceScale;
    float width, height, alpha, aspectRatio;    // Piece parameters;
    int i;
    LensFlare *element;

	float SCREENwidth = glutGet( GLUT_WINDOW_WIDTH );
	float SCREENheight = glutGet( GLUT_WINDOW_HEIGHT );

    // Compute how far off-center the flare source is.
    maxflaredist = sqrt(cx * cx + cy * cy);
    flaredist = sqrt((lx - cx) * (lx - cx) + (ly - cy) * (ly - cy));
    flaredist = (maxflaredist - flaredist);
	flaremaxsize = (SCREENwidth * FLARE_MAX_SIZE);
	flarescale = (SCREENwidth * FLARE_SCALE);

	distanceScale = (maxflaredist - flaredist) / maxflaredist;

    // Destination is opposite side of centre from source
    dx = cx + (cx - lx);
    dy = cy + (cy - ly);

    // Render each element.
	for (i = 0; i < _lensFlare.size(); i++)
    {
		element = _lensFlare[i];

        // Position is interpolated along line between start and destination.
		px = (1.0f - element->getFDistance()) * lx + element->getFDistance() * dx;
		py = (1.0f - element->getFDistance()) * ly + element->getFDistance() * dy;

        // Piece size are 0 to 1; flare size is proportion of
        // screen width; scale by flaredist/maxflaredist.
		width = (flaredist * flarescale * element->getFSize()) / maxflaredist;

        // Width gets clamped, to allows the off-axis flares
        // to keep a good size without letting the elements get
        // too big when centered.
        if (width > flaremaxsize)
        {
            width = flaremaxsize;
        }

        // Flare elements are square (round) so height is just
        // width scaled by aspect ratio.
		aspectRatio = SCREENwidth / SCREENheight;

		height = width * aspectRatio;

		alpha = element->getColor().getW();
		element->setAlpha(alpha * distanceScale);
		element->setPosition(px - width/2, element->getPosition().getY(), py - height/2);
		element->setWidth(width);
		element->setHeight(height);
    }
}


void GameManager::renderScene(void) {

	
		FrameCount++;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// load identity matrices
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		// set the camera using a function similar to gluLookAt
		if(camS == 0){
			lookAt(camX, camY, camZ,  atX, atY, atZ, 1.0, 0.0, 0.0);
		}else if(camS == 1){
			lookAt(camX, camY, camZ, atX, atY, atZ, 0.0, 1.0, 0.0);
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
			if(tracking == 0) {
				atX = auxCarX;
				atY = auxCarY;
				atZ = auxCarZ;
			}

			lookAt(camX, camY, camZ, atX, atY, atZ, 0.0, 1.0, 0.0);
		}else if(camS == 4){
			float auxCenterX = _gameObject[8]->getPosition()->getX() + (0.5f * cos((-_gameObject[8]->getAngle() * 3.14f / 180.0f)));
			float auxCenterY = _gameObject[8]->getPosition()->getY() + 1.2f;
			float auxCenterZ = _gameObject[8]->getPosition()->getZ() + (0.5f * sin((-_gameObject[8]->getAngle() * 3.14f / 180.0f)));
			float auxCarX = auxCenterX + (5.0f * cos((-_gameObject[8]->getAngle() * 3.14f / 180.0f)));
			float auxCarY = 0.0f;
			float auxCarZ = auxCenterZ + (5.0f * sin((-_gameObject[8]->getAngle() * 3.14f / 180.0f)));

			camX = auxCenterX;
			camY = auxCenterY;
			camZ = auxCenterZ;
			atX = auxCarX;
			atY = auxCarY;
			atZ = auxCarZ;

			lookAt(camX, camY, camZ, atX, atY, atZ, 0.0, 1.0, 0.0);
		}

		b->setCam(camX, camY, camZ);
		b->setPos(atX, atY, atZ);
		// use our shader
		glUseProgram(shader.getProgramIndex());

		//send the light position in eye coordinates

		//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

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

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, TextureArray[10]);

		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, TextureArray[11]);
		
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_2D, TextureArray[12]);

		glActiveTexture(GL_TEXTURE13);
		glBindTexture(GL_TEXTURE_2D, TextureArray[13]);

		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, TextureArray[14]);

		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, TextureArray[15]);

		glActiveTexture(GL_TEXTURE16);
		glBindTexture(GL_TEXTURE_2D, TextureArray[16]);

		glActiveTexture(GL_TEXTURE17);
		glBindTexture(GL_TEXTURE_2D, TextureArray[17]);


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
		glUniform1i(tex_loc10, 10);
		glUniform1i(tex_loc11, 11);
		glUniform1i(tex_loc12, 12);
		glUniform1i(tex_loc13, 13);
		glUniform1i(tex_loc14, 14);
		glUniform1i(tex_loc15, 15);
		glUniform1i(tex_loc16, 16);
		glUniform1i(tex_loc17, 17);
		
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
		

		//------------------- POT ------------------------------------

		objId = 18;
		_gameObject[18]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);

		objId = 19;
		_gameObject[19]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);

		//------------------- JUICE ------------------------------------

		objId = 14;
		_gameObject[13]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);

		//--------------------------------------------------------------

		// ----------------- STENCIL RUBIK CUBE --------------------
	
			//------------------- RUBIK ------------------------------------

			objId = 16;
			_gameObject[15]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);

		
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				glStencilMask(0xFF); // Write to stencil buffer
				glDepthMask(GL_FALSE); // Don't write to depth buffer
				glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

					// Draw coaster
					objId=15;
					pushMatrix(MODEL);
					translate(MODEL, 0.5f, 0.0f, -2.5f);
					scale(MODEL, 4.0f, 0.5f, 4.0f);
					_gameObject[14]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
					popMatrix(MODEL);
					glEnable(GL_BLEND);
					// Draw rubik cube reflection
				   glStencilFunc(GL_EQUAL, 1, 0xFF);
					glStencilMask(0x00);
					glDepthMask(GL_TRUE);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);
					objId=16;
					pushMatrix(MODEL);
					translate(MODEL, 0.0f, -3.0f, 0.0f);
					_gameObject[16]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
					popMatrix(MODEL);
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
			
				glDisable(GL_STENCIL_TEST);
				glDisable(GL_BLEND);

		// ----------------- STENCIL PARABRISAS --------------------
/*
		glEnable(GL_BLEND);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF); // Write to stencil buffer
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

			// Draw coaster
			objId=15;
			pushMatrix(MODEL);
			translate(MODEL, _gameObject[8]->getPosition()->getX(), _gameObject[8]->getPosition()->getY(), _gameObject[8]->getPosition()->getZ());
			rotate(MODEL, _gameObject[8]->getAngle(), 0.0f, 1.0f, 0.0f);
			scale(MODEL, 1.0f, 0.1f, 0.0f);
			_gameObject[14]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			popMatrix(MODEL);

			// Draw rubik cube reflection
           glStencilFunc(GL_EQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDepthMask(GL_TRUE);

			_cameras[4]->computeVisualizationMatrix(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		*/

	//---------------------------------BILLBOARD --------------------------------------

		objId=17;
		_gameObject[17]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			
	//-----------------------------------------------------------------------------------

		// ----------------- BLENDING (GLASS) ---------------------
		objId=13;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		_gameObject[12]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);

		//-----------------------------------------PARTICLES---------------------------------------------
		if (particles){

			glDisable(GL_DEPTH_TEST); // n�o interessa o z-buffer: as part�culas podem ser desenhadas umas por cima das outras sem problemas de ordena��o 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			GLint loc;

			// draw fireworks particles
			objId = 20;  //quad for particle

			//glUniform1i(texMode_uniformId, 15); // draw modulated textured particles 

			for (int i = 0; i < MAX_PARTICULAS; i++) {

				if (_particles[i]->getLife() > 0.0f){ // s� desenha as que ainda est�o vivas

					// A vida da part�cula representa o canal alpha da cor. Como o blend est� activo a cor final � a soma da cor rgb do fragmento multiplicada pelo
					//alpha com a cor do pixel destino 
					particle_color[0] = _particles[i]->getColor().getX();
					particle_color[1] = _particles[i]->getColor().getY();
					particle_color[2] = _particles[i]->getColor().getZ();
					particle_color[3] = _particles[i]->getLife();

					// send the material - diffuse color modulated with texture
					loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
					glUniform4fv(loc, 1, particle_color);
					glUniform1i(texMode_uniformId, 15); 
					pushMatrix(MODEL);
					translate(MODEL, _particles[i]->getPosition().getX(), _particles[i]->getPosition().getY(), _particles[i]->getPosition().getZ());
					
					// send matrices to OGL
					computeDerivedMatrix(PROJ_VIEW_MODEL);
					glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
					glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
					computeNormalMatrix3x3();
					glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

					glBindVertexArray(mesh[objId].vao);
					glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
					popMatrix(MODEL);
				}
			}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		}
		//----------------------------------------------------------------------------------------------

		//---------------------------------------LENS FLARE----------------------------------------------

		//Select orthogonal camera and draw cars(lives) left
		if(_isFlaresActive && camS == 2) {
			float SCREENwidth = glutGet( GLUT_WINDOW_WIDTH );
			float SCREENheight = glutGet( GLUT_WINDOW_HEIGHT ); 

			 glPushMatrix();
			 _cameras[5]->computeProjectionMatrix();
			 _cameras[5]->computeVisualizationMatrix(SCREENwidth, SCREENheight);

			 glEnable(GL_BLEND);
			 glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			 objId=21;

			 computeDerivedMatrix(PROJ_VIEW_MODEL);

			 //float res[4];
			 //multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so it is converted to eye space
			 //glUniform4fv(lPos_uniformId, 1, res);
 
			 float xFlare = endX;
			 float yFlare = endY;

			 if(tracking == 1) {
				flare_render(xFlare, yFlare, SCREENwidth/2, SCREENheight/2);
			 }
			 
			 for (int j = 0; j<MAX_LENSFLARES; j++) {
				_lensFlare[j]->draw(mesh, shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId, &objId);
			 }	
			 glDisable(GL_BLEND);

		 }

		//-------------------------------------------------------------------------------------------

		//-----------------------------------------LIVES---------------------------------------------------

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
		
		
			//renderBitmapString(0.0f,0.0f, "ola");

		//para voltar a repor a camara do jogo
		_cameras[camS]->computeVisualizationMatrix(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		//--------------------------------------------------------------------------------------------

		//Fog
		GLint loc = glGetUniformLocation(shader.getProgramIndex(), "isFogActive");
		glUniform1i(loc, _isFogActive);
		loc = glGetUniformLocation(shader.getProgramIndex(), "CameraHeight");
		glUniform1f(loc, camY);

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
	tex_loc10 = glGetUniformLocation(shader.getProgramIndex(), "texmap10");
	tex_loc11 = glGetUniformLocation(shader.getProgramIndex(), "texmap11");
	tex_loc12 = glGetUniformLocation(shader.getProgramIndex(), "texmap12");
	tex_loc13 = glGetUniformLocation(shader.getProgramIndex(), "texmap13");
	tex_loc14 = glGetUniformLocation(shader.getProgramIndex(), "texmap14");
	tex_loc15 = glGetUniformLocation(shader.getProgramIndex(), "texmap15");
	tex_loc16 = glGetUniformLocation(shader.getProgramIndex(), "texmap16");
	tex_loc17 = glGetUniformLocation(shader.getProgramIndex(), "texmap17");

	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramValid());
}

// ----------------------------------------------------------------------------------------
// Model loading and OpenGL setup
//----------------------------------------------------------------------------------------

//------------------------INIT PARTICLES-------------------------------

void GameManager::initParticles(){
	
	GLfloat v, theta, phi;
	int i;
	
	for (i = 0; i<MAX_PARTICULAS; i++)
	{
		v = 0.8*frand() + 0.2;
		phi = frand()*M_PI;
		theta = 2.0*frand()*M_PI;

		_particles[i]->setPosition(0.0f, 10.0f, 0.0f);
		_particles[i]->setSpeed(v * cos(theta) * sin(phi), v * cos(phi), v * sin(theta) * sin(phi));
		_particles[i]->setAcceleration(0.1f, -0.15f, 0.0f);
		_particles[i]->setColor(0.882f, 0.552f, 0.211f);
		_particles[i]->setLife(0.7f);
		_particles[i]->setFade(0.005f);
	}
}

void GameManager::initLensFlares(){
	int i;
	float posDistance = 0.0f;
	
	for (i = 0; i < MAX_LENSFLARES; i++) {
		_lensFlare[i]->setPosition(-3.5f*i+8, 0.0f, -3.5f*i+8);
		//_lensFlare[MAX_LENSFLARES-i]->setPosition(-3.5f*i+8, 0.0f, -3.5f*i+8);
		_lensFlare[i]->setFDistance(posDistance);
		posDistance += i/MAX_LENSFLARES;

		_lensFlare[i]->setFSize(0.4f);

	/*if(_lensFlare[i]->getTexture() == 0){
				_lensFlare[i]->setPosition(1.0f*i+2, 0.0f, 1.0f*i+2);
			}else if(_lensFlare[i]->getTexture() == 1){
				_lensFlare[i]->setPosition(1.0f*i+2, 0.0f, 1.0f*i+2);
			}else if(_lensFlare[i]->getTexture() == 2){
				_lensFlare[i]->setPosition(0.0f, 0.0f, 0.0f);
			}else if(_lensFlare[i]->getTexture() == 3){
				_lensFlare[i]->setPosition(0.0f, 0.0f, 0.0f);
			}*/
	}
}





// ------------------------------Init Function------------------------------

void GameManager::init(){

	// set the camera position based on its spherical coordinates
	//camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	//camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	//camY = r *   						     sin(beta * 3.14f / 180.0f);

	camX=0.0f;
	camY=20.0f;
	camZ=0.0f;
	atX=0.0f;
	atY=0.0f;
	atZ=0.0f;

	//Texture Object definition
	
	glGenTextures(20, TextureArray);
	TGA_Texture(TextureArray, "stone.tga", 0);
	TGA_Texture(TextureArray, "checker.tga", 1);
	TGA_Texture(TextureArray, "lightwood.tga", 2);
	TGA_Texture(TextureArray, "orange.tga", 3);
	TGA_Texture(TextureArray, "gameover.tga", 4);
	TGA_Texture(TextureArray, "pause.tga", 5);
	TGA_Texture(TextureArray, "manteiga.tga", 6);
	TGA_Texture(TextureArray, "cheerio.tga", 7);
	TGA_Texture(TextureArray, "candle.tga", 8);
	TGA_Texture(TextureArray, "tree.tga", 9);
	TGA_Texture(TextureArray, "juice.tga", 10);
	TGA_Texture(TextureArray, "rubik.tga", 11);
	TGA_Texture(TextureArray, "rubik2.tga", 12);
	TGA_Texture(TextureArray, "particula.tga", 13);
	TGA_Texture(TextureArray, "Flare1.tga", 14);
	TGA_Texture(TextureArray, "Flare2.tga", 15);
	TGA_Texture(TextureArray, "Flare3.tga", 16);
	TGA_Texture(TextureArray, "Flare4.tga", 17);

	int auxId;
	objId = 0;
	for(auxId=0; auxId<20; auxId++, objId++){
		if(auxId==3 || auxId==4 || auxId==5){
			objId=2;
			_gameObject[auxId]->create(mesh, &objId);
		}
		_gameObject[auxId]->create(mesh, &objId);
	}

	int auxId2;
	objId = 20;
	for (auxId2 = 0; auxId2<MAX_PARTICULAS; auxId2++){
		_particles[auxId2]->create(mesh, &objId);
	}
	initParticles();

	int auxId3;
	objId = 21;
	for (auxId3 = 0; auxId3<MAX_LENSFLARES; auxId3++){
		_lensFlare[auxId3]->create(mesh, &objId);
	}
	initLensFlares();
	
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

