#include "DynamicObject.h"

#ifndef ORANGE_H
#define ORANGE_H

//classe da laranja
class Orange : public DynamicObject{
    
public:
    Orange();
    ~Orange();
    void create(struct MyMesh* mesh);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};

#endif