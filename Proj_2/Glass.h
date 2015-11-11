#include "StaticObject.h"

#ifndef GLASS_H
#define GLASS_H


class Glass : public StaticObject {

public:
    Glass();
    ~Glass();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif