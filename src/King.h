#ifndef KING_H
#define KING_H


#include <vector>
#include "boardAPI.h"
#include "Piece.h"

class King : public Piece
{
private:
    /* data */
    /* if the king and rook have not moved we can castle */
    bool moved = false; /* 0 not moved; 1 moved */
public:
    King(int color, int file, int rank, int type, bool move);
    King();
    ~King();

    bool getMoved();
    void setMoved(bool m);

    bool checkChess();
    void possibleMoves();
};

#endif