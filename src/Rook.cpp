#include "Rook.h"
#include "Table.h"
#include "libs.hpp"
#include <vector>

Rook::Rook(int colour, int file, int rank, char type, bool hasMoved) : Piece(colour, file, rank, type)
{
    setMoved(hasMoved);
}

Rook::Rook()
{
}

Rook::~Rook()
{
}

bool Rook::getMoved()
{
    return moved;
}

void Rook::setMoved(bool m)
{
    moved = m;
}

void Rook::possibleMoves()
{
    auto pMatrix = Table::getInstance()->pieceMatrix;
    chessMove newMove;
    moves.clear();

    // No moves are generated if the piece is already captured
    if (getCaptured() == true)
        return;

    int sf = getFile();
    int sr = getRank();
    // std::cout << "Finding moves for piece at row " << sr << " col " << sf << std::endl;
    // Check north direction for available moves (i represents collumns)
    // Check north direction for available moves (i represents collumns)
    for (int i = getRank() - 1; i >= 0; i--)
    {

        if (checkFree(getFile(), i))
        {
            moves.push_back(returnMove(sf, sr, sf, i, 0));
        }
        else if (checkCapture(getFile(), i, color))
        {
            moves.push_back(returnMove(sf, sr, sf, i, 0));

            if (toupper(pMatrix[i][sf].containedPiece->type) == 'K')
            {
                continue;
            }

            break;
        }
        else if (!checkCapture(getFile(), i, color))
        {
            defending.push_back(returnMove(sf, sr, sf, i, 0));
            break;
        }
    }

    // Check south direction for available moves
    for (int i = getRank() + 1; i < 8; i++)
    {
        if (checkFree(getFile(), i))
        {
            moves.push_back(returnMove(sf, sr, sf, i, 0));
        }
        else if (checkCapture(getFile(), i, color))
        {
            moves.push_back(returnMove(sf, sr, sf, i, 0));

            if (toupper(pMatrix[i][sf].containedPiece->type) == 'K')
            {
                continue;
            }

            break;
        }
        else if (!checkCapture(getFile(), i, color))
        {
            defending.push_back(returnMove(sf, sr, sf, i, 0));
            break;
        }
    }

    // Check left direction for available moves
    for (int i = getFile() - 1; i >= 0; i--)
    {
        if (checkFree(i, getRank()))
        {
            moves.push_back(returnMove(sf, sr, i, sr, 0));
        }
        else if (checkCapture(i, getRank(), color))
        {
            moves.push_back(returnMove(sf, sr, i, sr, 0));

            if (toupper(pMatrix[sr][i].containedPiece->type) == 'K')
            {
                continue;
            }

            break;
        }
        else if (!checkCapture(i, getRank(), color))
        {
            defending.push_back(returnMove(sf, sr, i, sr, 0));
            break;
        }
    }

    // Check right direction for available moves
    for (int i = getFile() + 1; i < 8; i++)
    {
        if (checkFree(i, getRank()))
        {
            moves.push_back(returnMove(sf, sr, i, sr, 0));
        }
        else if (checkCapture(i, getRank(), color))
        {
            moves.push_back(returnMove(sf, sr, i, sr, 0));

            if (toupper(pMatrix[sr][i].containedPiece->type) == 'K')
            {
                continue;
            }

            break;
        }
        else if (!checkCapture(i, getRank(), color))
        {
            defending.push_back(returnMove(sf, sr, i, sr, 0));
            break;
        }
    }
}
