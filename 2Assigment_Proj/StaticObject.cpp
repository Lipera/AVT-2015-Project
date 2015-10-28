#include "StaticObject.h"

StaticObject::StaticObject (){
    //nothing to do
}

StaticObject::~StaticObject (){
    //nothing to do
}

void StaticObject::create (struct MyMesh* mesh, int *objId){}
void StaticObject::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){}