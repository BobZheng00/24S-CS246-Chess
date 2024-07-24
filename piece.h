#pragma once

#include "utils.h"

#include <cctype>
#include <utility>

enum class PieceType : char {
    King = 'K',
    Queen = 'Q',
    Rook = 'R',
    Bishop = 'B',
    Knight = 'N',
    Pawn = 'P'
};

int get_piece_value(PieceType type);

enum class ChessColour : bool {
    White = true,
    Black = false
};

ChessColour opposite_colour(ChessColour colour);

struct Piece {
    PieceType type;
    ChessColour colour;

    constexpr Piece(PieceType type, ChessColour colour): type(type), colour(colour) {};
    constexpr bool operator==(const Piece& other) const {
        return type == other.type && colour == other.colour;
    }
    char to_char() const;

    // Constants for white pieces
    static const Piece WhiteKing;
    static const Piece WhiteQueen;
    static const Piece WhiteRook;
    static const Piece WhiteBishop;
    static const Piece WhiteKnight;
    static const Piece WhitePawn;

    // Constants for black pieces
    static const Piece BlackKing;
    static const Piece BlackQueen;
    static const Piece BlackRook;
    static const Piece BlackBishop;
    static const Piece BlackKnight;
    static const Piece BlackPawn;
};
