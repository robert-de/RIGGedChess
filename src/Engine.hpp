#include "boardAPI.h"
#include "Table.h"
#include <list>
#include "libs.hpp"
#include <string>

#ifndef __ENGINE__
#define __ENGINE__

#define W 0
#define B 1

class Engine
{
public:
    static Engine *singletonEngine;
    static Engine *getInstance();

    boardAPI *API;
    // Engine playing as false == BLACK / true == WHITE
    bool color;

    Engine(/* args */);
    ~Engine();

    std::ofstream loggfile;
    void logString(std::string str);

    chessMove minimax();
    chessMove generateMove(Table *table);
    bool playingAs(bool color);
    std::vector<Cell*> attackPath(Cell* kingCell);
    void SetMoved(chessMove move);
};

#endif
