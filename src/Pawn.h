#ifndef PAWN_H
#define PAWN_H

#include <vector>
#include "boardAPI.h"
#include "Piece.h"
//#include "Table.h"

class Pawn : public Piece {
    private:
        /* data */
    public:
        Pawn();
        Pawn(int colour, int file, int rank, char type);
        ~Pawn();

        bool checkUnderAttack();
        void possibleMoves(); 
        
        void addAllPromotes(int sf, int df, int myColor);
        void contestPawn();

        void promote();
};

#endif
