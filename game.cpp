#include "game.h"

game::game(int cellsCount)
{
    _width = cellsCount;
    _height = cellsCount;
    _cellsCount = cellsCount * cellsCount;
    for (int i = 0; i < _height; i++)
        for (int j = 0; j < _width; j++)
        {
            auto p = make_shared<point>(j,i,_player1symbol,false);
            points.insert({make_pair(j,i),p});
        }
    for (int i = _height; i < (_height * 2); i++)
        for (int j = _width; j < (_width * 2); j++)
        {
            auto p = make_shared<point>(j,i,_player2symbol,true);
            points.insert({make_pair(j,i),p});
        }
}

bool game::checkAllowCoord(int x, int y)
{
    if ((x >= _width && x < 2*_width) || (y >= _height && y < 2*_height))
        return true;
    else
        return false;
}

bool game::isAccessiableStep(pair<coord, coord> player_step)
{
    int delta_x = abs(player_step.first.first - player_step.second.first);
    int delta_y = abs(player_step.first.second - player_step.second.second);
    if ((delta_x + delta_y) > 1)
        return false;
    try
    {
        points.at(player_step.second);
    }
    catch (...)
    {
        return true;
    }
    return false;
}

bool game::isAccessiableHop(pair<coord, coord> player_step)
{
    int delta_x = abs(player_step.first.first - player_step.second.first);
    int delta_y = abs(player_step.first.second - player_step.second.second);
    if ((delta_x + delta_y) > 2)
        return false;
    coord c_temp(player_step.first);
    if (delta_x > delta_y)
        if (player_step.first.first > player_step.second.first)
            c_temp.first -= 1;
        else
            c_temp.first += 1;
    else
        if (player_step.first.second > player_step.second.second)
            c_temp.second -= 1;
        else
            c_temp.second += 1;
    try
    {
        auto p = points.at(c_temp);
        if (isAccessiableStep(make_pair(c_temp,player_step.second)))
            return true;
        else
            return false;
    }
    catch(...)
    {
        return false;
    }
}

bool game::isCanFindPath(pair<coord, coord> player_step)
{
    coord current_p = player_step.first;
    stack<pair<int,coord> > path;
    int dir = 0;
    coord new_p(current_p);
    while (true)
    {
        switch (dir)
        {
        case 0:
            new_p.second += 2; break;
        case 1:
            new_p.first += 2;
           break;
        case 2:
            new_p.second -= 2; break;
        case 3:
            new_p.first -= 2; break;
        default:
           {
                if (current_p == player_step.first)
                    return false;
                auto tmp = path.top();
                path.pop();
                dir = tmp.first + 1;
                current_p = tmp.second;
                new_p = tmp.second;
                continue;
            }
        }
        if (new_p != path.top().second)
        {
            if (isAccessiableHop(make_pair(current_p,new_p)))
            {
                if (new_p == player_step.first)
                {
                    auto tmp = path.top();
                    path.pop();
                    dir = tmp.first + 1;
                    current_p = tmp.second;
                    continue;
                }
                if (new_p == player_step.second)
                    return true;
                else
                {
                    path.push(make_pair(dir,current_p));
                    current_p = new_p;
                    dir = 0;
                    continue;
                }
            }
        }
        dir += 1;
        new_p = current_p;
    }
}

void drawEmptyPoint()
{
    cout.width(3);
    cout << ".";
}

bool game::checkPointPlayer(coord c)
{
    try
    {
        if (points.at(c)->player_id() == currentPlayer)
            return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

bool game::checkGameState()
{
    _fpCount = calcCellsCountInField(false);
    _spCount = calcCellsCountInField(true);
    int dif = abs(_fpCount - _spCount);
    int res = currentPlayer ? _spCount : _fpCount;
    if (res == _cellsCount)
    {
        if (winner == 0)
        {
            winner = currentPlayer ? 2 : 1;
            currentPlayer = !currentPlayer;
        }
        if (dif != 1)
            return true;
        else
            return false;
    }
    else
        return false;
}

int game::calcCellsCountInField(bool player_id)
{
    int count = 0;
    int first_x = player_id ? 0: _width;
    int first_y = player_id ? 0 : _height;
    for (int i = first_x; i < first_x + _width; i++)
        for (int j = first_y; j < first_y + _height; j++)
        {
            try
            {
                if (points.at(make_pair(i,j))->player_id() == player_id)
                    count++;
            }
            catch (...)
            {
                continue;
            }
        }
    return count;
}

void game::drawGameMap()
{
    cout << "  ";
    for (int i = 0; i < _width * 2; i++)
    {
        std::cout.width(3);
        cout << i;
    }
    cout << endl;
    for (int i = 0; i < _height * 2; i++)
    {
        cout.width(2);
        cout << i;
        for (int j = 0; j < _width * 2; j++)
        {
            shared_ptr<point> p;
            try
            {
                p = points.at(make_pair(j,i));
            }
            catch (...)
            {
                drawEmptyPoint();
                continue;
            };
                p->draw();
        }
        cout << endl;
    }
    cout << "Player 1: " << _player1symbol << " Player 2: " << _player2symbol << endl;
    cout << "Cells in players field: Player 1 - " << _fpCount << " Player 2 - " << _spCount << endl;
}

pair<coord,coord> game::getPlayerStep()
{
    coord point_pos, new_pos;
    string s;
    cout << "Player" << (currentPlayer ? '2' : '1') << " step" << endl;
    cout << "Insert point position" << endl;
    cout << "(x): ";
    cin >> s;
    point_pos.first = stoi(s);
    cout << "(y): ";
    cin >> s;
    point_pos.second = stoi(s);
    cout << endl << "Insert new position" << endl;
    cout << "(x): ";
    cin >> s;
    new_pos.first = stoi(s);
    cout << "(y): ";
    cin >> s;
    new_pos.second = stoi(s);
    return make_pair(point_pos, new_pos);
}

bool game::step(pair<coord,coord> player_step)
{
    if (!checkPointPlayer(player_step.first))
    {
        cout << "Error step. This is not your point." << endl;
        return false;
    }
    if (isAccessiableStep(player_step) || isAccessiableHop(player_step) || isCanFindPath(player_step))
    {
        auto p = points.at(player_step.first);
        p->step(player_step.second);
        points.erase(points.find(player_step.first));
        points.insert(make_pair(player_step.second,p));
        if (checkGameState())
        {
            finishGame();
        }
        else
        {
            if (winner == 0)
                currentPlayer = !currentPlayer;
            if (winner != 0)
                penalty++;
        }
        return true;
    }
    else
    {
        cout << "Error step." << endl;
        return false;
    }
}

void game::finishGame()
{
    drawGameMap();
    cout << "Player" << (winner == 1 ? '1' : '2') << " win!" << " Penalty: " << penalty << endl;
    finished = true;
}



void game::mainLoop()
{
    while (!finished)
    {
        drawGameMap();
        try
        {
            auto s = getPlayerStep();
            step(s);
        }
        catch(...)
        {
            continue;
        }
    }
}

void game::startGame()
{
    mainLoop();
}

