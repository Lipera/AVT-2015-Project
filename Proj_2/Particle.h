#include "StaticObject.h"

#ifndef PARTICLE_H
#define PARTICLE_H

class Particle : public StaticObject {
private:
	float _life;
	float _fade;
	Vector3 _color; //r, g, b
	Vector3 _position; //x, y, z
	Vector3 _speed;//vx, vy, vz
	Vector3 _acceleration; //ax, ay, az

public:
    Particle();
    ~Particle();

	float getLife();
	float getFade();
	Vector3 getColor();
	Vector3 getPosition();
	Vector3 getSpeed();
	Vector3 getAcceleration();

	void setLife(float life);
	void setFade(float fade);
	void setColor(float r, float g, float b);
	void setPosition(float x, float y, float zn);
	void setSpeed(float vx, float vy, float vz);
	void setAcceleration(float ax, float ay, float az);

	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif
