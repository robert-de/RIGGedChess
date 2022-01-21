#include "Piece.h"
#include "Table.h"
#include "boardAPI.h"

Piece::Piece(int colour, int file, int rank, char type)
{
    this->setColor(colour);
    this->setFile(file);
    this->setRank(rank);
    this->setType(type);
    this->setCaptured(false);
    this->setPinned(false);
}

Piece::Piece()
{
}

Piece::~Piece() {}

// Return row
int Piece::getRank()
{
    return this->rank;
}

// Return collumn
int Piece::getFile()
{
    return this->file;
}

int Piece::getColor()
{
    return this->color;
}

bool Piece::getCaptured()
{
    return this->isCaptured;
}

bool Piece::getPinned()
{
    return this->isPinned;
}

char Piece::getType()
{
    return this->type;
}

std::vector<chessMove> Piece::getMoves()
{
    return this->moves;
};

void Piece::setColor(int c)
{
    this->color = c;
}

void Piece::setRank(int r)
{
    this->rank = r;
}

void Piece::setFile(int f)
{
    this->file = f;
}

void Piece::setCaptured(bool isCaptured)
{
    this->isCaptured = isCaptured;
}

void Piece::setPinned(bool pin)
{
    this->isPinned = pin;
}

void Piece::setType(char t)
{
    this->type = t;
}

// Update the field and rank fields in the Piece instance
void Piece::updatePosition(chessMove M)
{
    this->setFile(M.destinationFile - 'a');
    this->setRank(M.destinationRank - '1');
}

void Piece::addMove(chessMove m)
{
    this->moves.push_back(m);
}

chessMove Piece::returnMove(int sf, int sr, int df, int dr, char promote)
{
    chessMove m;
    m.sourceFile = sf + 97;
    m.sourceRank = sr + 49;

    m.destinationFile = df + 97;
    m.destinationRank = dr + 49;

    m.promote = promote;

    m.enPassant = false;
    m.castle = false;

    return m;
}

bool Piece::outOfBounds(int f, int r)
{
    // Make sure the check respects board bounds
    if (f < 0 || r < 0)
        return true;
    if (f > 7 || r > 7)
        return true;

    return false;
}

bool Piece::checkCapture(int f, int r, int myColour)
{
    Table *gameTable = Table::getInstance();
    auto matrix = gameTable->getMatrix();

    // Make sure the check respects board bounds
    if (f < 0 || r < 0)
        return false;
    if (f > 7 || r > 7)
        return false;

    if ((!this->checkFree(f, r)))
    {
        if (matrix[r][f].containedPiece->getColor() != myColour)
            return true;
    }

    return false;
}

bool Piece::checkDefend(int f, int r, int myColour)
{
    Table *gameTable = Table::getInstance();
    auto matrix = gameTable->getMatrix();

    // Make sure the check respects board bounds
    if (f < 0 || r < 0)
        return false;
    if (f > 7 || r > 7)
        return false;

    if ((!this->checkFree(f, r)) && matrix[r][f].containedPiece->getType() != 'k' && matrix[r][f].containedPiece->getType() != 'K')
    {

        if (matrix[r][f].containedPiece->getColor() == myColour)
            return true;
    }

    return false;
}

bool Piece::checkFree(int f, int r)
{
    Table *gameTable = Table::getInstance();
    auto matrix = gameTable->getMatrix();

    // Make sure the check respects board bounds
    if (f < 0 || r < 0)
        return false;
    if (f > 7 || r > 7)
        return false;

    if (matrix[r][f].containedPiece == NULL)
        return true;
    else
        return false;
}

std::string Piece::possMovesStr()
{
    std::string msg = "";
    msg += "\nGenerating moves for ";

    if (this->color == 1)
        msg += "White ";
    else
        msg += "Black ";

    msg += this->type;
    msg += " at ";
    msg += (char)('a' + file);
    msg += (char)('1' + rank);

    return msg;
}

std::string Piece::genMoveStr(chessMove move)
{
    std::string msg = "\t";
    msg += "Considering move ";

    msg += move.sourceFile;
    msg += move.sourceRank;
    msg += move.destinationFile;
    msg += move.destinationRank;

    msg += " ";
    msg += (char)(move.enPassant + '0');

    return msg;
}

bool Piece ::checkContested(int file, int rank)
{
    Table *table = Table ::getInstance();
    Cell target;
    target = table->pieceMatrix[rank][file];

    for (auto piece : target.contesters)
    {
        if (piece->color != this->color)
            return true;
    }

    return false;
}
