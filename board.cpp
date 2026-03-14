#include "board.h"

#include <cctype>
#include <iostream>
#include <stdexcept>

Board::Board() : board(8, std::vector<Piece*>(8, nullptr)) {}

Board::Board(const Board& other) {
    copyFrom(other);
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Board::~Board() {
    clear();
}

void Board::clear() {
    for (auto& row : board) {
        for (Piece*& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void Board::copyFrom(const Board& other) {
    board.assign(8, std::vector<Piece*>(8, nullptr));
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (other.board[r][c] != nullptr) {
                board[r][c] = other.board[r][c]->clone();
            }
        }
    }
}

bool Board::inBounds(Position pos) const {
    return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
}

Piece* Board::getPiece(Position pos) const {
    if (!inBounds(pos)) {
        return nullptr;
    }
    return board[pos.row][pos.col];
}

void Board::setPiece(Position pos, Piece* piece) {
    if (!inBounds(pos)) {
        delete piece;
        throw std::invalid_argument("Position out of bounds");
    }
    delete board[pos.row][pos.col];
    board[pos.row][pos.col] = piece;
}

void Board::movePiece(Position from, Position to) {
    if (!inBounds(from) || !inBounds(to)) {
        throw std::invalid_argument("Move out of bounds");
    }

    delete board[to.row][to.col];
    board[to.row][to.col] = board[from.row][from.col];
    board[from.row][from.col] = nullptr;
}

Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr) {
                continue;
            }
            if (piece->getColor() == color && std::tolower(piece->getSymbol()) == 'k') {
                return Position{r, c};
            }
        }
    }
    return Position{-1, -1};
}

bool Board::isSquareAttacked(Position square, Color byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr || piece->getColor() != byColor) {
                continue;
            }

            Position from{r, c};
            auto moves = piece->getPseudoLegalMoves(*this, from);
            for (const Position& move : moves) {
                if (move == square) {
                    return true;
                }
            }
        }
    }
    return false;
}

Color Board::opponentOf(Color color) {
    if (color == Color::White) {
        return Color::Black;
    }
    return Color::White;
}

bool Board::isInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!inBounds(kingPos)) {
        return false;
    }
    Color enemy = opponentOf(color);
    return isSquareAttacked(kingPos, enemy);
}

std::vector<Position> Board::getLegalMovesFrom(Position from) const {
    std::vector<Position> legal;
    Piece* piece = getPiece(from);
    if (piece == nullptr) {
        return legal;
    }

    auto candidates = piece->getPseudoLegalMoves(*this, from);
    for (const Position& to : candidates) {
        Board simulation(*this);
        simulation.movePiece(from, to);
        if (!simulation.isInCheck(piece->getColor())) {
            legal.push_back(to);
        }
    }

    return legal;
}

bool Board::hasAnyLegalMove(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr || piece->getColor() != color) {
                continue;
            }

            if (!getLegalMovesFrom(Position{r, c}).empty()) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) const {
    return isInCheck(color) && !hasAnyLegalMove(color);
}

bool Board::isStalemate(Color color) const {
    return !isInCheck(color) && !hasAnyLegalMove(color);
}

bool Board::hasCheckmateInOne(Color attacker) const {
    Color defender = opponentOf(attacker);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr || piece->getColor() != attacker) {
                continue;
            }

            Position from{r, c};
            std::vector<Position> moves = getLegalMovesFrom(from);

            for (const Position& to : moves) {
                Board sim(*this);
                sim.movePiece(from, to);
                if (sim.isCheckmate(defender)) {
                    return true;
                }
            }
        }
    }

    return false;
}

static bool allDefenderResponsesLeadToMateInOne(const Board& afterMove1, Color attacker, Color defender) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = afterMove1.board[r][c];
            if (piece == nullptr || piece->getColor() != defender) {
                continue;
            }

            Position from{r, c};
            std::vector<Position> responses = afterMove1.getLegalMovesFrom(from);

            for (const Position& to : responses) {
                Board sim(afterMove1);
                sim.movePiece(from, to);
                if (!sim.hasCheckmateInOne(attacker)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Board::hasCheckmateInTwo(Color attacker) const {
    Color defender = opponentOf(attacker);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr || piece->getColor() != attacker) {
                continue;
            }

            Position from{r, c};
            std::vector<Position> moves = getLegalMovesFrom(from);

            for (const Position& to : moves) {
                Board sim(*this);
                sim.movePiece(from, to);

                if (sim.isCheckmate(defender)) {
                    continue;
                }

                if (sim.isStalemate(defender)) {
                    continue;
                }

                if (allDefenderResponsesLeadToMateInOne(sim, attacker, defender)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    for (int r = 0; r < 8; ++r) {
        std::cout << 8 - r << ' ';
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece == nullptr) {
                std::cout << ". ";
            } else {
                std::cout << piece->getSymbol() << ' ';
            }
        }
        std::cout << 8 - r << '\n';
    }
    std::cout << "  a b c d e f g h\n";
}

bool Board::parseSquare(const std::string& text, Position& out) {
    if (text.size() != 2) {
        return false;
    }

    char file = static_cast<char>(std::tolower(text[0]));
    char rank = text[1];

    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return false;
    }

    out.col = file - 'a';
    out.row = 8 - (rank - '0');
    return true;
}

Color Board::parseColor(char c) {
    if (c == 'w' || c == 'W') {
        return Color::White;
    }
    return Color::Black;
}