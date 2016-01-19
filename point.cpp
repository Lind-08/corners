#include "point.h"

point::point(int x, int y, char symbol, int player_id)
{
    _x = x;
    _y = y;
    _player_id = player_id;
    _symbol = symbol;
}

void point::draw()
{
    std::cout.width(3);
    std::cout << _symbol;
}

