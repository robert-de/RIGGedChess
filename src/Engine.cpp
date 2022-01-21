#include "Engine.hpp"
#include "Piece.h"
#include "Table.h"
#include <time.h>
#include <vector>
Engine *Engine::singletonEngine = NULL;

Engine::Engine()
{
    srand(time(NULL));
    std::string fname;
    fname = "./debug/API_debug_" + std::to_string(rand());
    API = new boardAPI(fname);
    color = BLACK;
}

Engine::~Engine()
{
}

chessMove Engine::minimax()
{
    return chessMove();
}

bool Engine::playingAs(bool color)
{
    this->color = color;

    return this->color;
}

Engine *Engine::getInstance()
{
    if (!Engine ::singletonEngine)
        Engine ::singletonEngine = new Engine();
    return Engine ::singletonEngine;
}

void Engine::logString(std::string str)
{
    loggfile.open("debugAI", std::ofstream::app);
    loggfile << str << std::endl;
    loggfile.close();
}

// Check direction of attack
std::vector<Cell *> Engine::attackPath(Cell *kingCell)
{

    std::vector<Cell *> path;
    Table *table = Table::getInstance();
    int kingR = kingCell->containedPiece->rank;
    int kingF = kingCell->containedPiece->file;
    int contR = kingCell->contesters[0]->rank;
    int contF = kingCell->contesters[0]->file;
    int i, j;

    // Sex Orizontal
    if (kingR == contR)
    {
        // Sex Orizontal Dreapta
        if (contF > kingF)
        {
            for (i = kingF + 1; i < contF; i++)
            {

                path.push_back(&table->pieceMatrix[kingR][i]);
            }
            // Sex Orizontal Stanga
        }
        else
        {
            for (i = kingF - 1; i > contF; i--)
            {
                path.push_back(&table->pieceMatrix[kingR][i]);
            }
        }
        // Sex Vertical
    }
    else if (kingF == contF)
    {
        // Sex Vertical Sus
        if (kingR > contR)
        {
            for (i = kingR - 1; i > contR; i--)
            {
                path.push_back(&table->pieceMatrix[i][kingF]);
            }
            // Sex Vertical Jos
        }
        else
        {
            for (i = kingR + 1; i < contR; i++)
            {
                path.push_back(&table->pieceMatrix[i][kingF]);
            }
        }
        // Sex Diagonal
    }
    else
    {
        j = kingF;
        // Sex Diagonal Sus
        if (kingR < contR)
        {
            for (i = kingR + 1; i < contR; i++)
            {
                if (contF > kingF)
                {
                    j++;
                }
                else
                {
                    j--;
                }

                path.push_back(&table->pieceMatrix[i][j]);
            }
            // Sex Diagonal Jos
        }
        else
        {
            for (i = kingR - 1; i > contR; i--)
            {
                if (contF > kingF)
                {
                    j++;
                }
                else
                {
                    j--;
                }

                path.push_back(&table->pieceMatrix[i][j]);
            }
        }
    }
    // stiti cum e ;)
    return path;
}

chessMove Engine::generateMove(Table *table)
{
    srand(time(NULL));
    int c;

    if (color == WHITE)
        c = W;
    else
        c = B;

    // CHECK MOVE
    // Check if the King of color c is in check
    if (table->checks[c].first)
    {
        std::vector<chessMove> outOfCheckMoves;
        King *king = (King *)table->refs[c][KING][0];

        Cell *kingCell = &table->pieceMatrix[king->rank][king->file];

        // Check if the King is contested by at most 1 enemy piece
        // to determine if king is forced to move, or if it is
        // possible to block the check or take the checking piece instead
        if (kingCell->contesters.size() == 1)
        {
            int cFile;
            int cRank;

            cFile = kingCell->contesters[0]->file;
            cRank = kingCell->contesters[0]->rank;

            Cell *contesterCell = &table->pieceMatrix[cRank][cFile];

            // Iterate through all cells in the path of the attacking piece
            // to determine blocking moves by looking at the contesters of the
            // king
            std::vector<Cell *> cellsToBlock;
            if (toupper(kingCell->contesters[0]->type) != 'N')
            {
                // Determine direction of attack
                cellsToBlock = attackPath(kingCell);
                for (auto cell : cellsToBlock)
                {
                    for (auto piece : cell->contesters)
                    {
                        chessMove move;
                        // If piece is pinned, it cannot move
                        if (piece->isPinned)
                            continue;

                        // Determine whether piece can block the check
                        if (piece->color == king->color && toupper(piece->type) != 'K' && toupper(piece->type) != 'P')
                        {
                            move = piece->returnMove(piece->file, piece->rank, cell->file, cell->rank, 0);
                            outOfCheckMoves.push_back(move);
                        }
                    }

                    // Special logic for pawns to determine whether the pawn
                    // Can block the check
                    for (auto pawn : table->refs[c][PAWNS])
                    {
                        if (pawn->isPinned)
                            continue;

                        for (chessMove blockMove : pawn->moves)
                        {
                            if (blockMove.destinationFile - 'a' == cell->file && blockMove.destinationRank - '1' == cell->rank)
                            {
                                outOfCheckMoves.push_back(blockMove);
                            }
                        }
                    }
                }
            }

            // Determine whether or not contesting piece can be captured
            // and add possible capturing moves to move array
            for (auto piece : contesterCell->contesters)
            {
                chessMove move;
                // King capturing moves are added in the king move lo
                if (toupper(piece->type) == 'K')
                    continue;

                if (piece->isPinned)
                    continue;

                if (piece->color == king->color)
                {
                    move = piece->returnMove(piece->file, piece->rank, contesterCell->file, contesterCell->rank, 0);
                    outOfCheckMoves.push_back(move);
                }
            }
        }

        for (auto move : king->moves)
        {
            // Cannot castle out of check
            if (move.castle)
                continue;

            Cell target;
            int rank = move.destinationRank - '1';
            int file = move.destinationFile - 'a';
            target = table->pieceMatrix[rank][file];

            bool hasAttacker = false;
            for (auto piece : target.contesters)
            {
                if (piece->color != king->color)
                    hasAttacker = true;
            }

            for (auto piece2 : target.defenders)
            {
                if (king->color != piece2->color)
                    hasAttacker = true;
            }

            if (!hasAttacker)
                outOfCheckMoves.push_back(move);
        }

        // If there are available moves to get out of check,
        // pick a random one
        if (!outOfCheckMoves.empty())
        {
            chessMove sentMove = outOfCheckMoves[rand() % outOfCheckMoves.size()];

            table->checks[c].first = false;
            // If the king must move set its moved flag to true
            SetMoved(sentMove);

            return sentMove;
        }
        else
        {
            write(STDOUT_FILENO, "resign\n", 7);
            table->clearTable();
            table->tableInit();

            // Dummy move used to determine that no moves are possible
            // when returning from generateMove()
            Pawn *dummy = new Pawn(0, -1, -1, 0);
            return dummy->returnMove(-1, -1, -1, -1, 0);
        }
    }
    else
    {
        // NORMAL MOVE
        // Generate a vector of all available moves and pick one at random
        std::vector<chessMove> posMoves;

        for (int type = 0; type < 6; type++)
        {
            auto selectedType = table->refs[c][type];
            for (auto piece : selectedType)
            {

                if (piece->isPinned)
                    continue;

                piece->possibleMoves();

                for (auto move : piece->moves)
                {
                    // Determining whether or the move the king 
                    // would want to make is valid
                    if (toupper(piece->type) == 'K')
                    {
                        bool hasAttacker = false;
                        Cell target;
                        int rank = move.destinationRank - '1';
                        int file = move.destinationFile - 'a';
                        target = table->pieceMatrix[rank][file];

                        for (auto piece2 : target.contesters)
                        {
                            if (piece->color != piece2->color)
                                hasAttacker = true;
                        }

                        for (auto piece2 : target.defenders)
                        {
                            if (piece->color != piece2->color)
                                hasAttacker = true;
                        }

                        // If the target is contested, try another move
                        if (hasAttacker)
                        {
                            continue;
                        }
                    }

                    // If castling is available, castle
                    if (move.castle)
                    {
                        return move;
                    }

                    posMoves.push_back(move);
                }
            }
        }

        int index;

        if (posMoves.size() > 0)
            index = rand() % posMoves.size();
        else
        {
            API->logfile << "NO MORE AVAILABLE MOVES, RESIGNING\n";
            Pawn *dummy = new Pawn(0, -1, -1, 0);
            return dummy->returnMove(-1, -1, -1, -1, 0);
        }

        auto selectedMove = posMoves[index];

        // If the king must move set it's moved flag to troo
        SetMoved(selectedMove);

        return selectedMove;
    }
}

void Engine::SetMoved(chessMove move)
{
    int rank;
    int file;

    rank = move.sourceRank - '1';
    file = move.sourceFile - 'a';

    Table *table = Table::getInstance();
    Piece *p;

    p = table->pieceMatrix[rank][file].containedPiece;

    char type;
    type = toupper(p->type);
    if (type == 'K')
    {
        ((King *)p)->setMoved(true);
    }
    else if (type == 'R')
    {
        ((Rook *)p)->setMoved(true);
    }
}