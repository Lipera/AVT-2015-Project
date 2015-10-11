#include "DynamicObject.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H

//classe obstaculos
class Obstacle : public DynamicObject{
    
public:
    Obstacle();
    ~Obstacle();
    void draw();
};

#endif