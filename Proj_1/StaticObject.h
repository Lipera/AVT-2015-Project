#include "GameObject.h"

#ifndef STATICOBJECT_H
#define STATICOBJECT_H

class StaticObject : public GameObject {
    
public:
    StaticObject();
    ~StaticObject();
    virtual void draw();
};

#endif