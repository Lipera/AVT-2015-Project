#include "GameObject.h"
#include "Vector3.h"

#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

class DynamicObject : public GameObject {

protected:
    Vector3* _speed;

public:
    DynamicObject();
    ~DynamicObject();
    virtual void create(struct MyMesh* mesh, int *objId);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	virtual void update(int delta_t);
    Vector3* setSpeed(Vector3 &speed);
    Vector3* setSpeed(float x, float y, float z);
    Vector3* getSpeed();
};

#endif