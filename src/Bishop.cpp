#include "Bishop.h"
#include "Engine.hpp"
#include "Table.h"
#include "libs.hpp"
#include <vector>

//DONE: constructor

//TODO: checkUnderAttack();

Bishop::Bishop(int colour, int file, int rank, char type)
    : Piece(colour, file, rank, type)
{
}

Bishop::Bishop() : Piece()
{
}

Bishop::~Bishop()
{
}

void Bishop::possibleMoves()
{
    int r = getRank();
    int f = getFile();
    moves.clear();

    if (getCaptured() == true)
        return;
    /* considering a (0,0) position, we can go in 4 directions:
        +x +y
        +x -y
        -x -x
        -x +y
    */

    /* while we re still on the board, we can move to a free cell,
        capture an opponent piece (if the road is free until that spot)
        or not move from out position */

    // SOUTH EAST
    /* +x +y */
    int R = r + 1;
    int F = f + 1;
    auto pMatrix = Table::getInstance()->pieceMatrix;

    while (true)
    {
        if (R == 8 || F == 8)
            break;
        if (checkFree(F, R))
        {
            addMove(returnMove(f, r, F, R, 0));
            R++;
            F++;
        }
        else if (checkCapture(F, R, getColor()))
        {
            addMove(returnMove(f, r, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K')
            {
                R++;
                F++;
                continue;
            }
            break;
        }
        else if (!checkCapture(F, R, getColor()))
        {
            defending.push_back(returnMove(f, r, F, R, 0));
            break;
        }
    }

    // SOUTH WEST
    /* +x -y */
    R = r + 1;
    F = f - 1;

    while (true)
    {
        if (R == 8 || F == -1)
            break;
        if (checkFree(F, R))
        {
            moves.push_back(returnMove(f, r, F, R, 0));
            R++;
            F--;
        }
        else if (checkCapture(F, R, getColor()))
        {
            addMove(returnMove(f, r, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K')
            {
                R++;
                F--;
                continue;
            }
            break;
        }
        else if (!checkCapture(F, R, getColor()))
        {

            defending.push_back(returnMove(f, r, F, R, 0));
            break;
        }
    }

    // NORTH WEST
    /* -x -y */
    R = r - 1;
    F = f - 1;

    while (true)
    {
        if (R == -1 || F == -1)
            break;
        if (checkFree(F, R))
        {
            addMove(returnMove(f, r, F, R, 0));
            R--;
            F--;
        }
        else if (checkCapture(F, R, getColor()))
        {
            addMove(returnMove(f, r, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K')
            {
                R--;
                F--;
                continue;
            }
            break;
        }
        else if (!checkCapture(F, R, getColor()))
        {

            defending.push_back(returnMove(f, r, F, R, 0));
            break;
        }
    }

    // NORTH EAST
    /* -x +y */
    R = r - 1;
    F = f + 1;

    while (true)
    {
        if (R == -1 || F == 8)
            break;
        if (checkFree(F, R))
        {
            addMove(returnMove(f, r, F, R, 0));
            R--;
            F++;
        }
        else if (checkCapture(F, R, getColor()))
        {
            addMove(returnMove(f, r, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K')
            {
                R--;
                F++;
                continue;
            }
            break;
        }
        else if (!checkCapture(F, R, getColor()))
        {

            defending.push_back(returnMove(f, r, F, R, 0));
            break;
        }
    }
}
