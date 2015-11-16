#include "StaticObject.h"

#ifndef SUN_H
#define SUN_H


class Sun : public StaticObject {

public:
    Sun();
    ~Sun();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif