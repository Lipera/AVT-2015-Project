#include "StaticObject.h"
#include <math.h>

#ifndef LENS_FLARE_H
#define LENS_FLARE_H

//classe da laranja
class LensFlare : public StaticObject{

private:
	float _fScale;		// Scale factor for adjusting overall size of flare elements.
	float _fMaxSize;	// Max size of largest element, as proportion of screen width (0.0-1.0)
	int _nPieces;		// Number of elements in use
	int _texture;
	Vector3 _position;
	Vector3 _color;

public:
    LensFlare(int value);
    ~LensFlare();
	void setTexture(int value);
	int getTexture();
	void setPosition(float x, float y, float z);
	Vector3 getPosition();
	void setColor(float r, float g, float b);
	Vector3 getColor();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
	
};

#endif