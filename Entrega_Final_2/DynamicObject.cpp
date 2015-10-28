#include "DynamicObject.h"

DynamicObject::DynamicObject(){
    // nothing to do
}

DynamicObject::~DynamicObject(){
    //nothing to do
}

void DynamicObject::create (struct MyMesh* mesh, int *objId){}
void DynamicObject::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){}

void DynamicObject::update(int delta_t){}

Vector3* DynamicObject::setSpeed(Vector3 &speed){
    _speed->set((&speed)->getX(), (&speed)->getY(), (&speed)->getZ());
    return _speed;
}

Vector3* DynamicObject::setSpeed(float x, float y, float z){
    _speed->set(x, y, z);
    return _speed;
}

Vector3* DynamicObject::getSpeed(){
    return _speed;
}

