# Checkmate Detector

A C++ command-line tool that analyzes chess positions for checkmate, stalemate, and forced mating sequences up to two moves deep.

## Features

- Detect if a side is already in **checkmate**
- Detect if a side is in **stalemate**
- Detect if a side is in **check** (but not mated)
- Find **checkmate in 1** — whether the opponent has a move that delivers immediate checkmate
- Find **checkmate in 2** — whether the opponent can force checkmate in two moves regardless of any response

## Build

### Using CMake (recommended)

```bash
cmake -S . -B build
cmake --build build
./build/checkmate_detector
```

### Using g++ directly

```bash
g++ -std=c++17 -o checkmate_detector main.cpp board.cpp pieces.cpp
./checkmate_detector
```

## Usage

Run the program, then follow the prompts:

```
=== Checkmate Detector ===
Input each piece like: wK e1 (white king on e1)
Piece letters: K Q R B N P. Colors: w/b.
How many pieces are on the board? 5
Piece 1: bK h8
Piece 2: bP f7
Piece 3: bP g7
Piece 4: bP h7
Piece 5: wR e8
Analyze for which side? (w/b): b
```

**Piece format:** `<color><type> <square>`
- Colors: `w` (white), `b` (black)
- Types: `K` King, `Q` Queen, `R` Rook, `B` Bishop, `N` Knight, `P` Pawn
- Squares: standard algebraic notation, e.g. `e4`, `h8`, `a1`

### Example output

```
  a b c d e f g h
8 . . . . R . . k 8
7 . . . . . p p p 7
...
  a b c d e f g h

--- Position Analysis ---
Black is in checkmate.
White has checkmate in 1.
```

## Running the Tests

```bash
bash tests.sh
```

The test script covers all four scenarios:

| Test | Description |
|---|---|
| Already checkmate | Side is in checkmate with no escape |
| Stalemate | Side has no legal moves and is not in check |
| Checkmate in 1 | Opponent has one move that delivers immediate checkmate |
| Checkmate in 2 | Opponent can force checkmate in two moves |
| No forced mate | Open position with no forced sequence |

## Project Structure

```
.
├── main.cpp          # Entry point and user input handling
├── board.cpp         # Board logic, move generation, analysis
├── board.h
├── pieces.cpp        # Piece move generation (King, Queen, Rook, Bishop, Knight, Pawn)
├── pieces.h
├── CMakeLists.txt    # CMake build configuration
└── tests.sh          # Shell test script
```

## Requirements

- C++17 or later
- CMake 3.16+ (optional, for CMake builds)
- GCC or Clang

## Notes

- En passant and castling are not implemented
- Pawn promotion is not handled (promoted pieces must be entered directly)
- The checkmate-in-2 search is exhaustive: it tries all attacker moves and verifies every possible defender response leads to a forced mate
