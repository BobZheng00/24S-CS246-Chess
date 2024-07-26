#include "movefactory.h"

bool on_nth_rank(ChessColour colour, const BoardPosn& posn, int rank) {
    if (to_underlying(colour)) {
        return (posn.rank+1 == rank);
    }
    else {
        return (posn.rank == 8-rank);
    }
}

BoardPosn push_n_rank(ChessColour colour, const BoardPosn& posn, int rank) {
    if (to_underlying(colour)) {
        return {posn.file, posn.rank+rank};
    }
    else {
        return {posn.file, posn.rank-rank};
    }
}

std::unique_ptr<PossibleMove> MoveFactory::get_moves(const BoardPosn& posn) const {
    if (_board.get_piece(posn) == std::nullopt) return std::make_unique<PossibleMove>();
    switch (_board.get_piece(posn).value().type) {
        case PieceType::Rook:
            return _rook_moves(posn);
        case PieceType::Bishop:
            return _bishop_moves(posn);
        case PieceType::Queen:
            return _queen_moves(posn);
        case PieceType::Knight:
            return _knight_moves(posn);
        case PieceType::Pawn:
            return _pawn_moves(posn);
        case PieceType::King:
            return _king_moves(posn);
        default:
            return std::make_unique<PossibleMove>();
    }
}

bool MoveFactory::is_move_safe(const Move& move) const {
    Board tmp_board = _board;
    GameStatus tmp_status = _status;

    move.execute(tmp_board, tmp_status);

    MoveFactory tmp_factory{tmp_board, tmp_status};
    auto all_opponent_moves = tmp_factory.get_all_moves(opposite_colour(move.moved_piece.colour));

    for (auto& opponent_move : all_opponent_moves->moves) {
        if (opponent_move->to == move.to) {
            return false;
        }
    }
    return true;
}

bool MoveFactory::is_capture_valuable(const CaptureMove& move) const {
    if (is_move_safe(move)) return true;
    if (get_piece_value(move.captured_piece.type) > get_piece_value(move.moved_piece.type)) return true;
    return false;
}

bool MoveFactory::is_in_check(ChessColour colour) const {
    auto king_posn = _board.get_king_posn(colour);
    auto all_opponent_moves = get_all_moves(opposite_colour(colour));

    for (auto& opponent_move : all_opponent_moves->moves) {
        if (opponent_move->to == king_posn) {
            return true;
        }
    }
    return false;
}

bool MoveFactory::is_move_evading_capture(const Move& move) const {
    Board tmp_board = _board;
    GameStatus tmp_status = _status;

    MoveFactory tmp_factory{tmp_board, tmp_status};
    auto all_opponent_moves = tmp_factory.get_all_moves(opposite_colour(move.moved_piece.colour));
    bool is_in_capture = false;

    for (auto& opponent_move : all_opponent_moves->moves) {
        if (opponent_move->from == move.to) {
            is_in_capture = true;
        }
    }

    move.execute(tmp_board, tmp_status);

    auto all_opponent_moves_after = tmp_factory.get_all_moves(opposite_colour(move.moved_piece.colour));

    for (auto& opponent_move : all_opponent_moves_after->moves) {
        if (opponent_move->to == move.to) {
            return false;
        }
    }
    return true;
}

bool MoveFactory::will_attack_multi_pieces_next(const Move& move) const {
    Board tmp_board = _board;
    GameStatus tmp_status = _status;

    move.execute(tmp_board, tmp_status);

    MoveFactory tmp_factory{tmp_board, tmp_status};
    int count = 0;

    auto moves = tmp_factory.get_moves(move.to);

    for (auto& next_move : moves->moves) {
        if (dynamic_cast<CaptureMove*>(next_move.get())) {
            ++count;
        }
    }

    return count > 1;
}

bool MoveFactory::will_move_result_check(const Move& move) const {
    Board tmp_board = _board;
    GameStatus tmp_status = _status;

    move.execute(tmp_board, tmp_status);

    MoveFactory tmp_factory{tmp_board, tmp_status};
    return tmp_factory.is_in_check(move.moved_piece.colour);
}

bool MoveFactory::is_checkmated() const {
    if (!is_in_check(_status.cur_turn)) return false;

    if (_king_moves(_board.get_king_posn(_status.cur_turn))->moves.size() > 0) return false;

    auto all_moves = get_all_moves(_status.cur_turn);

    for (auto& move : all_moves->moves) {
        Board tmp_board = _board;
        GameStatus tmp_status = _status;

        move->execute(tmp_board, tmp_status);

        MoveFactory tmp_factory{tmp_board, tmp_status};
        if (!tmp_factory.is_in_check(_status.cur_turn)) {
            return false;
        }
    }
    
    return true;
}

