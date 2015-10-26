#include "OrthogonalCamera.h"

OrthogonalCamera::OrthogonalCamera(float left, float right, float bottom, float top, float near, float far){
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
    _near = near;
    _far = far;
}

OrthogonalCamera::~OrthogonalCamera(){
    delete(this);
}

void OrthogonalCamera::computeProjectionMatrix(){
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

void OrthogonalCamera::computeVisualizationMatrix(int w, int h){
	if(h == 0)
		h = 1;
	loadIdentity(PROJECTION);
	float ratio = (_right - _left) / (_top - _bottom);
		float aspect = (1.0f * w) / h;

		if (ratio < aspect) {
			float delta = ((_top - _bottom) * aspect - (_right - _left)) / 2;
			ortho(_left - delta, _right + delta, _bottom, _top, _near, _far);
		}
    
		else {
			float delta = ((_right - _left) / aspect - (_top - _bottom)) / 2;
			ortho(_left, _right, _bottom - delta, _top + delta, -20.0f, 40.0f);
		}
}

void OrthogonalCamera::setPosition(float x, float y, float z){}