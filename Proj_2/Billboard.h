#include <math.h>

#include "AVTmathLib.h"

#include "maths.h"

#ifndef BILLBOARD_H
#define BILLBOARD_H

class Billboard {

public:
	void billboardGetRightVector(float *right);
	void billboardGetUpRightVector(float *up, float *right);
	void billboardLocalToWorld(float *cam, float *worldPos);
	void billboardCylindricalBegin(float *cam, float *worldPos);
	void billboardSphericalBegin(float *cam, float *worldPos);
	void billboardCheatSphericalBegin(void);
	void billboardCheatCylindricalBegin(void);
	void billboardEnd(void);
};

#endif