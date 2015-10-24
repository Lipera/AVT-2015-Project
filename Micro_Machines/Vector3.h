#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {

protected:
	double _x;
	double _y;
	double _z;
    
public:
    Vector3();
	Vector3(double x, double y, double z);
    ~Vector3();
	double getX();
	double getY();
	double getZ();
	void set(double x, double y, double z);
	void setX(double x);
	void setY(double y);
	void setZ(double z);
    Vector3* operator=(Vector3 &vec);
	Vector3* operator*(double num);
	Vector3* operator+(Vector3 &vec);
	Vector3* operator-(Vector3 &vec);

};

#endif