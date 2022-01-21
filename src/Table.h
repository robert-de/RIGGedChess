#ifndef TABLE_H
#define TABLE_H

#include "Piece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Cell.hpp"
#include <list>


#define W 0
#define B 1

#define WHITE true
#define BLACK false

#define PAWNS 0
#define ROOKS 1
#define KNIGHTS 2
#define BISHOPS 3
#define KING 4
#define QUEEN 5

class Table
{

    
public:
    // Singleton implementation
    static Table *singleton;
    static Table *getInstance();

    std::vector<std::vector<Cell>> pieceMatrix;

    std::vector<std::vector<std::vector<Piece*>>> refs;
    std::pair<bool, int> checks[2];


    // Current player false == BLACK / true == WHITE
    bool turn;
    int turnCounter = 0;
    std::list<chessMove> history;
    // Force mode enabled?
    bool editMode;
    // For the quit command
    bool quit;

    std::ofstream logfile;
    std::string filename;
    Table(std::string fileName);
    ~Table();

    std::vector<std::vector<Cell>> getMatrix();
    void tableInit();
    void debugDraw();
    void clearTable();

    void updateTable(chessMove);
    void setColor(bool color);
    chessMove getLastMove();

    void addContestersToCell(std::vector<std::vector<Piece*>> matrix);

    void setPinnedPieces(King *xKing);
};

#endif