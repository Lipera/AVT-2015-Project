#include "GameObject.h"

#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

class DynamicObject : public GameObject {
    
public:
    DynamicObject();
    ~DynamicObject();
    virtual void draw();
};

#endif