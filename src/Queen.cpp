#include "Queen.h"
#include "libs.hpp"
#include <vector>
#include "Table.h"
#include "Engine.hpp"

Queen::Queen(int colour, int file, int rank, char type) : Piece(colour, file, rank, type){

}

Queen::Queen(){

}

Queen::~Queen(){

}

void Queen::possibleMoves() {

    auto pMatrix = Table::getInstance()->pieceMatrix;
    chessMove newMove;
    this->moves.clear();

    // No moves are generated if the piece is already captured
    if (this->getCaptured() == true)
        return;

    // Current file and rank
    int sf = this->getFile();
    int sr = this->getRank();

    // Finding N-S and W-E moves
    // Check north direction for available moves (i represents collumns)
    for (int i = this->getRank() - 1; i >= 0; i--) {
        
        if(this->checkFree(this->getFile(), i)) {
            this->moves.push_back(returnMove(sf, sr, sf, i, 0));
        } else if (this->checkCapture(getFile(), i, this->color)) {
            this->moves.push_back(returnMove(sf, sr, sf, i, 0));
            
            if (toupper(pMatrix[i][sf].containedPiece->type) == 'K') {
                continue;
            }
            
            break;
        } else if (!this->checkCapture(getFile(), i, this->color)) {
            defending.push_back(returnMove(sf, sr, sf, i, 0));
            break;
        }
    }

    // Check south direction for available moves
    for (int i = this->getRank() + 1; i < 8; i++) {
        if(this->checkFree(this->getFile(), i)) {
            this->moves.push_back(returnMove(sf, sr, sf, i, 0));
        } else if (this->checkCapture(getFile(), i, this->color)) {
            this->moves.push_back(returnMove(sf, sr, sf, i, 0));

            if (toupper(pMatrix[i][sf].containedPiece->type) == 'K') {
                continue;
            }
            
            break;
        } else if (!this->checkCapture(getFile(), i, this->color)) {
            defending.push_back(returnMove(sf, sr, sf, i, 0));
            break;
        }
    }

    // Check left direction for available moves
    for (int i = this->getFile() - 1; i >= 0; i--)
    {
        if(this->checkFree(i, this->getRank())) {
            this->moves.push_back(returnMove(sf, sr, i, sr, 0));
        } else if (this->checkCapture(i, this->getRank(), this->color)) {
            this->moves.push_back(returnMove(sf, sr, i, sr, 0));
            
            if (toupper(pMatrix[sr][i].containedPiece->type) == 'K') {
                continue;
            }

            break;
        } else if (!this->checkCapture(i, this->getRank(), this->color)) {
            defending.push_back(returnMove(sf, sr, i, sr, 0));
            break;
        } 
    }

    // Check right direction for available moves
    for (int i = this->getFile() + 1; i < 8; i++)
    {
        if(this->checkFree(i, this->getRank())) {
            this->moves.push_back(returnMove(sf, sr, i, sr, 0));
        } else if (this->checkCapture(i, this->getRank(), this->color)) {
            this->moves.push_back(returnMove(sf, sr, i, sr, 0));

            if (toupper(pMatrix[sr][i].containedPiece->type) == 'K') {
                continue;
            }

            break;
        } else if (!this->checkCapture(i, this->getRank(), this->color)) {
            defending.push_back(returnMove(sf, sr, i, sr, 0));
            break;
        } 

    }

    // Finding diagonal moves
    // SOUTH EAST
    /* +x +y */
    int R = sr + 1;
    int F = sf + 1;

    while (true) {
        if(R == 8 || F == 8)
            break;
        if(this->checkFree(F, R)) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            R++;
            F++;
        }
        else if(checkCapture(F, R, this->getColor())) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K') {
                R++;
                F++;
                continue;
            }
            break;
        } else if (!checkCapture(F, R, this->getColor())) {
            defending.push_back(returnMove(sf, sr, F, R, 0));
            break;
        }
    }

    // SOUTH WEST
    /* +x -y */
    R = sr + 1;
    F = sf - 1;

    while (true){
        if(R == 8 || F == -1)
            break;
        if(this->checkFree(F, R)) {
            this->moves.push_back(returnMove(sf, sr, F, R, 0));
            R++;
            F--;
        }
        else if(checkCapture(F, R, this->getColor())) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K') {
                R++;
                F--;
                continue;
            }
            break;
        } else if (!checkCapture(F, R, this->getColor())) {
            defending.push_back(returnMove(sf, sr, F, R, 0));
            break;
        }
    }

    // NORTH WEST
    /* -x -y */
    R = sr - 1;
    F = sf - 1;

    while (true){
        if(R == -1 || F == -1)
            break;
        if(this->checkFree(F, R)) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            R--;
            F--;
        }
        else if(checkCapture(F, R, this->getColor())) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K') {
                R--;
                F--;
                continue;
            }
            break;
        } else if (!checkCapture(F, R, this->getColor())) {
            defending.push_back(returnMove(sf, sr, F, R, 0));
            break;
        }
    }

    // NORTH EAST
    /* -x +y */
    R = sr - 1;
    F = sf + 1;

    while (true){
        if(R == -1 || F == 8)
            break;
        if(this->checkFree(F, R)) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            R--;
            F++;
        }
        else if(checkCapture(F, R, this->getColor())) {
            this->addMove(returnMove(sf, sr, F, R, 0));
            if (toupper(pMatrix[R][F].containedPiece->type) == 'K') {
                R--;
                F++;
                continue;
            }
            break;
        } else if (!checkCapture(F, R, this->getColor())) {
            defending.push_back(returnMove(sf, sr, F, R, 0));
            break;
        }
    }
}
