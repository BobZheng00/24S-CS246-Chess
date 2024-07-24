#include "piece.h"

// Definitions of white piece constants
const Piece Piece::WhiteKing(PieceType::King, ChessColour::White);
const Piece Piece::WhiteQueen(PieceType::Queen, ChessColour::White);
const Piece Piece::WhiteRook(PieceType::Rook, ChessColour::White);
const Piece Piece::WhiteBishop(PieceType::Bishop, ChessColour::White);
const Piece Piece::WhiteKnight(PieceType::Knight, ChessColour::White);
const Piece Piece::WhitePawn(PieceType::Pawn, ChessColour::White);

// Definitions of black piece constants
const Piece Piece::BlackKing(PieceType::King, ChessColour::Black);
const Piece Piece::BlackQueen(PieceType::Queen, ChessColour::Black);
const Piece Piece::BlackRook(PieceType::Rook, ChessColour::Black);
const Piece Piece::BlackBishop(PieceType::Bishop, ChessColour::Black);
const Piece Piece::BlackKnight(PieceType::Knight, ChessColour::Black);
const Piece Piece::BlackPawn(PieceType::Pawn, ChessColour::Black);

char Piece::to_char() const {
    return to_underlying(colour)? to_underlying(type) : tolower(to_underlying(type));
}

ChessColour opposite_colour(ChessColour colour) {
    return static_cast<ChessColour>(!static_cast<bool>(colour));
}

int get_piece_value(PieceType type) {
    switch (type) {
        case PieceType::King:
            return 10;
        case PieceType::Queen:
            return 9;
        case PieceType::Rook:
            return 5;
        case PieceType::Bishop:
            return 3;
        case PieceType::Knight:
            return 3;
        case PieceType::Pawn:
            return 1;
    }
    return 0;
}
