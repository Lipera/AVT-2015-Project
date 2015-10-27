#include "LightSource.h"

LightSource::LightSource(/*GLenum number*/) {

}

LightSource::~LightSource() {

}

void LightSource::setAmbient(Vector4 ambient) {
	_ambient = ambient;
}

void LightSource::setDiffuse(Vector4 diffuse) {
	_diffuse = diffuse;
}

void LightSource::setSpecular(Vector4 specular) {
	_specular = specular;
}

void LightSource::setPosition(Vector4 position) {
	_position = position;
}

void LightSource::setDirection(Vector3 direction) {
	_direction = direction;
}

void LightSource::setCutOff(double cut_off) {
	_cut_off = cut_off;
}

void LightSource::setExponent(double exponent) {
	_exponent = exponent;
}

//GLenum LightSource::getNum() {}

bool LightSource::getState() {
	return _state;
}

bool LightSource::setState(bool state) {
	_state = state;
    return _state;
}

float LightSource::getConstantAtt() {
	return _constantAtt;
}

float LightSource::getLinearAtt() {
	return _linearAtt;
}

float LightSource::getQuadraticAtt() {
	return _quadraticAtt;
}

void LightSource::setConstantAtt(float constantAtt) {
	_constantAtt = constantAtt;
}

void LightSource::setLinearAtt(float linearAtt) {
	_linearAtt = linearAtt;
}

void LightSource::setQuadraticAtt(float quadraticAtt) {
	_quadraticAtt = quadraticAtt;
}

void LightSource::draw() {

}
