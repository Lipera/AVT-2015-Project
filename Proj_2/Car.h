#include "DynamicObject.h"

#ifndef CAR_H
#define CAR_H

class Car : public DynamicObject{
    
public:
    Car();
    ~Car();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};

#endif