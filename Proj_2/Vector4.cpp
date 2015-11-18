#include "Vector4.h"

Vector4::Vector4(){
	_w = 0;
    Vector3();
}

Vector4::Vector4(float x, float y, float z, float w){
	_w = w;
    _xyz = new Vector3(x, y, z);
}

Vector4::Vector4(Vector3* xyz, float w){
	_w = w;
    _xyz = xyz;
}


Vector4::~Vector4(){
	delete _xyz;
}

float Vector4::getX(){
	return _xyz->getX();
}

float Vector4::getY(){
	return _xyz->getY();
}

float Vector4::getZ(){
	return _xyz->getZ();
}

float Vector4::getW(){
	return _w;
}

void Vector4::set(float x, float y, float z, float w){
	_w = w;
    _xyz->set(x, y, z);
}

void Vector4::setW(float w) {
	_w = w;
}

Vector4* Vector4::operator=(Vector4 &vec){
	_xyz->set((&vec)->getX(), (&vec)->getY(), (&vec)->getZ());
	_w = vec.getW();
    return this;
}

Vector4* Vector4::operator*(float num){
	float x = _xyz->getX() * num;
    float y = _xyz->getY() * num;
    float z = _xyz->getZ() * num;
    float w = _w * num;
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}

Vector4* Vector4::operator+(Vector4 &vec){
	float x = _xyz->getX() + (&vec)->getX();
    float y = _xyz->getY() + (&vec)->getY();
    float z = _xyz->getZ() + (&vec)->getZ();
    float w = _w + (&vec)->getW();
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}

Vector4* Vector4::operator-(Vector4 &vec){
	float x = _xyz->getX() - (&vec)->getX();
    float y = _xyz->getY() - (&vec)->getY();
    float z = _xyz->getZ() - (&vec)->getZ();
    float w = _w - (&vec)->getW();
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}