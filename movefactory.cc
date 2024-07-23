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
        default:
            return std::make_unique<PossibleMove>();
    }
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
        else if (this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
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
        else if (this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
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
    if (_board.get_piece(posn) == std::nullopt) return pm;

    auto insert_valid_posn = [this, posn](PossibleMove* pm, const BoardPosn& cur_posn) {
        if (!cur_posn.on_board()) {
            return false;
        }
        else if (this->_board.get_piece(cur_posn) == std::nullopt) {
            std::unique_ptr<Move> move{new BasicMove{posn, cur_posn, this->_board.get_piece(posn).value()}};
            pm->add_move(std::move(move));
            return true;
        }
        else if (this->_board.get_piece(cur_posn).value().colour != this->_board.get_piece(posn).value().colour) {
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
    insert_valid_posn(pm.get(), tmp+BoardPosn{-1, -2});
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
            std::cout << cur_posn.file << ", " << cur_posn.rank << std::endl;
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
            std::cout << cur_posn.file << ", " << cur_posn.rank << std::endl;
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
            if (tmp+BoardPosn{-1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, true);
        } else {
            if (tmp+BoardPosn{1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, true);
            if (tmp+BoardPosn{-1, 0} == *(_status.black_last_double_pawn_push)) insert_valid_posn(pm.get(), push_n_rank(_board.get_piece(posn).value().colour, posn, 1)+BoardPosn{1, 0}, true, true);
        }
    }

    return pm;
}

// std::unique_ptr<PossibleMove> MoveFactory::_king_moves(const BoardPosn& posn) const {
//     auto pm = std::make_unique<PossibleMove>();
//     if (_status.white_can_castle_queenside && posn == BoardPosn{4, 0} && _board.get_piece(posn) == Piece::WhiteKing) {

//     }
//     else if (_status.white_can_castle_kingside && posn == BoardPosn{4, 0} && _board.get_piece(posn) == Piece::WhiteKing) {
//     }
//     else if (_status.black_can_castle_queenside && posn == BoardPosn{4, 7} && _board.get_piece(posn) == Piece::BlackKing) {
//     }
//     else if (_status.black_can_castle_kingside && posn == BoardPosn{4, 7} && _board.get_piece(posn) == Piece::BlackKing) {
//     } else {
//     }
    
//     return pm;
// }
