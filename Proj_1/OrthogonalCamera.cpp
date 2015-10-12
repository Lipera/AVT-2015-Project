#include "OrthogonalCamera.h"

OrthogonalCamera::OrthogonalCamera(float left, float right, float bottom, float top, float near, float far){
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
    _nearp = near;
    _farp = far;
}

OrthogonalCamera::~OrthogonalCamera(){
    delete(this);
}

void OrthogonalCamera::computeProjectionMatrix(){
	loadIdentity(PROJECTION);
    ortho(_left, _right, _bottom, _top, _nearp, _farp);
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(0.0f, 20.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}