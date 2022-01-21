#include "Cell.hpp"

Cell::Cell() {
}

Cell::~Cell() {
}

void Cell::defenderGenerator() {
    if (containedPiece == NULL) {
        return;
    }

    bool color = containedPiece->color;

    for (auto i : contesters) {
        if (i->color == color) {
            defenders.push_back(i);
        }
    }
}
