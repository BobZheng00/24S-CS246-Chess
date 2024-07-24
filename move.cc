#include "move.h"
#include "board.h"
#include "status.h"

const BoardPosn BoardPosn::Invalid(-1, -1);

bool BoardPosn::operator==(const BoardPosn& other) const {
    return file == other.file && rank == other.rank;
}

std::string BoardPosn::to_string() const {
    return std::string(1, 'a'+file) + std::to_string(rank+1);
}

UniqueMove get_unable_kingside_castling(ChessColour colour) {
    switch (colour) {
    case ChessColour::White:
        return UniqueMove::UnableWhiteKingSideCastling;
    case ChessColour::Black:
        return UniqueMove::UnableBlackKingSideCastling;
    default:
        return UniqueMove::None;
    }
}

UniqueMove get_unable_queenside_castling(ChessColour colour) {
    switch (colour) {
    case ChessColour::White:
        return UniqueMove::UnableWhiteQueenSideCastling;
    case ChessColour::Black:
        return UniqueMove::UnableBlackQueenSideCastling;
    default:
        return UniqueMove::None;
    }
}

UniqueMove get_unable_all_castling(ChessColour colour) {
    switch (colour) {
    case ChessColour::White:
        return UniqueMove::UnableAllWhiteCastling;
    case ChessColour::Black:
        return UniqueMove::UnableAllBlackCastling;
    default:
        return UniqueMove::None;
    }
}

void handle_execution_status_change(const Move* move, UniqueMove move_type, Status& status) {
    switch (move_type) {
    case UniqueMove::BlackDoublePawnPush:
        status.black_last_double_pawn_push = std::make_unique<BoardPosn>(move->to.file, move->to.rank);
        break;
    case UniqueMove::WhiteDoublePawnPush:
        status.white_last_double_pawn_push = std::make_unique<BoardPosn>(move->to.file, move->to.rank);
        break;
    case UniqueMove::UnableBlackKingSideCastling:
        status.black_can_castle_kingside = false;
        break;
    case UniqueMove::UnableBlackQueenSideCastling:
        status.black_can_castle_queenside = false;
        break;
    case UniqueMove::UnableWhiteKingSideCastling:
        status.white_can_castle_kingside = false;
        break;
    case UniqueMove::UnableWhiteQueenSideCastling:
        status.white_can_castle_queenside = false;
        break;
    case UniqueMove::UnableAllBlackCastling:
        status.black_can_castle_kingside = false;
        status.black_can_castle_queenside = false;
        break;
    case UniqueMove::UnableAllWhiteCastling:
        status.white_can_castle_kingside = false;
        status.white_can_castle_queenside = false;
        break;
    default:
        break;
    }
}

void handle_undo_status_change(const Move* move, UniqueMove move_type, Status& status) {
    switch (move_type) {
    case UniqueMove::BlackDoublePawnPush:
        status.black_last_double_pawn_push = std::make_unique<BoardPosn>(-1, -1);
        break;
    case UniqueMove::WhiteDoublePawnPush:
        status.white_last_double_pawn_push = std::make_unique<BoardPosn>(-1, -1);
        break;
    case UniqueMove::UnableBlackKingSideCastling:
        status.black_can_castle_kingside = true;
        break;
    case UniqueMove::UnableBlackQueenSideCastling:
        status.black_can_castle_queenside = true;
        break;
    case UniqueMove::UnableWhiteKingSideCastling:
        status.white_can_castle_kingside = true;
        break;
    case UniqueMove::UnableWhiteQueenSideCastling:
        status.white_can_castle_queenside = true;
        break;
    case UniqueMove::UnableAllBlackCastling:
        status.black_can_castle_kingside = true;
        status.black_can_castle_queenside = true;
        break;
    case UniqueMove::UnableAllWhiteCastling:
        status.white_can_castle_kingside = true;
        status.white_can_castle_queenside = true;
        break;
    default:
        break;
    }
}

BoardPosn& BoardPosn::operator+=(const BoardPosn& other) {
    file += other.file;
    rank += other.rank;
    return *this;
}

BoardPosn BoardPosn::operator+(const BoardPosn& other) {
    return {file+other.file, rank+other.rank};
}

bool BoardPosn::on_board() const {
    return file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
}

std::unique_ptr<std::vector<BoardPosn>> BasicMove::execute(Board& board, Status& status) const {
    board.reset_piece(from);
    board.set_piece(to, moved_piece);
    handle_execution_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> BasicMove::undo(Board& board, Status& status) const {
    board.reset_piece(to);
    board.set_piece(from, moved_piece);
    handle_undo_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::execute(Board& board, Status& status) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.reset_piece(captured_posn);
    board.set_piece(to, moved_piece);
    handle_execution_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, captured_posn});
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::undo(Board& board, Status& status) const {
    board.reset_piece(to);
    board.set_piece(captured_posn, captured_piece);
    board.set_piece(from, moved_piece);
    handle_undo_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, captured_posn});
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::execute(Board& board, Status& status) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.set_piece(to, promoted_piece);
    handle_execution_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::undo(Board& board, Status& status) const {
    board.reset_piece(to);
    if (captured_piece) {
        board.set_piece(to, captured_piece.value());
    }
    board.set_piece(from, moved_piece);
    handle_undo_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::execute(Board& board, Status& status) const {
    board.reset_piece(from);
    board.reset_piece(rook_from);
    board.set_piece(to, moved_piece);
    board.set_piece(rook_to, Piece(PieceType::Rook, moved_piece.colour));
    handle_execution_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, rook_from, rook_to});
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::undo(Board& board, Status& status) const {
    board.reset_piece(to);
    board.reset_piece(rook_to);
    board.set_piece(from, moved_piece);
    board.set_piece(rook_from, Piece(PieceType::Rook, moved_piece.colour));
    handle_undo_status_change(this, move_type, status);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, rook_from, rook_to});
}

void MoveHistory::push_move(std::unique_ptr<Move> move) {
    moves.push(std::move(move));
}

void MoveHistory::reset() {
    std::stack<std::unique_ptr<Move>>().swap(moves);
}

void MoveHistory::undo_last_move(Board& board, Status& status) {
    if (moves.empty()) {
        std::cerr << "No moves to undo." << std::endl;
        return;
    }
    moves.top()->undo(board, status);
    moves.pop();
    if (moves.top()->move_type == UniqueMove::BlackDoublePawnPush) {
        status.black_last_double_pawn_push = std::make_unique<BoardPosn>(moves.top()->to.file, moves.top()->to.rank);
    }
    else if (moves.top()->move_type == UniqueMove::WhiteDoublePawnPush) {
        status.white_last_double_pawn_push = std::make_unique<BoardPosn>(moves.top()->to.file, moves.top()->to.rank);
    }
    return;
}

bool PossibleMove::is_possible_move(BoardPosn& from, BoardPosn& to) const {
    for (const auto& move : moves) {
        if (move->from == from && move->to == to) {
            return true;
        }
    }
    return false;
}

void PossibleMove::add_move(std::unique_ptr<Move> move) {
    moves.emplace_back(std::move(move));
}
