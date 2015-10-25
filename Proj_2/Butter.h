#include "StaticObject.h"

#ifndef BUTTER_H
#define BUTTER_H

//classe da manteiga
class Butter : public StaticObject{
    
public:
    Butter();
    ~Butter();
    void create(struct MyMesh* mesh);
    void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};

#endif