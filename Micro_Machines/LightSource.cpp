#include "LightSource.h"

LightSource::LightSource(/*GLenum number*/) {

}

LightSource::~LightSource() {

}

void LightSource::setAmbient(Vector4 ambient) {

}

void LightSource::setDiffuse(Vector4 diffuse) {

}

void LightSource::setSpecular(Vector4 specular) {

}

void LightSource::setPosition(Vector4 position) {

}

void LightSource::setDirection(Vector3 direction) {

}

void LightSource::setCutOff(double cut_off) {

}

void LightSource::setExponent(double exponent) {

}

//GLenum LightSource::getNum() {}

bool LightSource::getState() {
	return _state;
}

bool LightSource::setState(bool state) {
	_state = state;
    return _state;
}

void LightSource::draw() {

}
