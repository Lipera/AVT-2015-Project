#include "DynamicObject.h"

#ifndef CAR_H
#define CAR_H

class Car : public DynamicObject{
private:
	float _varAngle;
	float _aceleracao;
	float _velocidade;
	float _varVelocidade;
public:
    Car(float x, float y, float z);
    ~Car();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	void update(int delta_t);
	void setAceleracao(float aceleracao);
	float getAceleracao();
	void setVelocidade(float velocidade);
	float getVelocidade();
	void setVarAngle(float varAngle);
	float getVarAngle();
	void setVarVelocidade(float varVelocidade);
	float getVarVelocidade();
	void updateAux(bool keyQ, bool keyA, bool keyO, bool keyP, int delta_t);
};

#endif