bool MoveFactory::is_stalemated() const {
    if (is_in_check(_status.cur_turn)) return false;

    if (_king_moves(_board.get_king_posn(_status.cur_turn))->moves.size() > 0) return false;

    auto all_moves = get_all_moves(_status.cur_turn);

    for (auto& move : all_moves->moves) {
        Board tmp_board = _board;
        GameStatus tmp_status = _status;

        move->execute(tmp_board, tmp_status);

        MoveFactory tmp_factory{tmp_board, tmp_status};
        if (!tmp_factory.is_in_check(_status.cur_turn)) {
            return false;
        }
    }
    
    return true;
}

std::unique_ptr<PossibleMove> MoveFactory::get_all_moves(ChessColour colour) const {
    auto pm = std::make_unique<PossibleMove>();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (_board.get_piece({i, j}) && _board.get_piece({i, j}).value().colour == colour && _board.get_piece({i, j}).value().type != PieceType::King) {
                auto tmp = get_moves({i, j});
                for (auto& move : tmp->moves) {
                    pm->add_move(std::move(move));
                }
            }
        }
    }
    return pm;
}

void MoveFactory::_diagonal_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type) const {
    if (_board.get_piece(posn) == std::nullopt) return;

    auto insert_valid_posn = [this, posn, pm, move_type](const BoardPosn& cur_posn) {
        if (!cur_posn.on_board()) {
            return false;
        }
        else if (this->_board.get_piece(cur_posn) == std::nullopt) {
            std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value(), move_type}};
            pm->add_move(std::move(move));
            return true;
        }
        else if (this->_board.get_piece(cur_posn) && this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
            std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(cur_posn).value(), cur_posn, move_type}};
            pm->add_move(std::move(move));
            return false;
        }
        else {
            return false;
        }
    };

    BoardPosn tmp = posn;

    while (insert_valid_posn(tmp+=BoardPosn{1, 1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{-1, 1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{-1, -1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{1, -1}));
}

void MoveFactory::_linear_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type) const {
    if (_board.get_piece(posn) == std::nullopt) return;

    auto insert_valid_posn = [this, posn, pm, move_type](const BoardPosn& cur_posn) {
        if (!cur_posn.on_board()) {
            return false;
        }
        else if (this->_board.get_piece(cur_posn) == std::nullopt) {
            std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value(), move_type}};
            pm->add_move(std::move(move));
            return true;
        }
        else if (this->_board.get_piece(cur_posn) && this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
            std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(cur_posn).value(), cur_posn, move_type}};
            pm->add_move(std::move(move));
            return false;
        }
        else {
            return false;
        }
    };

    BoardPosn tmp = posn;

    while (insert_valid_posn(tmp+=BoardPosn{0, 1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{1, 0}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{0, -1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{-1, 0}));
}

std::unique_ptr<PossibleMove> MoveFactory::_rook_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    if (_status.white_can_castle_queenside && posn == BoardPosn{0, 0} && _board.get_piece(posn) == Piece::WhiteRook) {
        _linear_moves(pm.get(), posn, UniqueMove::UnableWhiteQueenSideCastling);
    }
    else if (_status.white_can_castle_kingside && posn == BoardPosn{7, 0} && _board.get_piece(posn) == Piece::WhiteRook) {
        _linear_moves(pm.get(), posn, UniqueMove::UnableWhiteKingSideCastling);
    }
    else if (_status.black_can_castle_queenside && posn == BoardPosn{0, 7} && _board.get_piece(posn) == Piece::BlackRook) {
        _linear_moves(pm.get(), posn, UniqueMove::UnableBlackQueenSideCastling);
    }
    else if (_status.black_can_castle_kingside && posn == BoardPosn{7, 7} && _board.get_piece(posn) == Piece::BlackRook) {
        _linear_moves(pm.get(), posn, UniqueMove::UnableBlackKingSideCastling);
    } else {
        _linear_moves(pm.get(), posn);
    }
    
    return pm;
}

std::unique_ptr<PossibleMove> MoveFactory::_bishop_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    _diagonal_moves(pm.get(), posn);
    return pm;
}

std::unique_ptr<PossibleMove> MoveFactory::_queen_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    _linear_moves(pm.get(), posn);
    _diagonal_moves(pm.get(), posn);
    return pm;
}

