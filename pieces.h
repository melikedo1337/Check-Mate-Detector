#ifndef PIECES_H
#define PIECES_H

#include <vector>

class Board;

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

enum class Color {
    White,
    Black
};

class Piece {
protected:
    Color color;

public:
    explicit Piece(Color color);
    virtual ~Piece() = default;

    Color getColor() const;
    virtual char getSymbol() const = 0;
    virtual Piece* clone() const = 0;
    virtual std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const = 0;
};

class King : public Piece {
public:
    explicit King(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

class Queen : public Piece {
public:
    explicit Queen(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

class Rook : public Piece {
public:
    explicit Rook(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

class Bishop : public Piece {
public:
    explicit Bishop(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

class Knight : public Piece {
public:
    explicit Knight(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

class Pawn : public Piece {
public:
    explicit Pawn(Color color);
    char getSymbol() const override;
    Piece* clone() const override;
    std::vector<Position> getPseudoLegalMoves(const Board& board, Position from) const override;
};

#endif