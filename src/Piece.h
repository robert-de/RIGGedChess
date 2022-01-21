
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "boardAPI.h"
//#include "Table.h"

class Piece {
public:
    /* data */
    int file; /*from 0 to 7 */
    int rank; /* from 0 to 7 */
    int color; /* -1 negru 1 alb */

    bool isCaptured;
    bool isPinned;
    std::vector<chessMove> moves;
    std::vector<chessMove> defending;
    char type;
    // Pawn p
    // Rook r
    // Knight n
    // Bishop b
    // Queen q
    // King k

    Piece();
    Piece(int colour, int rank, int file, char type) ;
    ~Piece();

    int getColor();
    int getFile();
    int getRank();
    bool getCaptured();
    bool getPinned();
    char getType();
    std::vector<chessMove> getMoves();
    
    void addMove(chessMove m);
    void setColor(int c);
    void setFile(int f);
    void setRank(int r);
    void setCaptured(bool isCaptured);
    void setPinned(bool pin);
    void setType(char t);

    virtual void possibleMoves() = 0;

    chessMove returnMove(int sf, int sr, int df, int dr, char promote);

    void updatePosition(chessMove M);
    
    bool checkDefend(int f, int r, int myColour);
    
    bool checkCapture(int f, int r, int myColour);

    bool checkFree(int f, int r);
    chessMove returnMove(int sf, int sr, int df, int dr);

    std::string possMovesStr();

    std::string genMoveStr(chessMove move);

    bool outOfBounds(int f, int r);

    bool checkContested(int file, int rank);
};

#endif
