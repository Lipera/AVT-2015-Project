#include "DynamicObject.h"

#ifndef CAR_H
#define CAR_H

//class do carro
class Car : public DynamicObject{
    
public:
    Car();
    ~Car();
    void draw();
};

#endif