#include "StaticObject.h"
#include <math.h>

#ifndef LENS_FLARE_H
#define LENS_FLARE_H

//classe da laranja
class LensFlare : public StaticObject{

public:
    LensFlare();
    ~LensFlare();
    void create(struct MyMesh* mesh, int *objId);
	void render_flare(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);

};

#endif