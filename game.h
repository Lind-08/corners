#ifndef GAME_H
#define GAME_H
#include <map>
#include <stack>
#include <algorithm>
#include <string>
#include <memory>
#include "point.h"
#include "definitions.h"

using namespace std;


class game
{
    int _width;
    int _height;
    int _cellsCount;
    int _fpCount = 0;
    int _spCount = 0;
    char _emptysymbol = '.';
    char _player1symbol = 'O';
    char _player2symbol = '@';
    bool currentPlayer = false;
    bool finished = false;
    int penalty = 0;
    int winner = 0;
    map<coord,shared_ptr<point>> points;

    bool checkAllowCoord(int x, int y);
    bool isAccessiableStep(pair<coord,coord> player_step);
    bool isAccessiableHop(pair<coord,coord> player_step);
    bool isCanFindPath(pair<coord,coord> player_step);
    bool checkGameState();
    bool checkPointPlayer(coord);
    pair<coord,coord> getPlayerStep();
    bool step(pair<coord,coord>);
    void changeCoordinates(coord new_c, point* p);

    int calcCellsCountInField(bool player_id);
    void mainLoop();
    void drawGameMap();
    void finishGame();
public:
    game(int cellsCount);
    void startGame();
};

#endif // GAME_H
