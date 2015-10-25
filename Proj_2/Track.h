#include "StaticObject.h"

#ifndef TRACK_H
#define TRACK_H

class Track : public StaticObject{
    
public:
    Track();
    ~Track();
    void create(struct MyMesh* mesh, int *objId);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId);
};


#endif