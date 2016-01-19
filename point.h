#ifndef POINT_H
#define POINT_H
#include <iostream>
#include "definitions.h"

class point
{
    int _x;
    int _y;
    int _player_id;
    char _symbol;
public:
    point(int x, int y, char symbol, int player_id);
    void draw();
    inline int x() { return _x; }
    inline int y() { return _y; }
    inline int player_id() { return _player_id; }
    inline void step(coord c)
    {
        _x = c.first;
        _y = c.second;
    }
};

#endif // POINT_H
