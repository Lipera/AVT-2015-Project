
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "basic_geometry.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {

public:
    GameObject();
    ~GameObject();
	virtual void create(struct MyMesh* mesh, int *objId);
	virtual void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif