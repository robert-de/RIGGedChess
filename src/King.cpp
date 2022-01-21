#include "King.h"
#include "Piece.h"
#include "Table.h"
#include "boardAPI.h"

//DONE: constructor; getMoved; setMoved

//TODO: checkChess(); possibleMoves()

King::King(int color, int file, int rank, int type, bool hasMoved) : Piece(color, file, rank, type)
{
    moved = moved;
}

King::King() : Piece()
{
}

King::~King()
{
}

void King::setMoved(bool m)
{
    moved = m;
}

bool King::getMoved()
{
    return moved;
}

void King::possibleMoves()
{
    int r = getRank();
    int f = getFile();
    int c = getColor();
    moves.clear();

    // S
    if (checkFree(f, r + 1) || checkCapture(f, r + 1, c))
    {
        addMove(returnMove(f, r, f, r + 1, 0));
    }
    else if (checkDefend(f, r + 1, c))
    {
        defending.push_back(returnMove(f, r, f, r + 1, 0));
    }

    //SE
    if (checkFree(f + 1, r + 1) || checkCapture(f + 1, r + 1, c))
        addMove(returnMove(f, r, f + 1, r + 1, 0));
    else if (checkDefend(f + 1, r + 1, c))
        defending.push_back(returnMove(f, r, f + 1, r + 1, 0));

    //E
    if (checkFree(f + 1, r) || checkCapture(f + 1, r, c))
        addMove(returnMove(f, r, f + 1, r, 0));
    else if (checkDefend(f + 1, r, c))
        defending.push_back(returnMove(f, r, f + 1, r, 0));

    //NE
    if (checkFree(f + 1, r - 1) || checkCapture(f + 1, r - 1, c))
        addMove(returnMove(f, r, f + 1, r - 1, 0));
    else if (checkDefend(f + 1, r - 1, c))
        defending.push_back(returnMove(f, r, f + 1, r - 1, 0));
    //N
    if (checkFree(f, r - 1) || checkCapture(f, r - 1, c))
        addMove(returnMove(f, r, f, r - 1, 0));
    else if (checkDefend(f, r - 1, c))
        defending.push_back(returnMove(f, r, f, r - 1, 0));

    //NW
    if (checkFree(f - 1, r - 1) || checkCapture(f - 1, r - 1, c))
        addMove(returnMove(f, r, f - 1, r - 1, 0));
    else if (checkDefend(f - 1, r - 1, c))
        defending.push_back(returnMove(f, r, f - 1, r - 1, 0));

    //W
    if (checkFree(f - 1, r) || checkCapture(f - 1, r, c))
        addMove(returnMove(f, r, f - 1, r, 0));
    else if (checkDefend(f - 1, r, c))
        defending.push_back(returnMove(f, r, f - 1, r, 0));

    //SW
    if (checkFree(f - 1, r + 1) || checkCapture(f - 1, r + 1, c))
        addMove(returnMove(f, r, f - 1, r + 1, 0));
    else if (checkDefend(f - 1, r + 1, c))
        defending.push_back(returnMove(f, r, f - 1, r + 1, 0));

    Table *gameTable = Table::getInstance();

    auto matrix = gameTable->getMatrix();
    // Castle for white
    if (getMoved() == false && getColor() == 1)
    {
        //queen's side
        Rook *r = (Rook *)(matrix[0][0].containedPiece);

        if (r && r->getColor() == getColor() && r->getMoved() == false && r->getType() == 'R')
        {
            if (checkFree(1, 0) && checkFree(2, 0) && checkFree(3, 0) && !checkContested(2, 0) && !checkContested(3, 0))
            {

                chessMove m = returnMove(4, 0, 2, 0, 0);
                m.castle = true;
                addMove(m);
            }
        }
        //king's side
        r = (Rook *)(matrix[0][7].containedPiece);
        if (r && r->getColor() == getColor() && r->getMoved() == false && r->getType() == 'R')
        {
            if (checkFree(5, 0) && checkFree(6, 0) && !checkContested(5, 0) && !checkContested(6, 0))
            {

                chessMove m = returnMove(4, 0, 6, 0, 0);
                m.castle = true;
                addMove(m);
            }
        }
    }

    if (getMoved() == false && getColor() == -1)
    {
        //queen's side
        Rook *r = (Rook *)(matrix[7][0].containedPiece);
        if (r && r->getColor() == getColor() && r->getMoved() == false && r->getType() == 'r')
        {
            if (checkFree(1, 7) && checkFree(2, 7) && checkFree(3, 7) && !checkContested(2, 7) && !checkContested(3, 7))
            {
                chessMove m = returnMove(4, 7, 2, 7, 0);
                m.castle = true;
                addMove(m);
            }
        }
        //king's side
        r = (Rook *)(matrix[7][7].containedPiece);
        if (r && r->getColor() == getColor() && r->getMoved() == false && r->getType() == 'r')
        {
            if (checkFree(5, 7) && checkFree(6, 7) && !checkContested(5, 7) && !checkContested(6, 7))
            {
                chessMove m = returnMove(4, 7, 6, 7, 0);
                m.castle = true;
                addMove(m);
            }
        }
    }

    // Loop through all king moves and remove those where destination cell
    // has contesters of opposite color
    for (int i = 0; i < (int)moves.size(); i++)
    {
        int file = moves[i].destinationFile - 'a';
        int rank = moves[i].destinationRank - '1';
        if (checkContested(file, rank))
        {
            moves.erase(moves.begin() + i);
        }
    }
}
