#include "StaticObject.h"

#ifndef __TRACK_H__
#define __TRACK_H__

class Track : public StaticObject{
    
public:
    Track();
    ~Track();
    void create(struct MyMesh* mesh);
	void draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId);
};


#endif