#include "Vector4.h"

Vector4::Vector4(){
	_w = 0;
    Vector3();
}

Vector4::Vector4(double x, double y, double z, double w){
	_w = w;
    Vector3 (x, y, z);
}

Vector4::~Vector4(){
	//nothing to do
}

double Vector4::getW(){
	return _w;
}

void Vector4::set(double x, double y, double z, double w){
	_w = w;
    Vector3::set(x, y, z);
}

Vector4* Vector4::operator=(Vector4 &vec){
	_x = (&vec)->getX();
    _y = (&vec)->getY();
    _z = (&vec)->getZ();
    _w = (&vec)->getW();
    Vector4* vec_aux = new Vector4(_x, _y, _z, _w);
    return vec_aux;
}

Vector4* Vector4::operator*(double num){
	_x = _x * num;
    _y = _y * num;
    _z = _z * num;
    _w = _w * num;
    Vector4* vec_aux = new Vector4(_x, _y, _z, _w);
    return vec_aux;
}

Vector4* Vector4::operator+(Vector4 &vec){
	_x = _x + (&vec)->getX();
    _y = _y + (&vec)->getY();
    _z = _z + (&vec)->getZ();
    _w = _w + (&vec)->getW();
    Vector4* vec_aux = new Vector4(_x, _y, _z, _w);
    return vec_aux;
}

Vector4* Vector4::operator-(Vector4 &vec){
	_x = _x - (&vec)->getX();
    _y = _y - (&vec)->getY();
    _z = _z - (&vec)->getZ();
    _w = _w - (&vec)->getW();
    Vector4* vec_aux = new Vector4(_x, _y, _z, _w);
    return vec_aux;
}