#include "DynamicObject.h"
#include <math.h>

#ifndef CHEERIOS_H
#define CHEERIOS_H

class Cheerios : public DynamicObject{
private:
	float _velocidade;
public:
    Cheerios(float x, float y, float z);
    ~Cheerios();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	void update(int delta_t);
	void setVelocidade(float velocidade);
	float getVelocidade();
	void setAngle(float varAngle);
	float getAngle();
};

#endif