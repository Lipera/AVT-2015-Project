#include "Vector3.h"

Vector3::Vector3(){
	_x = 0;
    _y = 0;
    _z = 0;
}

Vector3::Vector3(float x, float y, float z){
	_x = x;
    _y = y;
    _z = z;
}

Vector3::~Vector3(){
	//nothing to do
}

float Vector3::getX(){
	return _x;
}

float Vector3::getY(){
	return _y;
}

float Vector3::getZ(){
	return _z;
}

void Vector3::set(float x, float y, float z){
	_x = x;
    _y = y;
    _z = z;
}

void Vector3::setX(float x) {
	_x = x;
}

void Vector3::setY(float y) {
	_y = y;
}

void Vector3::setZ(float z) {
	_z = z;
}

Vector3* Vector3::operator=(Vector3 &vec){
	_x = (&vec)->getX();
    _y = (&vec)->getY();
    _z = (&vec)->getZ();
    return this;
}

Vector3* Vector3::operator*(float num){
	float x = _x*num;
    float y = _y*num;
    float z = _z*num;
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}

Vector3* Vector3::operator+(Vector3 &vec){
	float x = _x + (&vec)->getX();
    float y = _y + (&vec)->getY();
    float z = _z + (&vec)->getZ();
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}

Vector3* Vector3::operator-(Vector3 &vec){
	float x = _x - (&vec)->getX();
    float y = _y - (&vec)->getY();
    float z = _z - (&vec)->getZ();
    Vector3* vec_aux = new Vector3(x, y, z);
    return vec_aux;
}