std::unique_ptr<PossibleMove> MoveFactory::_knight_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    if (!_board.get_piece(posn)) return pm;

    auto insert_valid_posn = [this, posn](PossibleMove* pm, const BoardPosn& cur_posn) {
        if (!cur_posn.on_board()) {
            return false;
        }
        else if (this->_board.get_piece(cur_posn) == std::nullopt) {
            std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value()}};
            pm->add_move(std::move(move));
            return true;
        }
        else if (this->_board.get_piece(cur_posn) && this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
            std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(cur_posn).value(), cur_posn}};
            pm->add_move(std::move(move));
            return false;
        }
        else {
            return false;
        }
    };

    BoardPosn tmp = posn;
    insert_valid_posn(pm.get(), tmp+BoardPosn{1, 2});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, 2});
    insert_valid_posn(pm.get(), tmp+BoardPosn{1, -2});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, -2});
    insert_valid_posn(pm.get(), tmp+BoardPosn{2, 1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{2, -1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-2, 1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-2, -1});

    return pm;
}

std::unique_ptr<PossibleMove> MoveFactory::_pawn_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    if (_board.get_piece(posn) == std::nullopt) return pm;
    
    // promotion case 
    if (on_nth_rank(_board.get_piece(posn).value().colour, posn, 7)) {
        auto insert_valid_posn = [this, posn](PossibleMove* pm, const BoardPosn& cur_posn, bool should_capture) {
            // std::cout << cur_posn.file << ", " << cur_posn.rank << std::endl;
            if (!cur_posn.on_board()) {
                return false;
            } // promote to Queen by default
            else if (!should_capture && !this->_board.get_piece(cur_posn) ) {
                std::unique_ptr<Move> move{new PromotionMove{posn, cur_posn, this->_board.get_piece(posn).value(), std::nullopt, Piece{PieceType::Queen, this->_board.get_piece(posn).value().colour}}};
                pm->add_move(std::move(move));
                return true;
            } // promote to Queen by default
            else if (should_capture && this->_board.get_piece(cur_posn) && this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
                std::unique_ptr<Move> move{new PromotionMove{posn, cur_posn, this->_board.get_piece(posn).value(), std::make_optional<Piece>(this->_board.get_piece(cur_posn).value()), Piece{PieceType::Queen, this->_board.get_piece(posn).value().colour}}};
                pm->add_move(std::move(move));
                return false;
            } 
            else {
                return false;
            }
        };
        insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true);
        insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{-1, 0}, true);
        insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1), false);
    }
    else {
        auto insert_valid_posn = [this, posn](PossibleMove* pm, const BoardPosn& cur_posn, bool should_capture, bool should_enpassant) {
            // std::cout << cur_posn.file << ", " << cur_posn.rank << std::endl;
            if (!cur_posn.on_board()) {
                return false;
            } // Basic Move
            else if (!should_capture && !this->_board.get_piece(cur_posn) ) {
                if (push_n_rank(_board.get_piece(posn).value().colour, posn, 2) == cur_posn) {
                    if (_board.get_piece(posn).value().colour == ChessColour::White) {
                        std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value(), UniqueMove::WhiteDoublePawnPush}};
                        pm->add_move(std::move(move));
                    } else {
                        std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value(), UniqueMove::BlackDoublePawnPush}};
                        pm->add_move(std::move(move));
                    }                   
                    return true;
                }
                std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value()}};
                pm->add_move(std::move(move));
                return true;
            } // enpassant
            else if (should_enpassant && should_capture && !this->_board.get_piece(cur_posn)) {
                std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(push_n_rank(_board.get_piece(posn).value().colour, cur_posn, -1)).value(), push_n_rank(_board.get_piece(posn).value().colour, cur_posn, -1)}};
                pm->add_move(std::move(move));
                return false;
            } // Capture but not enpassant
            else if (should_capture && !should_enpassant && this->_board.get_piece(cur_posn) && this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
                std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(cur_posn).value(), cur_posn}};
                pm->add_move(std::move(move));
                return false;
            } 
            else {
                return false;
            }
        };

        // double push and Basic Move
        if (on_nth_rank(_board.get_piece(posn).value().colour, posn, 2)) {
            int rank = 1;
            while (rank <= 2 && insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, rank), false, false)) {
                ++rank;
            }
        }
        else {
            insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1), false, false);
        }

        // Normal capture
        insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, false);
        insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{-1, 0}, true, false);
        
        BoardPosn tmp = posn;
        if (_status.cur_turn == ChessColour::White) {
            if (tmp+BoardPosn{1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, true);
            if (tmp+BoardPosn{-1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{-1, 0}, true, true);
        } else {
            if (tmp+BoardPosn{1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, true);
            if (tmp+BoardPosn{-1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{-1, 0}, true, true);
        }
    }

    return pm;
}

std::unique_ptr<PossibleMove> MoveFactory::_king_moves(const BoardPosn& posn) const {
    auto pm = std::make_unique<PossibleMove>();
    if (_board.get_piece(posn) == std::nullopt) return pm;
    BoardPosn tmp = posn;

    // castling
    if (_status.can_castle_queenside(_board.get_piece(posn).value().colour) &&
        _board.get_piece(tmp+BoardPosn{-1, 0}) == std::nullopt &&
        _board.get_piece(tmp+BoardPosn{-2, 0}) == std::nullopt &&
        _board.get_piece(tmp+BoardPosn{-3, 0}) == std::nullopt &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{0, 0}, _board.get_piece(posn).value()}) &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{-1, 0}, _board.get_piece(posn).value()}) &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{-2, 0}, _board.get_piece(posn).value()})) {
        if (!_status.can_castle_kingside(_board.get_piece(posn).value().colour)){
            std::unique_ptr<Move> move{new CastlingMove{posn, tmp+BoardPosn{-2, 0}, _board.get_piece(posn).value(), tmp+BoardPosn{-4, 0}, tmp+BoardPosn{-1, 0}, get_unable_queenside_castling(_board.get_piece(posn).value().colour)}};
            pm->add_move(std::move(move));
        } 
        else {
            std::unique_ptr<Move> move{new CastlingMove{posn, tmp+BoardPosn{-2, 0}, _board.get_piece(posn).value(), tmp+BoardPosn{-4, 0}, tmp+BoardPosn{-1, 0}, get_unable_all_castling(_board.get_piece(posn).value().colour)}};
            pm->add_move(std::move(move));
        }
    }
    
    if (_status.can_castle_kingside(_board.get_piece(posn).value().colour) &&
        _board.get_piece(tmp+BoardPosn{1, 0}) == std::nullopt &&
        _board.get_piece(tmp+BoardPosn{2, 0}) == std::nullopt &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{0, 0}, _board.get_piece(posn).value()}) &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{1, 0}, _board.get_piece(posn).value()}) &&
        is_move_safe(BasicMove{posn, tmp+BoardPosn{2, 0}, _board.get_piece(posn).value()})) {
        if (!_status.can_castle_queenside(_board.get_piece(posn).value().colour)){
            std::unique_ptr<Move> move{new CastlingMove{posn, tmp+BoardPosn{2, 0}, _board.get_piece(posn).value(), tmp+BoardPosn{3, 0}, tmp+BoardPosn{1, 0}, get_unable_kingside_castling(_board.get_piece(posn).value().colour)}};
            pm->add_move(std::move(move));
        } 
        else {
            std::unique_ptr<Move> move{new CastlingMove{posn, tmp+BoardPosn{2, 0}, _board.get_piece(posn).value(), tmp+BoardPosn{3, 0}, tmp+BoardPosn{1, 0}, get_unable_all_castling(_board.get_piece(posn).value().colour)}};
            pm->add_move(std::move(move));
        }
    }

    // normal moves
    auto insert_valid_posn = [this, posn](PossibleMove* pm, const BoardPosn& cur_posn) {
        if (!cur_posn.on_board()) {
            return;
        }
        else if (this->_board.get_piece(cur_posn) == std::nullopt) {
            std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value(), _status.king_unable_castling(_board.get_piece(posn).value().colour)}};
            if (is_move_safe(*move)) {
                pm->add_move(std::move(move));
            }
            return;
        }
        else if (this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
            std::unique_ptr<Move> move{new CaptureMove{posn, cur_posn, this->_board.get_piece(posn).value(), this->_board.get_piece(cur_posn).value(), cur_posn, _status.king_unable_castling(_board.get_piece(posn).value().colour)}};
            if (is_move_safe(*move)) {
                pm->add_move(std::move(move));
            }
            return;
        }
        else {
            return;
        }
    };
    
    insert_valid_posn(pm.get(), tmp+BoardPosn{1, 0});
    insert_valid_posn(pm.get(), tmp+BoardPosn{1, 1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{0, 1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, 1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, 0});
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, -1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{0, -1});
    insert_valid_posn(pm.get(), tmp+BoardPosn{1, -1});
    
    return pm;
}


