#include "DynamicObject.h"

#ifndef BUTTER_H
#define BUTTER_H

//classe da manteiga
class Butter : public DynamicObject{
    
public:
    Butter(float x, float y, float z);
    ~Butter();
    void create(struct MyMesh* mesh, int *objId);
    void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	void update(int delta_t);
};

#endif