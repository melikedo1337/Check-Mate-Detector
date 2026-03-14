#!/bin/bash

PASS=0
FAIL=0

run_test() {
    local name="$1"
    local input="$2"
    local expected="$3"

    output=$(echo "$input" | ./checkmate_detector 2>&1)

    if echo "$output" | grep -qF "$expected"; then
        echo "PASS: $name"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $name"
        echo "  Expected to find: $expected"
        echo "  Got:"
        echo "$output" | sed 's/^/    /'
        FAIL=$((FAIL + 1))
    fi
}

# --- Already checkmate ---
# bK h8, bP f7 g7 h7, wR e8
# Rook on e8 attacks f8 g8 h8; king trapped by own pawns
run_test "Already checkmate" \
"5
bK h8
bP f7
bP g7
bP h7
wR e8
b" \
"Black is in checkmate."

# --- Stalemate ---
# bK a8, wK c7, wQ b6
# King on a8 has no legal moves and is not in check
run_test "Stalemate" \
"3
bK a8
wK c7
wQ b6
b" \
"Black is in stalemate."

# --- Checkmate in 1 ---
# bK h8, bP f7 g7 h7, wQ d4
# Qd4-h8# is checkmate in 1 (queen delivers back-rank mate)
run_test "Checkmate in 1" \
"5
bK h8
bP f7
bP g7
bP h7
wQ d8
b" \
"White has checkmate in 1."

# --- Checkmate in 2 ---
# bK h8, wR a1 b2
# 1. Ra1-h1+ Kh8-g8  2. Rh1-h8#
# or  1. Ra1-h1+ Kh8-... followed by Rb2-h2/g2 etc.
run_test "Checkmate in 2" \
"3
bK h8
wR a1
wR b2
b" \
"White has checkmate in 2."

# --- No forced mate ---
# bK e5, wK e3 - open position, no forced mate
run_test "No forced mate in 1 or 2" \
"2
bK e5
wK e3
b" \
"No forced checkmate in 1 or 2 found for White."

echo ""
echo "Results: $PASS passed, $FAIL failed"
if [ "$FAIL" -eq 0 ]; then
    exit 0
else
    exit 1
fi