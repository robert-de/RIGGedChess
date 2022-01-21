#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include "boardAPI.h"
#include <vector>

class Rook : public Piece
{
private:
    /* data */
    /* if the king and rook have not moved we can castle */
    bool moved; /* 0 not moved; 1 moved */
public:
    Rook(int colour, int file, int rank, char type, bool moved);
    Rook();

    ~Rook();

    bool getMoved();
    void setMoved(bool m);

    bool checkUnderAttack();

    void possibleMoves();
};

#endif