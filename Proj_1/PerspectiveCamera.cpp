#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera (float fovy, float ratio, float nearp, float farp){
    _fovy = fovy;
	_ratio = ratio;
	_nearp = nearp;
	_farp = farp;
}

PerspectiveCamera::~PerspectiveCamera(){
//nothing to do
}


void PerspectiveCamera::computeProjectionMatrix(){
	loadIdentity(PROJECTION);
    perspective(_fovy, _ratio, _nearp, _farp);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void PerspectiveCamera::setPosition(float x, float y, float z){}