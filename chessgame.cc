#include "chessgame.h"

std::unique_ptr<std::vector<BoardPosn>> get_rerendered_posns(const Move* move) {
    if (dynamic_cast<const BasicMove*>(move)) {
        std::unique_ptr<std::vector<BoardPosn>> posns = std::make_unique<std::vector<BoardPosn>>();
        posns->push_back(move->from);
        posns->push_back(move->to);
        return posns;
    }
    else if (dynamic_cast<const CaptureMove*>(move)) {
        std::unique_ptr<std::vector<BoardPosn>> posns = std::make_unique<std::vector<BoardPosn>>();
        posns->push_back(move->from);
        posns->push_back(move->to);
        posns->push_back(dynamic_cast<const CaptureMove*>(move)->captured_posn);
        return posns;
    }
    else if (dynamic_cast<const PromotionMove*>(move)) {
        std::unique_ptr<std::vector<BoardPosn>> posns = std::make_unique<std::vector<BoardPosn>>();
        posns->push_back(move->from);
        posns->push_back(move->to);
        return posns;
    }
    else if (dynamic_cast<const CastlingMove*>(move)) {
        std::unique_ptr<std::vector<BoardPosn>> posns = std::make_unique<std::vector<BoardPosn>>();
        posns->push_back(move->from);
        posns->push_back(move->to);
        posns->push_back(dynamic_cast<const CastlingMove*>(move)->rook_from);
        posns->push_back(dynamic_cast<const CastlingMove*>(move)->rook_to);
        return posns;
    }
    return nullptr;
}

ChessGame::ChessGame() : _status(), _board(), _move_factory(_board, _status) {}

void ChessGame::regular_init() {
    reset();
    std::unique_ptr<std::vector<BoardPosn>> posns{new std::vector<BoardPosn>};
    _board.standard_init();
    _status.clear();
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            if (_board.get_piece(file, rank)) {
                posns->push_back(BoardPosn(file, rank));
            }
        }
    }
    _board.notify_observers(std::move(posns), _status.result);
}

void ChessGame::setup_init() {
    reset();
    _status.black_can_castle_kingside = false;
    _status.black_can_castle_queenside = false;
    _status.white_can_castle_kingside = false;
    _status.white_can_castle_queenside = false;
}

void ChessGame::reset() {
    std::unique_ptr<std::vector<BoardPosn>> posns{new std::vector<BoardPosn>};
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            if (_board.get_piece(file, rank)) {
                posns->push_back(BoardPosn(file, rank));
            }
        }
    }
    _board.clear();
    _status.clear();
    _board.notify_observers(std::move(posns), _status.result);
}

void ChessGame::set_turn(ChessColour colour) {
    _status.cur_turn = colour;
}

ChessColour ChessGame::get_turn() const {
    return _status.cur_turn;
}

void ChessGame::set_piece(const BoardPosn& posn, const Piece& piece) {
    _board.set_piece(posn, piece);
    std::unique_ptr<std::vector<BoardPosn>> posns{new std::vector<BoardPosn>{posn}};
    std::cout << (posns->empty() ? "empty" : "not empty") << std::endl;
    _board.notify_observers(std::move(posns), _status.result);
}

void ChessGame::reset_piece(const BoardPosn& posn) {
    _board.reset_piece(posn);
    std::unique_ptr<std::vector<BoardPosn>> posns{new std::vector<BoardPosn>{posn}};
    std::cout << (posns->empty() ? "empty" : "not empty") << std::endl;
    _board.notify_observers(std::move(posns), _status.result);
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

BoardSubject* ChessGame::get_board_for_observers() {
    return &_board;
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
            std::cout << "move found" << std::endl;
            auto promotion_ptr = dynamic_cast<PromotionMove*>(move.get());
            std::cout << "promotion_ptr: " << promotion_ptr << std::endl;
            if (promotion_ptr) {
                if (!opt_promotion || opt_promotion.value() == PieceType::King || opt_promotion.value() == PieceType::Pawn) {
                    return false;
                }
                else {
                    promotion_ptr->promoted_piece = Piece(opt_promotion.value(), _status.cur_turn);
                }
            }
            if (_move_factory.will_move_result_check(*move)) return false;
            std::cout << "move will not result in check" << std::endl;
            move->execute(_board, _status); // player turn has changed
            std::cout << "move executed" << std::endl;
            // check for checkmate, stalemate, and check
            if (_move_factory.is_in_check(_status.cur_turn)) {
                std::cout << "in check" << std::endl;
                if (_move_factory.is_checkmated()) {
                    std::cout << "checkmated" << std::endl;
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
                std::cout << "stalemated" << std::endl;
                _status.result = Result::Draw;
            }
            else {
                _status.result = Result::Unfinished;
            }
            std::cout << "status updated" << std::endl;
            _status.move_history->push_move(std::move(move));
            _board.notify_observers(get_rerendered_posns(_status.move_history->last_move()), _status.result);
            return true;
        }
    }

    return false;
}

void ChessGame::undo_move() {
    std::unique_ptr<std::vector<BoardPosn>> posns = get_rerendered_posns(_status.move_history->last_move());
    if (!_status.move_history->undo_last_move(_board, _status)) return;
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
    _board.notify_observers(std::move(posns), _status.result);
}
