#include "Vector3.h"
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "basic_geometry.h"

#ifndef ENTITY_H
#define ENTITY_H

class Entity{
protected:
    Vector3* _position;
    
public:
    Entity();
    ~Entity();
    Vector3* getPosition();
    Vector3* setPosition(double x, double y, double z);
    Vector3* setPosition(Vector3 &p);
};

#endif