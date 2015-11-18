#include <math.h>

#include "AVTmathLib.h"

#include "StaticObject.h"
#include "maths.h"

#ifndef BILLBOARD_H
#define BILLBOARD_H

class Coin : public StaticObject {

private:
	float _camX;
	float _camY;
	float _camZ;
	float _posX;
	float _posY;
	float _posZ;
	int _reflection;
	int _type;
	int _number;

public:
	Coin(int n);
	~Coin();
	void setReflection(int reflection);
	float getCamX();
	float getCamY();
	float getCamZ();
	void setCam(float camX, float camY, float camZ);
	float getPosX();
	float getPosY();
	float getPosZ();
	void setPos(float posX, float posY, float posZ);
	void setType(int i);
	int getType();
	void setNumber(int i);
	int getNumber();
	//void l3dBillboardGetRightVector(float *right);
	//void l3dBillboardGetUpRightVector(float *up, float *right);
	void l3dBillboardLocalToWorld(float *cam, float *worldPos);
	void l3dBillboardCylindricalBegin(float *cam, float *worldPos);
	void l3dBillboardSphericalBegin(float *cam, float *worldPos);
	void BillboardCheatSphericalBegin();
	void BillboardCheatCylindricalBegin();
	void BillboardEnd(float *m);
	void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);

};

#endif