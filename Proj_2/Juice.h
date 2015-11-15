#include "StaticObject.h"

#ifndef JUICE_H
#define JUICE_H


class Juice : public StaticObject {

public:
    Juice();
    ~Juice();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif