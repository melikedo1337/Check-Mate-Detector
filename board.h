#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"

#include <string>
#include <vector>

class Board {
public:
    std::vector<std::vector<Piece*>> board;

    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();

    bool inBounds(Position pos) const;
    Piece* getPiece(Position pos) const;
    void setPiece(Position pos, Piece* piece);
    void movePiece(Position from, Position to);

    Position findKing(Color color) const;
    bool isSquareAttacked(Position square, Color byColor) const;
    bool isInCheck(Color color) const;
    std::vector<Position> getLegalMovesFrom(Position from) const;
    bool hasAnyLegalMove(Color color) const;

    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    bool hasCheckmateInOne(Color attacker) const;
    bool hasCheckmateInTwo(Color attacker) const;

    void print() const;

    static bool parseSquare(const std::string& text, Position& out);
    static Color parseColor(char c);
    static Color opponentOf(Color color);

private:
    void clear();
    void copyFrom(const Board& other);
};

#endif