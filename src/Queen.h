#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include "boardAPI.h"
#include <vector>

class Queen : public Piece
{
private:
    /* data */
public:
    Queen(int colour, int file, int rank, char type);
    Queen(/* args */);
    ~Queen();

    bool checkUnderAttack();
    void possibleMoves();
};

#endif