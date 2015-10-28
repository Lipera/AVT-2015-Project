#include "GameObject.h"

GameObject::GameObject(){
    //nothing to do

}


GameObject::~GameObject(){
    //nothing to do
}

void GameObject::create(struct MyMesh* mesh, int *objId){
    //nothing to do
}

void GameObject::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
    //nothing to do
}

void GameObject::setAngle(float angle){
	_angle = angle;
}
float GameObject::getAngle(){
	return _angle;
}

Vector3* GameObject::setbottomLeft(float x, float y, float z){
    _bottomLeft->set(x, y, z);
    return _bottomLeft;
}

Vector3* GameObject::getbottomLeft(){
    return _bottomLeft;
}

Vector3* GameObject::settopRight(float x, float y, float z){
    _topRight->set(x, y, z);
    return _bottomLeft;
}

Vector3* GameObject::gettopRight(){
    return _topRight;
}



