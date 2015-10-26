#include "GameManager.h" //the other includes are in GameManager.h

//-----------------------------------Variables---------------------------------

int WinX = 640, WinY = 480;

//-----------------------------------Function---------------------------------

//declaration of object GameManager
GameManager *manager;

void reshape(int w, int h){
	manager->reshape(w, h);
}

void refresh(int value){
	manager->refresh(value);
	glutTimerFunc(1000/60, refresh, 0);
}

void processKeys(unsigned char key, int xx, int yy){
	manager->processKeys(key, xx, yy);
}

void processKeysUp(unsigned char key, int xx, int yy){
	manager->processKeysUp(key, xx, yy);
}

void processMouseButtons(int button, int state, int xx, int yy){
	manager->processMouseButtons(button, state, xx, yy);
}

void processMouseMotion(int xx, int yy){
	manager->processMouseMotion(xx, yy);
}

void mouseWheel(int wheel, int direction, int x, int y){
	manager->mouseWheel(wheel, direction, x, y);
}

void renderScene(void){
	manager->renderScene();
}

void init(void){
	manager->init();
}

void timer(int value){
	manager->timer(value);
    glutTimerFunc(1000/60, timer, 0);
}

GLuint setupShaders() {
	return manager->setupShaders();
}


//----------------------------------------------------------------------------
//---------------------------------Main Function------------------------------
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
	
	//creation of object GameManager
	manager = new GameManager();
	manager->setWinX(WinX);
	manager->setWinY(WinY);

//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE );

	glutInitContextVersion ( 3, 3 );
	glutInitContextProfile ( GLUT_CORE_PROFILE );
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );

	glutInitWindowPosition( 100,100 );
	glutInitWindowSize( WinX, WinY );
	manager->setWindowHandle(glutCreateWindow(CAPTION));

//  Callback Registration
	glutDisplayFunc( renderScene );
	glutReshapeFunc( reshape );
	//glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc( processKeys );
	glutKeyboardUpFunc( processKeysUp );
	glutMouseFunc( processMouseButtons );
	glutMotionFunc( processMouseMotion );
	glutMouseWheelFunc ( mouseWheel ) ;
	glutTimerFunc( 0,timer,0 );
	glutTimerFunc( 0, refresh, 0 );


//	return from main loop
	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);
}

