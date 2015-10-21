#include "Vector3.h"

#ifndef ENTITY_H
#define ENTITY_H

class Entity {
    
public:
    Entity();
    ~Entity();
    Vector3* getPosition();
	Vector3* setPosition(double x, double y, double z);
	Vector3* setPosition(const Vector3 &p);
};

#endif