#include "Vector4.h"

#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

class LightSource {

private:
	Vector4 _ambient;
	Vector4 _diffuse;
	Vector4 _specular;
	Vector4 _position;
	Vector3 _direction;
	double _cut_off;
	double _exponent;
	//GLenum _num;
	bool _state;

	//Attenuation constants
	float _constantAtt;
    float _linearAtt;
	float _quadraticAtt;

public:
	LightSource(/*GLenum number*/);
    ~LightSource();
	void setAmbient(Vector4 ambient);
	void setDiffuse(Vector4 diffuse);
	void setSpecular(Vector4 specular);
	void setPosition(Vector4 position);
	void setDirection(Vector3 direction);
	void setCutOff(double cut_off);
	void setExponent(double exponent);
	//GLenum getNum();
	bool getState();
	bool setState(bool state);

	float getConstantAtt();
	float getLinearAtt();
	float getQuadraticAtt();
	void setConstantAtt(float constantAtt);
	void setLinearAtt(float lineatAtt);
	void setQuadraticAtt(float quadraticAtt);
	
	void draw();

};

#endif
