#include "movefactory.h"

bool on_nth_rank(ChessColour colour, const BoardPosn& posn, int rank) {
    if (to_underlying(colour)) {
        return (posn.rank+1 == rank);
    }
    else {
        return (posn.rank == 8-rank);
    }
}

int push_n_rank(ChessColour colour, const BoardPosn& posn, int rank) {
    if (to_underlying(colour)) {
        return posn.rank+rank;
    }
    else {
        return posn.rank-rank;
    }
}

void MoveFactory::_diagonal_moves(PossibleMove* pm, const BoardPosn& posn) const {
    if (_board.get_piece(posn) == std::nullopt) return;

    auto insert_valid_posn = [this, posn, pm](const BoardPosn& cur_posn) {
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

    while (insert_valid_posn(tmp+=BoardPosn{1, 1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{-1, 1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{-1, -1}));
    tmp = posn;
    while (insert_valid_posn(tmp+=BoardPosn{1, -1}));
}

void MoveFactory::_linear_moves(PossibleMove* pm, const BoardPosn& posn) const {
    if (_board.get_piece(posn) == std::nullopt) return;

    auto insert_valid_posn = [this, posn, pm](const BoardPosn& cur_posn) {
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
    _linear_moves(pm.get(), posn);
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
        insert_valid_posn(pm.get(), BoardPosn{posn.file, push_n_rank(_board.get_piece(posn).value().colour, posn, 1)}+BoardPosn{1, 0}, true);
        insert_valid_posn(pm.get(), BoardPosn{posn.file, push_n_rank(_board.get_piece(posn).value().colour, posn, 1)}+BoardPosn{-1, 0}, true);
        insert_valid_posn(pm.get(), BoardPosn{posn.file, push_n_rank(_board.get_piece(posn).value().colour, posn, 1)}, false);
    }
    else {
        
    }

    return pm;
}
