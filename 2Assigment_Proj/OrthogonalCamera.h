#include "Camera.h"

#ifndef ORTOGONALCAMERA_H
#define ORTOGONALCAMERA_H

class OrthogonalCamera : public Camera {
private:
    float _left;
    float _right;
    float _bottom;
    float _top;
	float _near;
	float _far;
    
public:
    OrthogonalCamera(float left, float right, float bottom, float top, float near, float far);
    ~OrthogonalCamera();
	void computeProjectionMatrix();
	void computeVisualizationMatrix(int w, int h);
	void setPosition(float x, float y, float z);
};

#endif