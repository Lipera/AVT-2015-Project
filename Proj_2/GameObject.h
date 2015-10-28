#include "Entity.h"
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject: public Entity {
protected:
	float _angle;
	Vector3* _bottomLeft;
	Vector3* _topRight;

public:
    GameObject();
    ~GameObject();
	void setAngle(float angle);
	float getAngle();
	Vector3* setbottomLeft(float x, float y, float z);
	Vector3* settopRight(float x, float y, float z);
	Vector3* getbottomLeft();
	Vector3* gettopRight();
	virtual void create(struct MyMesh* mesh, int *objId);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif