#include "StaticObject.h"

#ifndef __BOARD_H__
#define __BOARD_H__


class Board : public StaticObject {

public:
    Board();
    ~Board();
	void create(struct MyMesh* mesh);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};


#endif