#include "StaticObject.h"

#ifndef BOARD_H
#define BOARD_H


class Board : public StaticObject {

public:
    Board();
    ~Board();
	void create(struct MyMesh* mesh);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};


#endif