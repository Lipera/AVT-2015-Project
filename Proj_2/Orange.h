#include "DynamicObject.h"
#include <math.h>

#ifndef ORANGE_H
#define ORANGE_H

//classe da laranja
class Orange : public DynamicObject{

public:
    Orange(float x, float y, float z);
    ~Orange();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	void update(int delta_t);

};

#endif