#include "StaticObject.h"

#ifndef POT_H
#define POT_H


class Pot : public StaticObject {

public:
    Pot();
    ~Pot();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif