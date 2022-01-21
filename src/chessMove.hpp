#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include "libs.hpp"


class chessMove
{
public:

    // Coordinates
    char sourceFile;
    char sourceRank;
    char destinationFile;
    char destinationRank;

    // Special move flags
    char promote;
    bool castle;
    bool enPassant;

    chessMove();
    ~chessMove();
};

#endif
// // NUMERIC VALUES
// newMove.num_sf = this->commBuffer[cmdLen - 5] - 'a';
// newMove.num_sr = this->commBuffer[cmdLen - 4] - '1';
// newMove.num_df = this->commBuffer[cmdLen - 3] - 'a';
// newMove.num_dr = this->commBuffer[cmdLen - 2] - '1';
