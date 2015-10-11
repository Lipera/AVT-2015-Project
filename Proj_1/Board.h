#include "StaticObject.h"

#ifndef BOARD_H
#define BOARD_H

//classe do tabuleiro
class Board : public StaticObject{
    
public:
    Board();
    ~Board();
    void draw();
};

#endif