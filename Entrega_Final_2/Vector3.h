#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {

protected:
	float _x;
	float _y;
	float _z;
    
public:
    Vector3();
	Vector3(float x, float y, float z);
    ~Vector3();
	float getX();
	float getY();
	float getZ();
	void set(float x, float y, float z);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
    Vector3* operator=(Vector3 &vec);
	Vector3* operator*(float num);
	Vector3* operator+(Vector3 &vec);
	Vector3* operator-(Vector3 &vec);

};

#endif