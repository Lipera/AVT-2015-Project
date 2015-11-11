#include "StaticObject.h"

#ifndef COASTER_H
#define COASTER_H


class Coaster : public StaticObject {

public:
    Coaster();
    ~Coaster();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif