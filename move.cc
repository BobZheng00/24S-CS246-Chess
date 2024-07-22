#include "move.h"

const BoardPosn BoardPosn::Invalid(-1, -1);

bool BoardPosn::operator==(const BoardPosn& other) const {
    return row == other.row && col == other.col;
}

std::unique_ptr<std::vector<BoardPosn>> BasicMove::execute(Board& board) const {
    board.reset_piece(from);
    board.set_piece(to, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to);
}

std::unique_ptr<std::vector<BoardPosn>> BasicMove::undo(Board& board) const {
    board.reset_piece(to);
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to);
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.reset_piece(captured_posn);
    board.set_piece(to, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to, captured_posn);
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::undo(Board& board) const {
    board.reset_piece(to);
    board.set_piece(captured_posn, captured_piece);
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to, captured_posn);
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.set_piece(to, promoted_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to);
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::undo(Board& board) const {
    board.reset_piece(to);
    if (captured_piece) {
        board.set_piece(to, captured_piece.value());
    }
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(from, to);
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(rook_from);
    board.set_piece(to, moved_piece);
    board.set_piece(rook_to, Piece(PieceType::Rook, moved_piece.colour));

    return std::make_unique<std::vector<BoardPosn>>(from, to, rook_from, rook_to);
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::undo(Board& board) const {
    board.reset_piece(to);
    board.reset_piece(rook_to);
    board.set_piece(from, moved_piece);
    board.set_piece(rook_from, Piece(PieceType::Rook, moved_piece.colour));

    return std::make_unique<std::vector<BoardPosn>>(from, to, rook_from, rook_to);
}
