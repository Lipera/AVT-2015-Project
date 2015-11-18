#include "StaticObject.h"
#include "Vector4.h"
#include <math.h>

#ifndef LENS_FLARE_H
#define LENS_FLARE_H

//classe da laranja
class LensFlare : public StaticObject{

private:
	float _fDistance;        // Distance along ray from source (0.0-1.0)
    float _fSize;            // Size relative to flare envelope (0.0-1.0)
	int _texture;
	float _width;
	float _height;
	Vector3 _position;
	Vector4 _color;


public:
    LensFlare(int value);
    ~LensFlare();
	void setTexture(int value);
	int getTexture();
	float getWidth();
	void setWidth(float width);
	float getHeight();
	void setHeight(float height);
	void setPosition(float x, float y, float z);
	Vector3 getPosition();
	void setColor(float r, float g, float b, float a);
	void setAlpha(float a);
	Vector4 getColor();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	
	float getFDistance();
	float getFSize();
};

#endif