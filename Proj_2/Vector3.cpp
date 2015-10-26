#include "Vector3.h"

Vector3::Vector3(){
	_x = 0;
    _y = 0;
    _z = 0;
}

Vector3::Vector3(double x, double y, double z){
	_x = x;
    _y = y;
    _z = z;
}

Vector3::~Vector3(){
	//nothing to do
}

double Vector3::getX(){
	return _x;
}

double Vector3::getY(){
	return _y;
}

double Vector3::getZ(){
	return _z;
}

void Vector3::set(double x, double y, double z){
	_x = x;
    _y = y;
    _z = z;
}

void Vector3::setX(double x) {
	_x = x;
}

void Vector3::setY(double y) {
	_y = y;
}

void Vector3::setZ(double z) {
	_z = z;
}

Vector3* Vector3::operator=(Vector3 &vec){
	_x = (&vec)->getX();
    _y = (&vec)->getY();
    _z = (&vec)->getZ();
    return this;
}

Vector3* Vector3::operator*(double num){
	double x = _x*num;
    double y = _y*num;
    double z = _z*num;
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}

Vector3* Vector3::operator+(Vector3 &vec){
	double x = _x + (&vec)->getX();
    double y = _y + (&vec)->getY();
    double z = _z + (&vec)->getZ();
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}

Vector3* Vector3::operator-(Vector3 &vec){
	double x = _x - (&vec)->getX();
    double y = _y - (&vec)->getY();
    double z = _z - (&vec)->getZ();
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}