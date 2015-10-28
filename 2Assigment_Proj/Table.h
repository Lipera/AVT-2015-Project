#include "StaticObject.h"

#ifndef TABLE_H
#define TABLE_H


class Table : public StaticObject{

public:
    Table();
    ~Table();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};

#endif