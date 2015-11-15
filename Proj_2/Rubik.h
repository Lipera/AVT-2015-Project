#include "StaticObject.h"

#ifndef RUBIK_H
#define RUBIK_H


class Rubik : public StaticObject {

public:
    Rubik();
    ~Rubik();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif