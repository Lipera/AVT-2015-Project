#include "StaticObject.h"

#ifndef GAMEOVER_H
#define GAMEOVER_H


class GameOver : public StaticObject {

public:
    GameOver();
    ~GameOver();
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif