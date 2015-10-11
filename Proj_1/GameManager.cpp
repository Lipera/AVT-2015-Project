#include "GameManager.h"


//---------------------------------------------variables--------------------------------------------

// Camera Position
float camX, camY, camZ;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 25.0f;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

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

//mesh variables
struct MyMesh mesh[961];

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};

//--------------------------------------Constructor and Destructor--------------------------------------

GameManager::GameManager(){
    //to do
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

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	perspective(53.13f, ratio, 0.1f, 1000.0f);
}

// -------------------------------------Timer--------------------------------------------

void GameManager::timer(int value){

	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << _WinX << "x" << _WinY << ")";
	std::string s = oss.str();
	glutSetWindow(_WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
}

// -------------------------------------Refresh--------------------------------------------

void GameManager::refresh(int value)
{
	glutPostRedisplay();
}

// -----------------------------------------------------------------------------------------
// Events from the Keyboard
//------------------------------------------------------------------------------------------

void GameManager::processKeys(unsigned char key, int xx, int yy)
{
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;

		case 'c': 
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
			break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
	}
}


// ----------------------------------------------------------------------------------------
// Mouse Events
//-----------------------------------------------------------------------------------------

void GameManager::processMouseButtons(int button, int state, int xx, int yy){
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


// -------------------------Track mouse motion while buttons are pressed-------------------------

void GameManager::processMouseMotion(int xx, int yy){

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

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}


void GameManager::mouseWheel(int wheel, int direction, int x, int y) {
	//velocidade do zoom
	r += direction * 0.75f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

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
	lookAt(camX, camY, camZ, 0,0,0, 0,1,0);
	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates
		//luz como capacete de mineiro, que acompanha o movimento
		glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

		//luz esta sempre no mesmo ponto

		//float res[4];
		//multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
		//glUniform4fv(lPos_uniformId, 1, res);

	objId=0;
	for (int i = 0 ; i < 31; ++i) {
		for (int j = 0; j < 31; ++j) {
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
			translate(MODEL, -15.5 + i*1.0f, -0.5f, -15.5 + j*1.0f);

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


	float amb[]= {0.1f, 0.1f, 0.1f, 1.0f};
	float diff[] = {0.2f, 0.7f, 0.9f, 1.0f};
	float spec[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float emissive[] = {0.3f, 0.8f, 0.4f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;
	
	float amb1[]= {0.1f, 0.1f, 0.1f, 1.0f};
	float diff1[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float spec1[] = {1.0f, 1.0f, 1.0f, 1.0f};
	shininess=500.0;

	//criacao do tabuleiro

	//cria cubos pares da malha
	int aux1;
	for(aux1=0; aux1 < 961; aux1=aux1+2){
		objId=aux1;
		memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();
	}

	//cria cubos impares da malha
	int aux2;
	for(aux2=1; aux2 < 961; aux2=aux2+2){
		objId=aux2;
		memcpy(mesh[objId].mat.ambient, amb1,4*sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff1,4*sizeof(float));
		memcpy(mesh[objId].mat.specular, spec1,4*sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();
	}


	//criacao da pista
	
	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

}