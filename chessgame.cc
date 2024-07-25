#include "chessgame.h"

ChessGame::ChessGame() : _status(), _board(), _move_factory(_board, _status) {}

void ChessGame::regular_init() {
    _board.standard_init();
    _status.clear();
}

void ChessGame::setup_init() {
    _board.clear();
    _status.clear();
    _status.black_can_castle_kingside = false;
    _status.black_can_castle_queenside = false;
    _status.white_can_castle_kingside = false;
    _status.white_can_castle_queenside = false;
}

void ChessGame::reset() {
    _board.clear();
    _status.clear();
}

void ChessGame::set_turn(ChessColour colour) {
    _status.cur_turn = colour;
}

void ChessGame::set_piece(const BoardPosn& posn, const Piece& piece) {
    _board.set_piece(posn, piece);
}

void ChessGame::reset_piece(const BoardPosn& posn) {
    _board.reset_piece(posn);
}

bool ChessGame::is_valid_setup() const {
    int white_king_count = 0;
    int black_king_count = 0;

    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            if (_board.get_piece(file, rank)) {
                if (_board.get_piece(file, rank).value().type == PieceType::King) {
                    if (_board.get_piece(file, rank).value().colour == ChessColour::White) {
                        ++white_king_count;
                    } else {
                        ++black_king_count;
                    }
                }
                if (white_king_count > 1 || black_king_count > 1) {
                    return false;
                }
                if (_board.get_piece(file, rank).value().type == PieceType::Pawn) {
                    if (on_nth_rank(_board.get_piece(file, rank).value().colour, {file, rank}, 8)) {
                        return false;
                    }
                }
            }
        }
    }

    return white_king_count == 1 && black_king_count == 1 && !_move_factory.is_in_check(ChessColour::White) && !_move_factory.is_in_check(ChessColour::Black);
}

void ChessGame::set_status(Result result) {
    _status.result = result;
}

Result ChessGame::get_status() const {
    return _status.result;
}

bool ChessGame::execute_move(const BoardPosn& from, const BoardPosn& to, std::optional<PieceType> opt_promotion) {
    if (_status.result != Result::Unfinished && _status.result != Result::WhiteInCheck && _status.result != Result::BlackInCheck) {
        return false;
    }
    if (!_board.get_piece(from) || _board.get_piece(from).value().colour != _status.cur_turn) {
        return false;
    }

    auto moves = _move_factory.get_moves(from);

    for (auto& move: moves->moves) {
        if (move->to == to) {
            auto promotion_ptr = dynamic_cast<PromotionMove*>(move.get());
            if (promotion_ptr) {
                if (!opt_promotion || opt_promotion.value() == PieceType::King || opt_promotion.value() == PieceType::Pawn) {
                    return false;
                }
                else {
                    promotion_ptr->promoted_piece = Piece(opt_promotion.value(), _status.cur_turn);
                }
            }
            if (_move_factory.will_move_result_check(*move)) return false;
            move->execute(_board, _status); // player turn has changed

            // check for checkmate, stalemate, and check
            if (_move_factory.is_in_check(_status.cur_turn)) {
                if (_move_factory.is_checkmated()) {
                    if (_status.cur_turn == ChessColour::White) {
                        _status.result = Result::BlackWin;
                    } else {
                        _status.result = Result::WhiteWin;
                    }
                } else if (_status.cur_turn == ChessColour::White) {
                    _status.result = Result::WhiteInCheck;
                } else {
                    _status.result = Result::BlackInCheck;
                }
            }
            else if (_move_factory.is_stalemated()) {
                _status.result = Result::Draw;
            }
            else {
                _status.result = Result::Unfinished;
            }
            _status.move_history->push_move(std::move(move));
            return true;
        }
    }

    return false;
}

void ChessGame::undo_move() {
    _status.move_history->undo_last_move(_board, _status);
    if (_move_factory.is_in_check(_status.cur_turn)) {
        if (_move_factory.is_checkmated()) {
            if (_status.cur_turn == ChessColour::White) {
                _status.result = Result::BlackWin;
            } else {
                _status.result = Result::WhiteWin;
            }
        } else if (_status.cur_turn == ChessColour::White) {
            _status.result = Result::WhiteInCheck;
        } else {
            _status.result = Result::BlackInCheck;
        }
    }
    else if (_move_factory.is_stalemated()) {
        _status.result = Result::Draw;
    }
    else {
        _status.result = Result::Unfinished;
    }
}
