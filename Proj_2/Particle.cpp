#include "Particle.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor
Particle::Particle(){
    //to do
}

//destructor
Particle::~Particle(){
    //to do
}

//--------------------------------GETs e SETs------------------------------------

float Particle::getLife(){
	return _life;
}

float Particle::getFade(){
	return _fade;
}

Vector3 Particle::getColor(){
	return _color;
}

Vector3 Particle::getPosition(){
	return _position;
}

Vector3 Particle::getSpeed(){
	return _speed;
}

Vector3 Particle::getAcceleration(){
	return _acceleration;
}

void Particle::setLife(float life){
	_life = life;
}

void Particle::setFade(float fade){
	_fade = fade;
}

void Particle::setColor(float r, float g, float b){
	_color.set(r, g, b);
}

void Particle::setPosition(float x, float y, float z){
	_position.set(x, y, z);
}

void Particle::setSpeed(float vx, float vy, float vz){
	_speed.set(vx, vy, vz);
}

void Particle::setAcceleration(float x, float y, float z){
	_acceleration.set(x, y, z);
}


//--------------------------------------------------------------------------------


//desenha mesa
void Particle::create(struct MyMesh* mesh, int *objId){

	int texcount = 0;

	// create geometry and VAO of the quad for particles
	*objId = 20;
	mesh[*objId].mat.texCount = texcount;
	createQuad(2, 2);

}

void Particle::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	//to do
}
