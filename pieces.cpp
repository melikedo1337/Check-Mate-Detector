#include "pieces.h"
#include "board.h"

namespace {
void appendRayMoves(const Board& board, Position from, Color color, int dRow, int dCol, std::vector<Position>& moves) {
    Position current{from.row + dRow, from.col + dCol};

    while (board.inBounds(current)) {
        Piece* target = board.getPiece(current);
        if (target == nullptr) {
            moves.push_back(current);
        } else {
            if (target->getColor() != color) {
                moves.push_back(current);
            }
            break;
        }

        current = Position{current.row + dRow, current.col + dCol};
    }
}
}

Piece::Piece(Color color) : color(color) {}

Color Piece::getColor() const {
    return color;
}

King::King(Color color) : Piece(color) {}
char King::getSymbol() const {
    if (color == Color::White) {
        return 'K';
    }
    return 'k';
}
Piece* King::clone() const { return new King(*this); }
std::vector<Position> King::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            Position to{from.row + dr, from.col + dc};
            if (!board.inBounds(to)) {
                continue;
            }
            Piece* target = board.getPiece(to);
            if (target == nullptr || target->getColor() != color) {
                moves.push_back(to);
            }
        }
    }
    return moves;
}

Queen::Queen(Color color) : Piece(color) {}
char Queen::getSymbol() const {
    if (color == Color::White) {
        return 'Q';
    }
    return 'q';
}
Piece* Queen::clone() const { return new Queen(*this); }
std::vector<Position> Queen::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;
    appendRayMoves(board, from, color, -1, 0, moves);
    appendRayMoves(board, from, color, 1, 0, moves);
    appendRayMoves(board, from, color, 0, -1, moves);
    appendRayMoves(board, from, color, 0, 1, moves);
    appendRayMoves(board, from, color, -1, -1, moves);
    appendRayMoves(board, from, color, -1, 1, moves);
    appendRayMoves(board, from, color, 1, -1, moves);
    appendRayMoves(board, from, color, 1, 1, moves);
    return moves;
}

Rook::Rook(Color color) : Piece(color) {}
char Rook::getSymbol() const {
    if (color == Color::White) {
        return 'R';
    }
    return 'r';
}
Piece* Rook::clone() const { return new Rook(*this); }
std::vector<Position> Rook::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;
    appendRayMoves(board, from, color, -1, 0, moves);
    appendRayMoves(board, from, color, 1, 0, moves);
    appendRayMoves(board, from, color, 0, -1, moves);
    appendRayMoves(board, from, color, 0, 1, moves);
    return moves;
}

Bishop::Bishop(Color color) : Piece(color) {}
char Bishop::getSymbol() const {
    if (color == Color::White) {
        return 'B';
    }
    return 'b';
}
Piece* Bishop::clone() const { return new Bishop(*this); }
std::vector<Position> Bishop::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;
    appendRayMoves(board, from, color, -1, -1, moves);
    appendRayMoves(board, from, color, -1, 1, moves);
    appendRayMoves(board, from, color, 1, -1, moves);
    appendRayMoves(board, from, color, 1, 1, moves);
    return moves;
}

Knight::Knight(Color color) : Piece(color) {}
char Knight::getSymbol() const {
    if (color == Color::White) {
        return 'N';
    }
    return 'n';
}
Piece* Knight::clone() const { return new Knight(*this); }
std::vector<Position> Knight::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;
    const int jumps[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (const auto& jump : jumps) {
        Position to{from.row + jump[0], from.col + jump[1]};
        if (!board.inBounds(to)) {
            continue;
        }
        Piece* target = board.getPiece(to);
        if (target == nullptr || target->getColor() != color) {
            moves.push_back(to);
        }
    }

    return moves;
}

Pawn::Pawn(Color color) : Piece(color) {}
char Pawn::getSymbol() const {
    if (color == Color::White) {
        return 'P';
    }
    return 'p';
}
Piece* Pawn::clone() const { return new Pawn(*this); }
std::vector<Position> Pawn::getPseudoLegalMoves(const Board& board, Position from) const {
    std::vector<Position> moves;

    int dir = 0;
    int startRow = 0;
    if (color == Color::White) {
        dir = -1;
        startRow = 6;
    } else {
        dir = 1;
        startRow = 1;
    }

    Position oneForward{from.row + dir, from.col};
    if (board.inBounds(oneForward) && board.getPiece(oneForward) == nullptr) {
        moves.push_back(oneForward);

        Position twoForward{from.row + 2 * dir, from.col};
        if (from.row == startRow && board.inBounds(twoForward) && board.getPiece(twoForward) == nullptr) {
            moves.push_back(twoForward);
        }
    }

    for (int dc : {-1, 1}) {
        Position capture{from.row + dir, from.col + dc};
        if (!board.inBounds(capture)) {
            continue;
        }
        Piece* target = board.getPiece(capture);
        if (target != nullptr && target->getColor() != color) {
            moves.push_back(capture);
        }
    }

    return moves;
}