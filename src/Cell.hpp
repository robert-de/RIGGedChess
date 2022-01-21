#ifndef CELL_HPP
#define CELL_HPP

#include "Piece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"

class Cell
{
    
public:

    int rank;
    int file;
    Piece *containedPiece;
    std::vector <Piece *> contesters;
    std::vector <Piece *> defenders;
    Cell();
    ~Cell();

    void defenderGenerator();
};

#endif