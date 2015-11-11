#include <string>
#include <sstream>
#include "Entity.h"
#include "Vector4.h"

#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

class LightSource {

private:
	Vector4 *_ambient;
	Vector4 *_diffuse;
	Vector4 *_specular;
	Vector4 *_position;
	Vector4 *_spot_direction;
	float _cut_off;
	float _exponent;
	int _isEnabled;

	//Attenuation constants
	float _constantAtt;
    float _linearAtt;
	float _quadraticAtt;

public:
	LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position);
	LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position, float constantAtt, float linearAtt, float quadraticAtt);
	LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position, float constantAtt, float linearAtt, float quadraticAtt, float cut_off, float spot_exponent, Vector4 *spot_direction);
    ~LightSource();
	void setAmbient(Vector4 *ambient);
	void setDiffuse(Vector4 *diffuse);
	void setSpecular(Vector4 *specular);
	void setPosition(Vector4 *position);
	//void setDirection(Vector3 *direction);
	void setCutOff(float cut_off);
	void setExponent(float exponent);
	int getState();
	int changeState();

	float getConstantAtt();
	float getLinearAtt();
	float getQuadraticAtt();
	void setConstantAtt(float constantAtt);
	void setLinearAtt(float lineatAtt);
	void setQuadraticAtt(float quadraticAtt);
	
	void draw(int index, VSShaderLib& shader);

};

#endif
