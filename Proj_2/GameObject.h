#include "Entity.h"
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject: public Entity {
protected:
	int _angle;
public:
    GameObject();
    ~GameObject();
	void setAngle(int angle);
	int getAngle();
	virtual void create(struct MyMesh* mesh, int *objId);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif