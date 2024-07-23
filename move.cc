#include "move.h"

const BoardPosn BoardPosn::Invalid(-1, -1);

bool BoardPosn::operator==(const BoardPosn& other) const {
    return file == other.file && rank == other.rank;
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

std::unique_ptr<std::vector<BoardPosn>> BasicMove::execute(Board& board) const {
    board.reset_piece(from);
    board.set_piece(to, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> BasicMove::undo(Board& board) const {
    board.reset_piece(to);
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.reset_piece(captured_posn);
    board.set_piece(to, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, captured_posn});
}

std::unique_ptr<std::vector<BoardPosn>> CaptureMove::undo(Board& board) const {
    board.reset_piece(to);
    board.set_piece(captured_posn, captured_piece);
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, captured_posn});
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(to);
    board.set_piece(to, promoted_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> PromotionMove::undo(Board& board) const {
    board.reset_piece(to);
    if (captured_piece) {
        board.set_piece(to, captured_piece.value());
    }
    board.set_piece(from, moved_piece);

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to});
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::execute(Board& board) const {
    board.reset_piece(from);
    board.reset_piece(rook_from);
    board.set_piece(to, moved_piece);
    board.set_piece(rook_to, Piece(PieceType::Rook, moved_piece.colour));

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, rook_from, rook_to});
}

std::unique_ptr<std::vector<BoardPosn>> CastlingMove::undo(Board& board) const {
    board.reset_piece(to);
    board.reset_piece(rook_to);
    board.set_piece(from, moved_piece);
    board.set_piece(rook_from, Piece(PieceType::Rook, moved_piece.colour));

    return std::make_unique<std::vector<BoardPosn>>(std::initializer_list<BoardPosn>{from, to, rook_from, rook_to});
}

void MoveHistory::push_move(std::unique_ptr<Move> move) {
    moves.push(std::move(move));
}

void MoveHistory::reset() {
    std::stack<std::unique_ptr<Move>>().swap(moves);
}

std::unique_ptr<Move> MoveHistory::pop_last_move() {
    if (moves.empty()) {
        std::cerr << "No moves to undo." << std::endl;
        return nullptr;
    }
    std::unique_ptr<Move> last_move = std::move(moves.top());
    moves.pop();
    return last_move;
}

bool PossibleMove::is_possible_move(BoardPosn from, BoardPosn to) const {
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
