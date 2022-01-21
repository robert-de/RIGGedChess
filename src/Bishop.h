#ifndef BISHOP_H
#define BISHOP_H

#include <vector>
#include "boardAPI.h"
#include "Piece.h"

class Bishop : public Piece
{
private:
    /* data */
public:
    Bishop();
    Bishop(int color, int file, int rank, char type);
    ~Bishop();
    
    bool checkUnderAttack();
    void possibleMoves();
};

#endif