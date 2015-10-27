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

void GameObject::setAngle(int angle){
	_angle = angle;
}
int GameObject::getAngle(){
	return _angle;
}

