#include "Entity.h"

Entity::Entity(){
    //nothig to do
}

Entity::~Entity(){
    //nothing to do
}

Vector3* Entity::getPosition(){
    return _position;
}

Vector3* Entity::setPosition(float x, float y, float z){
    _position->set(x, y, z);
    return _position;
}

Vector3* Entity::setPosition(Vector3 &p){
    Vector3 vec_aux = *new Vector3();
    _position->set((&p)->getX(), (&p)->getY(), (&p)->getZ());
    return _position;
}
