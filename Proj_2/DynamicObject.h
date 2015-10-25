#include "GameObject.h"

#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

class DynamicObject : public GameObject {
    
public:
    DynamicObject();
    ~DynamicObject();
    virtual void create(struct MyMesh* mesh);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};

#endif