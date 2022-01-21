#include "Knight.h"
#include "libs.hpp"
#include <vector>

// DONE: constructori

//TODO: checkUnderAttack; possibleMoves

Knight::Knight()
{
}

Knight::~Knight()
{
}

Knight::Knight(int colour, int file, int rank, char type) : Piece(colour, file, rank, type)
{
}

void Knight::processMove(int file, int rank)
{

    int color = getColor();

    if (checkCapture(file, rank, color) || checkFree(file, rank))
    {
        moves.push_back(returnMove(getFile(), getRank(), file, rank, 0));
    }
    else if (checkDefend(file, rank, color))
        defending.push_back(returnMove(getFile(), getRank(), file, rank, 0));
}

void Knight::possibleMoves()
{

    moves.clear();

    // No moves are generated if the piece is already captured
    if (getCaptured() == true)
        return;

    // Current file and rank
    int cfile = getFile();
    int crank = getRank();

    // 2 up, 1 right
    processMove(cfile - 2, crank + 1);

    // 2 up, 1 left
    processMove(cfile - 2, crank - 1);

    // 2 right, 1 down
    processMove(cfile + 2, crank + 1);

    // 2 down, 1 left
    processMove(cfile + 2, crank - 1);

    // 2 right, 1 up
    processMove(cfile - 1, crank + 2);

    // 2 down, 1 right
    processMove(cfile + 1, crank + 2);

    // 2 left, 1 up
    processMove(cfile - 1, crank - 2);

    // 2 left, 1 down
    processMove(cfile + 1, crank - 2);
}
