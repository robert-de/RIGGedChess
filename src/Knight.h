#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "boardAPI.h"
#include "Piece.h"

class Knight : public Piece
{
private:
    /* data */
public:
    Knight(int colour, int file, int rank, char type);
    Knight();
    ~Knight();

    bool checkUnderAttack();
    void possibleMoves();

    void processMove(int file, int rank);
};

#endif