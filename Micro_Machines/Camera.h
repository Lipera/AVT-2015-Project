#include "AVTmathLib.h"
#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    
public:
    Camera();
    ~Camera();
    virtual void computeProjectionMatrix();
	virtual void setPosition(float x, float y, float z);
};

#endif