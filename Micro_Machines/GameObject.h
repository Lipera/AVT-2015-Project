#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


class GameObject{
    
public:
    GameObject();
    ~GameObject();
    virtual void draw();
    virtual void update(double delta_t);
};

#endif