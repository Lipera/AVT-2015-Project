#include "Vector4.h"

Vector4::Vector4(){
	_w = 0;
    Vector3();
}

Vector4::Vector4(double x, double y, double z, double w){
	_w = w;
    _xyz = new Vector3(x, y, z);
}

Vector4::~Vector4(){
	delete _xyz;
}

double Vector4::getX(){
	return _xyz->getX();
}

double Vector4::getY(){
	return _xyz->getY();
}

double Vector4::getZ(){
	return _xyz->getZ();
}

double Vector4::getW(){
	return _w;
}

void Vector4::set(double x, double y, double z, double w){
	_w = w;
    _xyz->set(x, y, z);
}

Vector4* Vector4::operator=(Vector4 &vec){
	_xyz->set((&vec)->getX(), (&vec)->getY(), (&vec)->getZ());
	_w = vec.getW();
    return this;
}

Vector4* Vector4::operator*(double num){
	double x = _xyz->getX() * num;
    double y = _xyz->getY() * num;
    double z = _xyz->getZ() * num;
    double w = _w * num;
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}

Vector4* Vector4::operator+(Vector4 &vec){
	double x = _xyz->getX() + (&vec)->getX();
    double y = _xyz->getY() + (&vec)->getY();
    double z = _xyz->getZ() + (&vec)->getZ();
    double w = _w + (&vec)->getW();
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}

Vector4* Vector4::operator-(Vector4 &vec){
	double x = _xyz->getX() - (&vec)->getX();
    double y = _xyz->getY() - (&vec)->getY();
    double z = _xyz->getZ() - (&vec)->getZ();
    double w = _w - (&vec)->getW();
    Vector4* vec_aux = new Vector4(x, y, z, _w);
    return vec_aux;
}