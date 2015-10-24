#include "Camera.h"

#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

class PerspectiveCamera : public Camera {
private:
    float _fovy;
	float _ratio;
	float _nearp;
	float _farp;
    
public:
    PerspectiveCamera (float fovy, float ratio, float nearp, float farp);
    ~PerspectiveCamera();
	void computeProjectionMatrix();
	void setPosition(float x, float y, float z);
};

#endif