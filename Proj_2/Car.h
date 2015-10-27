#include "DynamicObject.h"

#ifndef CAR_H
#define CAR_H

class Car : public DynamicObject{
    
public:
    Car(float x, float y, float z);
    ~Car();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	void update(int delta_t);
};

#endif