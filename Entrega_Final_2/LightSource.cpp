#include "LightSource.h"

#define DEFAULT_CUT_OFF 180

LightSource::LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position) {
	_diffuse = diffuse;
	_specular = specular;
	_position = position;
	_isEnabled = 0;

	_constantAtt = 1.0f;
	_linearAtt = 0.0f;
	_quadraticAtt = 0.0f;

	_cut_off = 180;
	_spot_direction = new Vector4(0.0f, 0.0f, 0.0f, -1.0f);
}

LightSource::LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position, float constantAtt, float linearAtt, float quadraticAtt) {
	_diffuse = diffuse;
	_specular = specular;
	_position = position;
	_constantAtt = constantAtt;
	_linearAtt = linearAtt;
	_quadraticAtt = quadraticAtt;
	_isEnabled = 0;

	_cut_off = 180;
	_spot_direction = new Vector4(0.0f, 0.0f, 0.0f, -1.0f);
}

LightSource::LightSource(Vector4 *diffuse, Vector4 *specular, Vector4 *position, float constantAtt, float linearAtt, float quadraticAtt, float cut_off, Vector4 *spot_direction) {
	_diffuse = diffuse;
	_specular = specular;
	_position = position;
	_constantAtt = constantAtt;
	_linearAtt = linearAtt;
	_quadraticAtt = quadraticAtt;
	_isEnabled = 0;

	_cut_off = cut_off;
	_spot_direction = spot_direction;
}

LightSource::~LightSource() {
	delete _ambient;
	delete _diffuse;
	delete _specular;
	delete _position;
	delete _spot_direction;
}

void LightSource::setAmbient(Vector4 *ambient) {
	_ambient = ambient;
}

void LightSource::setDiffuse(Vector4 *diffuse) {
	_diffuse = diffuse;
}

void LightSource::setSpecular(Vector4 *specular) {
	_specular = specular;
}

void LightSource::setPosition(Vector4 *position) {
	_position = position;
}

/*
void LightSource::setDirection(Vector3 direction) {
	_direction = direction;
}
*/

void LightSource::setCutOff(float cut_off) {
	_cut_off = cut_off;
}

void LightSource::setExponent(float exponent) {
	_exponent = exponent;
}

//GLenum LightSource::getNum() {}

int LightSource::getState() {
	return _isEnabled;
}

int LightSource::changeState() {
	_isEnabled = (_isEnabled + 1)%2;
	return _isEnabled;
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

void LightSource::draw(int index, VSShaderLib& shader) {
	GLint loc;
	float res[4];
	float resSpot[4];

	float position[4] = {_position->getX(),_position->getY(),_position->getZ(),_position->getW()};
	float diffuse[4] = {_diffuse->getX(),_diffuse->getY(),_diffuse->getZ(),_diffuse->getW()};
	float specular[4] = {_specular->getX(),_specular->getY(),_specular->getZ(),_specular->getW()};
	float spot_direction[4] = {_spot_direction->getX(), _spot_direction->getY(), _spot_direction->getZ(), _spot_direction->getW()};

	multMatrixPoint(VIEW, position, res);   //position defined in World Coord so it is converted to eye space
	multMatrixPoint(VIEW, spot_direction, resSpot);

/*
	std::string aux1("lights[");
	std::string aux2("].position");
	std::ostringstream oss;
	oss << aux1 << index << aux2;
	std::string aux3(oss.str());
	const char *LightShader = aux3.c_str();


	// send the lights
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform4fv(loc, 1, res);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform4fv(loc, 1, diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform4fv(loc, 1, specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform1f(loc, _constantAtt);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform1f(loc, _linearAtt);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform1f(loc, _quadraticAtt);
	loc = glGetUniformLocation(shader.getProgramIndex(), LightShader);
	glUniform1i(loc, _isEnabled);
*/


	switch(index) {
		case 0:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 1:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 2:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 3:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 4:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 5:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 6:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 7:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;

		case 8:
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].position");
				glUniform4fv(loc, 1, res);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].diffuse");
				glUniform4fv(loc, 1, diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].specular");
				glUniform4fv(loc, 1, specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].constantAtt");
				glUniform1f(loc, _constantAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].linearAtt");
				glUniform1f(loc, _linearAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].quadraticAtt");
				glUniform1f(loc, _quadraticAtt);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].isEnabled");
				glUniform1i(loc, _isEnabled);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].cutOff");
				glUniform1f(loc, _cut_off);
				loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotDirection");
				glUniform4fv(loc, 1, resSpot);
				break;
	}


}