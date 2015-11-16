#include "Vector3.h"

#ifndef VECTOR4_H
#define VECTOR4_H

class Vector4 {

private:
	Vector3 *_xyz;
	float _w;
    
public:
    Vector4();
	Vector4(float x, float y, float z, float w);
    ~Vector4();
	float getX();
	float getY();
	float getZ();
	float getW();
	void set(float x, float y, float z, float w);
    Vector4* operator=(Vector4 &vec);
	Vector4* operator*(float num);
	Vector4* operator+(Vector4 &vec);
	Vector4* operator-(Vector4 &vec);

};

#endif