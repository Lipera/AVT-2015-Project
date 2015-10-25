#include "StaticObject.h"

#ifndef CANDLE_H
#define CANDLE_H


class Candle : public StaticObject{

public:
    Candle();
    ~Candle();
	void create(struct MyMesh* mesh);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};

#endif