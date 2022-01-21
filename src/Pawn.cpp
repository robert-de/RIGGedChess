#include "Pawn.h"
#include "Engine.hpp"
#include "Table.h"
#include "libs.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

Pawn::Pawn(int colour, int file, int rank, char type) : Piece(colour, file, rank, type)
{
}

Pawn::~Pawn()
{
}

Pawn::Pawn() : Piece()
{
}

void Pawn::contestPawn()
{

    Table *table = Table::getInstance();
    if (getColor() == 1)
    {
        // se uita an sus si la stanga
        if (!outOfBounds(file - 1, rank + 1))
            if (table->pieceMatrix[rank + 1][file - 1].containedPiece == NULL)
                table->pieceMatrix[rank + 1][file - 1].contesters.push_back(this);
            else if (color != table->pieceMatrix[rank + 1][file - 1].containedPiece->color)
                table->pieceMatrix[rank + 1][file - 1].contesters.push_back(this);
            else
                table->pieceMatrix[rank + 1][file - 1].defenders.push_back(this);

        // se uita an sus si la dreapta
        if (!outOfBounds(file + 1, rank + 1))
            if (table->pieceMatrix[rank + 1][file + 1].containedPiece == NULL)
                table->pieceMatrix[rank + 1][file + 1].contesters.push_back(this);
            else if (color != table->pieceMatrix[rank + 1][file + 1].containedPiece->color)
                table->pieceMatrix[rank + 1][file + 1].contesters.push_back(this);
            else
                table->pieceMatrix[rank + 1][file + 1].defenders.push_back(this);
    }
    else
    {
        // se uita an jos si la stanga
        if (!outOfBounds(file - 1, rank - 1))
            if (table->pieceMatrix[rank - 1][file - 1].containedPiece == NULL)
                table->pieceMatrix[rank - 1][file - 1].contesters.push_back(this);
            else if (color != table->pieceMatrix[rank - 1][file - 1].containedPiece->color)
                table->pieceMatrix[rank - 1][file - 1].contesters.push_back(this);
            else
                table->pieceMatrix[rank - 1][file - 1].defenders.push_back(this);

        // se uita an jos si la dreapta
        if (!outOfBounds(file + 1, rank - 1))
            if (table->pieceMatrix[rank - 1][file + 1].containedPiece == NULL)
                table->pieceMatrix[rank - 1][file + 1].contesters.push_back(this);
            else if (color != table->pieceMatrix[rank - 1][file + 1].containedPiece->color)
                table->pieceMatrix[rank - 1][file + 1].contesters.push_back(this);
            else
                table->pieceMatrix[rank - 1][file + 1].defenders.push_back(this);
    }
}
void Pawn::possibleMoves()
{

    std::string msg = "";

    int r = getRank();
    int f = getFile();
    moves.clear();
    defending.clear();

    if (getCaptured() == true)
        return;

    contestPawn();

    if (getColor() == 1)
    {
        /* move 2 space for first move of pawn */
        if (r == 1 && checkFree(f, 2) && checkFree(f, 3))
        {
            addMove(returnMove(f, r, f, 3, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f, 3, 0)));
        }
        /* move 1 space, but not promote*/
        if (checkFree(f, r + 1) && r < 6)
        {
            addMove(returnMove(f, r, f, r + 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f, r + 1, 0)));
        }
        /* capture, but not promote */

        // Capture left
        if (checkCapture(f - 1, r + 1, getColor()) && r < 6)
        {

            addMove(returnMove(f, r, f - 1, r + 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f - 1, r + 1, 0)));
        }
        else if (checkDefend(f - 1, r + 1, getColor()) && r < 6)
        {

            defending.push_back(returnMove(f, r, f - 1, r + 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f - 1, r + 1, 0)));
        }

        // Capture right
        if (checkCapture(f + 1, r + 1, getColor()) && r < 6)
        {

            addMove(returnMove(f, r, f + 1, r + 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f + 1, r + 1, 0)));
        }
        else if (checkDefend(f + 1, r + 1, getColor()) && r < 6)
        {

            defending.push_back(returnMove(f, r, f + 1, r + 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f + 1, r + 1, 0)));
        }

        /*promote by moving forward one space */
        if (checkFree(f, r + 1) && r == 6)
        {
            addAllPromotes(f, f, getColor());
        }

        /* capture and promote */
        if (checkCapture(f - 1, r + 1, getColor()) && r == 6)
            addAllPromotes(f, f - 1, getColor());
        if (checkCapture(f + 1, r + 1, getColor()) && r == 6)
            addAllPromotes(f, f + 1, getColor());
    }
    else
    {
        /* move 2 space for first move of pawn */
        if (r == 6 && checkFree(f, 4) && checkFree(f, 5))
        {
            addMove(returnMove(f, r, f, 4, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f, 4, 0)));
        }

        /* move 1 space, but not promote*/
        if (checkFree(f, r - 1) && r > 1)
        {
            addMove(returnMove(f, r, f, r - 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f, r - 1, 0)));
        }

        /* capture, but not promote */
        if (checkCapture(f - 1, r - 1, getColor()) && r > 1)
        {
            addMove(returnMove(f, r, f - 1, r - 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f - 1, r - 1, 0)));
        }

        if (checkCapture(f + 1, r - 1, getColor()) && r > 1)
        {
            addMove(returnMove(f, r, f + 1, r - 1, 0));
            // rigged->logString(genMoveStr(returnMove(f, r, f + 1, r - 1, 0)));
        }

        /*promote by moving forward one space */
        if (checkFree(f, r - 1) && r == 1)
        {
            addAllPromotes(f, f, getColor());
        }

        /* capture and promote */
        if (checkCapture(f - 1, r - 1, getColor()) && r == 1)
            addAllPromotes(f, f - 1, getColor());
        if (checkCapture(f + 1, r - 1, getColor()) && r == 1)
            addAllPromotes(f, f + 1, getColor());
    }

    /*en passant*/
    if (getColor() == 1 && getRank() == 4)
    {
        Table *gameTable = Table::getInstance();
        auto matrix = gameTable->getMatrix();
        chessMove m = gameTable->history.back();
        int df = m.destinationFile - 97;
        int dr = m.destinationRank - 49;

        if (dr == 4 && (df == getFile() + 1 || df == getFile() - 1) && matrix[dr][df].containedPiece->getType() == 'p')
        {
            int sr = m.sourceRank - 49;

            if (dr - sr == -2)
            {
                chessMove q = returnMove(getFile(), 4, df, 5, 0);
                q.enPassant = true;
                moves.push_back(q);
            }
        }
    }
    else if (getColor() == -1 && getRank() == 3)
    {
        Table *gameTable = Table::getInstance();
        auto matrix = gameTable->getMatrix();
        chessMove m = gameTable->history.back();
        int df = m.destinationFile - 97;
        int dr = m.destinationRank - 49;

        if (dr == 3 && (df == getFile() + 1 || df == getFile() - 1) && matrix[dr][df].containedPiece->getType() == 'P')
        {
            int sr = m.sourceRank - 49;

            if (dr - sr == 2)
            {

                chessMove q = returnMove(getFile(), 3, df, 2, 0);
                q.enPassant = true;
                moves.push_back(q);
            }
        }
    }
}

void Pawn::addAllPromotes(int sf, int df, int myColor)
{
    if (myColor == 1)
    {
        //white
        addMove(returnMove(sf, 6, df, 7, 114)); //rook
        addMove(returnMove(sf, 6, df, 7, 110)); //knight
        addMove(returnMove(sf, 6, df, 7, 98));  //bishop
        addMove(returnMove(sf, 6, df, 7, 113)); //queen
    }
    else
    {
        //black
        addMove(returnMove(sf, 1, df, 0, 114)); //rook
        addMove(returnMove(sf, 1, df, 0, 110)); //knight
        addMove(returnMove(sf, 1, df, 0, 98));  //bishop
        addMove(returnMove(sf, 1, df, 0, 113)); //queen
    }
}