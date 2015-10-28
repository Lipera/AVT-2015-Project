#include "StaticObject.h"

#ifndef PAUSE_H
#define PAUSE_H


class Pause : public StaticObject {

public:
    Pause();
    ~Pause();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif