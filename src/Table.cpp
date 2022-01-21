#include "Table.h"
#include "Engine.hpp"
#include "chessMove.hpp"
Table *Table ::singleton = NULL;

Table::Table(std::string fileName) : logfile(fileName, std::ofstream::app)
{
    std::vector<std::vector<Cell>> temp(8, std::vector<Cell>(8));
    filename = fileName;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            temp[i][j].rank = i;
            temp[i][j].file = j;
        }

    pieceMatrix = temp;
    editMode = false;

    std::vector<std::vector<Piece *>> tempWRef(6, std::vector<Piece *>());
    std::vector<std::vector<Piece *>> tempBRef(6, std::vector<Piece *>());
    std::vector<std::vector<std::vector<Piece *>>> tempRefs(2, 
        std::vector<std::vector<Piece *>>(6, 
        std::vector<Piece *>()));

    refs = tempRefs;
    refs.push_back(tempWRef);
    refs.push_back(tempBRef);
    tableInit();

    turn = WHITE;
}

Table::~Table()
{
}

Table *Table::getInstance()
{

    srand(time(NULL));
    std::string fname;
    fname = "./debug/TABLE_debug_" + std::to_string(rand());

    if (!Table ::singleton)
        Table ::singleton = new Table(fname);
    return Table ::singleton;
}

void Table::tableInit()
{

    turn = WHITE;
    editMode = false;
    quit = false;

    checks[W] = std::make_pair(false, 0);
    checks[B] = std::make_pair(false, 0);

    // PAWNS
    for (int i = 0; i < 8; i++)
    {
        // WHITE
        pieceMatrix[1][i].containedPiece = new Pawn(1, i, 1, 'P');

        refs[W][PAWNS].push_back((Pawn *)pieceMatrix[1][i].containedPiece);
        // BLACK
        pieceMatrix[6][i].containedPiece = new Pawn(-1, i, 6, 'p');
        refs[B][PAWNS].push_back((Pawn *)pieceMatrix[6][i].containedPiece);
    }

    // WHITE ROOKS
    pieceMatrix[0][0].containedPiece = new Rook(1, 0, 0, 'R', false);
    refs[W][ROOKS].push_back((Rook *)pieceMatrix[0][0].containedPiece);
    pieceMatrix[0][7].containedPiece = new Rook(1, 7, 0, 'R', false);
    refs[W][ROOKS].push_back((Rook *)pieceMatrix[0][7].containedPiece);
    // BLACK ROOKS
    pieceMatrix[7][0].containedPiece = new Rook(-1, 0, 7, 'r', false);
    refs[B][ROOKS].push_back((Rook *)pieceMatrix[7][0].containedPiece);
    pieceMatrix[7][7].containedPiece = new Rook(-1, 7, 7, 'r', false);
    refs[B][ROOKS].push_back((Rook *)pieceMatrix[7][7].containedPiece);

    // WHITE BISHOPS
    pieceMatrix[0][2].containedPiece = new Bishop(1, 2, 0, 'B');
    refs[W][BISHOPS].push_back((Bishop *)pieceMatrix[0][2].containedPiece);
    pieceMatrix[0][5].containedPiece = new Bishop(1, 5, 0, 'B');
    refs[W][BISHOPS].push_back((Bishop *)pieceMatrix[0][5].containedPiece);
    // BLACK BISHOPS
    pieceMatrix[7][2].containedPiece = new Bishop(-1, 2, 7, 'b');
    refs[B][BISHOPS].push_back((Bishop *)pieceMatrix[7][2].containedPiece);
    pieceMatrix[7][5].containedPiece = new Bishop(-1, 5, 7, 'b');
    refs[B][BISHOPS].push_back((Bishop *)pieceMatrix[7][5].containedPiece);

    // WHITE KNIGHTS
    pieceMatrix[0][1].containedPiece = new Knight(1, 1, 0, 'N');
    refs[W][KNIGHTS].push_back((Knight *)pieceMatrix[0][1].containedPiece);
    pieceMatrix[0][6].containedPiece = new Knight(1, 6, 0, 'N');
    refs[W][KNIGHTS].push_back((Knight *)pieceMatrix[0][6].containedPiece);
    // BLACK KNIGHTS
    pieceMatrix[7][1].containedPiece = new Knight(-1, 1, 7, 'n');
    refs[B][KNIGHTS].push_back((Knight *)pieceMatrix[7][1].containedPiece);
    pieceMatrix[7][6].containedPiece = new Knight(-1, 6, 7, 'n');
    refs[B][KNIGHTS].push_back((Knight *)pieceMatrix[7][6].containedPiece);

    // WHITE ROYAL COUPLE
    pieceMatrix[0][4].containedPiece = new King(1, 4, 0, 'K', false);

    refs[W][KING].push_back((King *)pieceMatrix[0][4].containedPiece);
    pieceMatrix[0][3].containedPiece = new Queen(1, 3, 0, 'Q');
    refs[W][QUEEN].push_back((Queen *)pieceMatrix[0][3].containedPiece);
    // BLACK ROYAL COUPLE
    pieceMatrix[7][4].containedPiece = new King(-1, 4, 7, 'k', false);
    refs[B][KING].push_back((King *)pieceMatrix[7][4].containedPiece);
    pieceMatrix[7][3].containedPiece = new Queen(-1, 3, 7, 'q');

    refs[B][QUEEN].push_back((Queen *)pieceMatrix[7][3].containedPiece);

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pieceMatrix[i][j].containedPiece = NULL;
        }
    }
}

std::vector<std::vector<Cell>> Table::getMatrix()
{
    return pieceMatrix;
}

void Table::debugDraw()
{

    // logfile.open(filename);
    Engine *rigged = Engine::getInstance();

    if (rigged->color)
    {
        logfile << "]======[ WHITE ]======[\n";
    }
    else
        logfile << "]======[ BLACK ]======[\n";

    logfile << "   a b c d e f g h\t\t   a b c d e f g h\n";
    logfile << "   ---------------\t\t   ---------------\n";

    for (int i = 7; i >= 0; i--)
    {
        logfile << i + 1 << "| ";
        // Normal view
        for (int j = 0; j < 8; j++)
        {
            if (pieceMatrix[i][j].containedPiece == NULL)
            {
                logfile << ". ";
            }
            else
            {
                logfile << pieceMatrix[i][j].containedPiece->getType() << " ";
            }
        }

        logfile << "\t\t";

        // Contesters view
        logfile << i + 1 << "| ";
        for (int j = 0; j < 8; j++)
        {
            if (pieceMatrix[i][j].contesters.size() != 0)
            {
                logfile << pieceMatrix[i][j].contesters[0]->getType() << " ";
            }
            else
            {
                logfile << ". ";
            }
        }

        logfile << "\t\t";

        // // Contesters view
        logfile << i + 1 << "| ";
        for (int j = 0; j < 8; j++)
        {
            if (pieceMatrix[i][j].containedPiece != NULL &&
                pieceMatrix[i][j].containedPiece->isPinned)
            {
                logfile << "x ";
            }
            else
            {
                logfile << ". ";
            }
        }

        logfile << std::endl;
    }
    logfile << std::endl;
    logfile << std::endl;
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         logfile << "\n" << (char)(j + 'a') << i + 1 << ":";
    //         logfile << "\nC: ";
    //         for (auto &&p : pieceMatrix[i][j].contesters)
    //         {
    //             logfile << p->type << "-" << (char)(p->file + 'a') << p->rank + 1 << "  ";
    //         }
    //         logfile << " " << pieceMatrix[i][j].contesters.size();
    //         logfile << "\nD: ";
    //         for (auto &&p : pieceMatrix[i][j].defenders)
    //         {
    //             logfile << p->type << "-" << (char)(p->file + 'a') << p->rank + 1 << "  ";
    //         }
    //         logfile << " " << pieceMatrix[i][j].defenders.size() << std::endl;
    //     }
    // }

    // logfile << std::endl << std::endl;
    // logfile.close();
}

/**
 * add contesters 
 * @txt: updates the game table with a new move
 * @param newMove chess move used to update the table
 **/
void Table::addContestersToCell(std::vector<std::vector<Piece *>> references)
{

    int file;
    int rank;

    for (auto reference : references)
    {
        for (auto piece : reference)
        {
            if (piece->isCaptured)
            {
                continue;
            }
            // For each new move that was generated add a new contester entry
            // to targeted cell
            // Pawn contesters are added separately in contestPawn()
            if (toupper(piece->type) == 'P')
                continue;

            for (auto move : piece->moves)
            {
                file = move.destinationFile - 'a';
                rank = move.destinationRank - '1';
                pieceMatrix[rank][file].contesters.push_back(piece);
            }

            for (auto move : piece->defending)
            {
                file = move.destinationFile - 'a';
                rank = move.destinationRank - '1';
                pieceMatrix[rank][file].defenders.push_back(piece);
            }
        }
    }
    // Generate all possible moves for each piece
}

/**
 *
 * Updates the game table with a new move.
 * Updates the contester vectors for each square in the table.
 * Checks whether one of the kings is in check.
 * Draw debug info in debugTable.
 * @param newMove Chess move used to update the table
 */
void Table::updateTable(chessMove newMove)
{

    Engine *thisEngine;
    thisEngine = Engine::getInstance();
    int srcRank = newMove.sourceRank - '1';
    int srcFile = newMove.sourceFile - 'a';
    int destRank = newMove.destinationRank - '1';
    int destFile = newMove.destinationFile - 'a';

    // thisEngine->API->logfile << "UPDATING TABLE WITH MOVE " << newMove.sourceFile\
    //                                     << newMove.sourceRank\
    //                                     << newMove.destinationFile\
    //                                     << newMove.destinationRank << std::endl;

    Cell *source;
    Cell *destination;
    source = &pieceMatrix[srcRank][srcFile];
    destination = &pieceMatrix[destRank][destFile];

    if (destination->containedPiece != NULL)
    {
        destination->containedPiece->isCaptured = true;
    }
    // If not moving from an empty square, change the location
    // of the source piece to destination cell
    // Also update the location in the source piece's instance fields
    if (source->containedPiece != NULL)
    {
        // Promotion check
        if (newMove.promote != 0)
        {
            source->containedPiece->setCaptured(true);
            int promcolor = source->containedPiece->color;
            source->containedPiece = NULL;

            // Piece is placed instead of the pawn before updating in order to continue using
            // default update logic
            switch (newMove.promote)
            {
            case 'q':
                if (promcolor == 1)
                {
                    source->containedPiece = new Queen(promcolor, destFile, destRank, 'Q');
                    refs[W][QUEEN].push_back(source->containedPiece);
                }
                else
                {
                    source->containedPiece = new Queen(promcolor, destFile, destRank, 'q');
                    refs[B][QUEEN].push_back(source->containedPiece);
                }

                break;
            case 'b':
                if (promcolor == 1)
                {
                    source->containedPiece = new Bishop(promcolor, destFile, destRank, 'B');
                    refs[W][BISHOPS].push_back(source->containedPiece);
                }
                else
                {
                    source->containedPiece = new Bishop(promcolor, destFile, destRank, 'b');
                    refs[B][BISHOPS].push_back(source->containedPiece);
                }
                break;
            case 'n':
                if (promcolor == 1)
                {
                    source->containedPiece = new Knight(promcolor, destFile, destRank, 'N');
                    refs[W][KNIGHTS].push_back(source->containedPiece);
                }
                else
                {
                    source->containedPiece = new Knight(promcolor, destFile, destRank, 'n');
                    refs[B][KNIGHTS].push_back(source->containedPiece);
                }
                break;
            case 'r':
                if (promcolor == 1)
                {
                    source->containedPiece = new Rook(promcolor, destFile, destRank, 'R', true);
                    refs[W][ROOKS].push_back(source->containedPiece);
                }
                else
                {
                    source->containedPiece = new Rook(promcolor, destFile, destRank, 'r', true);
                    refs[B][ROOKS].push_back(source->containedPiece);
                }
                break;
            default:
                break;
            }
        }

        // EN PASSANT VIBE CHECK FIINDCA CU FLAGURILE NUI PLACE
        if (toupper(source->containedPiece->type) == 'P' && destination->containedPiece == NULL && destination->file != source->file)
        {
            int rank = newMove.sourceRank - '1';
            int file = newMove.destinationFile - 'a';
            Pawn *p = (Pawn *)pieceMatrix[rank][file].containedPiece;
            p->setCaptured(true);

            pieceMatrix[rank][file].containedPiece = NULL;
        }

        destination->containedPiece = source->containedPiece;
        source->containedPiece = NULL;
        destination->containedPiece->updatePosition(newMove);

        if (toupper(destination->containedPiece->type) == 'K' && abs(destFile - srcFile) == 2)
        {
            Rook *r;
            int rank;
            switch (newMove.destinationFile)
            {
            case 'g':
                rank = newMove.destinationRank - '1';
                r = (Rook *)pieceMatrix[rank][7].containedPiece;
                r->updatePosition(r->returnMove(7, rank, 5, rank, 0));
                pieceMatrix[rank][5].containedPiece = r;
                pieceMatrix[rank][7].containedPiece = NULL;
                break;
            case 'c':
                rank = newMove.destinationRank - '1';
                r = (Rook *)pieceMatrix[rank][0].containedPiece;
                r->updatePosition(r->returnMove(0, rank, 3, rank, 0));
                pieceMatrix[rank][3].containedPiece = r;
                pieceMatrix[rank][0].containedPiece = NULL;
                break;
            }
        }
    }

    // Switch current turn
    turn = !turn;
    if (turn == BLACK)
        turnCounter++;

    // CLEAR OUT ALL THE PREVIOUS CONTESTERS, DUE TO HARD LOGIC TO DETERMINE WHICH SQUARES ARE AFFECTED
    // BY CURRENT MOVE
    // -- POSSIBLE FUTURE OPTIMIZATION IS TO DETERMINE WHICH PIECES ARE AFFECTED BY CURRENT MOVE --
    // Clear previous contesters

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            pieceMatrix[i][j].contesters.clear();
            pieceMatrix[i][j].defenders.clear();

            if (pieceMatrix[i][j].containedPiece != NULL)
            {
                pieceMatrix[i][j].containedPiece->isPinned = false;
            }
        }

    for (auto pieceType : refs[W])
    {
        for (auto piece : pieceType)
        {
            piece->defending.clear();
            piece->possibleMoves();
        }
    }

    for (auto pieceType : refs[B])
    {
        for (auto piece : pieceType)
        {
            piece->defending.clear();
            piece->possibleMoves();
        }
    }

    King *wKing;
    wKing = (King *)refs[W][KING][0];
    King *bKing;
    bKing = (King *)refs[B][KING][0];
    // Add contesters to each cell for each color
    addContestersToCell(refs[W]);
    addContestersToCell(refs[B]);

    setPinnedPieces(wKing);
    setPinnedPieces(bKing);

    // Check if each king is in a contested cell
    Cell kCell;

    kCell = pieceMatrix[wKing->rank][wKing->file];

    if (kCell.contesters.size() > 0)
    {
        // Engine::getInstance()->API->logfile << "BLACK GETTING A CHECK\n" << std::endl;

        checks[W].first = true;
        if (checks[W].second == 3)
        {
            if (Engine::getInstance()->color == WHITE)
            {
                // Engine::getInstance()->API->logfile << "3 CHECK RESIGN\n" << std::endl;
                write(STDOUT_FILENO, "resign\n", 7);
                Table::getInstance()->clearTable();
                Table::getInstance()->tableInit();
            }
        }
        else
            checks[W].second++;
    }

    kCell = pieceMatrix[bKing->rank][bKing->file];

    if (kCell.contesters.size() > 0)
    {
        // Engine::getInstance()->API->logfile << "WHITE GETTING A CHECK"<< std::endl;
        checks[B].first = true;
        if (checks[B].second == 3)
        {
            if (Engine::getInstance()->color == BLACK)
            {
                // Engine::getInstance()->API->logfile << "3 CHECK RESIGN"<< std::endl;
                write(STDOUT_FILENO, "resign\n", 7);
                Table::getInstance()->clearTable();
                Table::getInstance()->tableInit();
            }
        }
        else
            checks[B].second++;
    }
}

void Table::setPinnedPieces(King *xKing)
{
    Cell kCell;

    kCell = pieceMatrix[xKing->rank][xKing->file];

    int rank = xKing->rank + 1, file, firstRank, firstFile;
    bool foundFirst = false;

    // UP
    while (rank <= 7)
    {

        if (pieceMatrix[rank][xKing->file].containedPiece != NULL)
        {
            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][xKing->file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = xKing->file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este tura SAU regina)
            else if ((pieceMatrix[rank][xKing->file].containedPiece->color != xKing->color) && (toupper(pieceMatrix[rank][xKing->file].containedPiece->type) == 'R' || toupper(pieceMatrix[rank][xKing->file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe linia urmatoare
        rank++;
    }
    foundFirst = false;
    rank = xKing->rank - 1;

    // DOWN
    while (rank >= 0)
    {

        if (pieceMatrix[rank][xKing->file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][xKing->file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = xKing->file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este tura SAU regina)
            else if (pieceMatrix[rank][xKing->file].containedPiece->color != xKing->color &&
                     (toupper(pieceMatrix[rank][xKing->file].containedPiece->type) == 'R' || toupper(pieceMatrix[rank][xKing->file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe linia urmatoare
        rank--;
    }

    foundFirst = false;
    file = xKing->file + 1;

    // RIGHT
    while (file <= 7)
    {

        if (pieceMatrix[xKing->rank][file].containedPiece != NULL)
        {
            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[xKing->rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = xKing->rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este tura SAU regina)
            else if (pieceMatrix[xKing->rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[xKing->rank][file].containedPiece->type) == 'R' || toupper(pieceMatrix[xKing->rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe linia urmatoare
        file++;
    }

    foundFirst = false;
    file = xKing->file - 1;

    // LEFT
    while (file >= 0)
    {

        if (pieceMatrix[xKing->rank][file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[xKing->rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = xKing->rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este tura SAU regina)
            else if (pieceMatrix[xKing->rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[xKing->rank][file].containedPiece->type) == 'R' || toupper(pieceMatrix[xKing->rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe linia urmatoare
        file--;
    }

    foundFirst = false;
    rank = xKing->rank + 1;
    file = xKing->file + 1;

    // UP-RIGHT
    while (rank <= 7 && file <= 7)
    {

        if (pieceMatrix[rank][file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este nebun SAU regina)
            else if (pieceMatrix[rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[rank][file].containedPiece->type) == 'B' || toupper(pieceMatrix[rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe pozitia urmatoare
        rank++;
        file++;
    }

    foundFirst = false;
    rank = xKing->rank + 1;
    file = xKing->file - 1;

    // UP-LEFT
    while (rank <= 7 && file >= 0)
    {

        if (pieceMatrix[rank][file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este nebun SAU regina)
            else if (pieceMatrix[rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[rank][file].containedPiece->type) == 'B' || toupper(pieceMatrix[rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe pozitia urmatoare
        rank++;
        file--;
    }

    foundFirst = false;
    rank = xKing->rank - 1;
    file = xKing->file + 1;

    // DOWN-RIGHT
    while (rank >= 0 && file <= 7)
    {

        if (pieceMatrix[rank][file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {

                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este nebun SAU regina)
            else if (pieceMatrix[rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[rank][file].containedPiece->type) == 'B' || toupper(pieceMatrix[rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe pozitia urmatoare
        rank--;
        file++;
    }

    foundFirst = false;
    rank = xKing->rank - 1;
    file = xKing->file - 1;

    // DOWN-LEFT
    while (rank >= 0 && file >= 0)
    {

        if (pieceMatrix[rank][file].containedPiece != NULL)
        {

            // daca primul de culoarea regelui nu a fost gasit
            if (foundFirst == false)
            {
                // verific daca este piesa este de culoarea regelui
                if (pieceMatrix[rank][file].containedPiece->color == xKing->color)
                {
                    foundFirst = true;
                    firstRank = rank;
                    firstFile = file;
                }
                // prima piesa este opusa ca culoare
                else
                    break;
            }
            // daca am gasit deja prima piesa; verific daca a doua piesa este culoare opusa regelui SI (este nebun SAU regina)
            else if (pieceMatrix[rank][file].containedPiece->color != xKing->color && (toupper(pieceMatrix[rank][file].containedPiece->type) == 'B' || toupper(pieceMatrix[rank][file].containedPiece->type) == 'Q'))
            {
                pieceMatrix[firstRank][firstFile].containedPiece->setPinned(true);
                // am gasit cea de-a doua piesa, actualizez flag-ul de pinned
                // break orice ar fi, se stie ca cell-ul contine o piesa, deci este o piesa de aceeasi culoare sau ceva in afara de rook sau queen
                break;
            }
            else
                break;
        }
        // ma mut pe pozitia urmatoare
        rank--;
        file--;
    }
}

void Table::clearTable()
{

    for (int i = 0; i < 2; i++)
    {

        free(refs[W][ROOKS].at(i));
        free(refs[B][ROOKS].at(i));

        free(refs[W][BISHOPS].at(i));
        free(refs[B][BISHOPS].at(i));

        free(refs[W][KNIGHTS].at(i));
        free(refs[B][KNIGHTS].at(i));
    }

    for (int i = 0; i < 8; i++)
    {
        free(refs[W][PAWNS].at(i));
        free(refs[B][PAWNS].at(i));
    }

    refs[W][BISHOPS].clear();
    refs[W][KNIGHTS].clear();
    refs[W][ROOKS].clear();

    refs[B][BISHOPS].clear();
    refs[B][KNIGHTS].clear();
    refs[B][ROOKS].clear();

    refs[W][PAWNS].clear();
    refs[B][PAWNS].clear();

    free(refs[W][KING][0]);
    free(refs[W][QUEEN][0]);

    free(refs[B][KING][0]);
    free(refs[B][QUEEN][0]);

    refs[W][KING].clear();
    refs[W][QUEEN].clear();

    refs[B][KING].clear();
    refs[B][QUEEN].clear();
}
