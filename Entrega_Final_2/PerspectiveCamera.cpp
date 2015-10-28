#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera (float fov, float ratio, float nearp, float farp){
    _fov = fov;
	_ratio = ratio;
	_nearp = nearp;
	_farp = farp;
}

PerspectiveCamera::~PerspectiveCamera(){
//nothing to do
}


void PerspectiveCamera::computeProjectionMatrix(){
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

void PerspectiveCamera::computeVisualizationMatrix(int w, int h){
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

void PerspectiveCamera::setPosition(float x, float y, float z){}