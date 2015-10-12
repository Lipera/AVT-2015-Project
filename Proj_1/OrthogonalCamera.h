#include "Camera.h"

#ifndef ORTOGONALCAMERA_H
#define ORTOGONALCAMERA_H

class OrthogonalCamera : public Camera {
private:
    float _left;
    float _right;
    float _bottom;
    float _top;
	float _nearp;
	float _farp;
    
public:
    OrthogonalCamera(float left, float right, float bottom, float top, float near, float far);
    ~OrthogonalCamera();
	void computeProjectionMatrix();
};

#endif