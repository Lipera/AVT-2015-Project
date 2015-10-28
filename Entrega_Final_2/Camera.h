#include "AVTmathLib.h"
#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    
public:
    Camera();
    ~Camera();
    virtual void computeProjectionMatrix();
	virtual void computeVisualizationMatrix(int w, int h);
	virtual void setPosition(float x, float y, float z);
};

#endif