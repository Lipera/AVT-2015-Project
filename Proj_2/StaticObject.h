#include "GameObject.h"

#ifndef STATICOBJECT_H
#define STATICOBJECT_H

class StaticObject : public GameObject {
    
public:
    StaticObject();
    ~StaticObject();
    virtual void create(struct MyMesh* mesh);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};

#endif