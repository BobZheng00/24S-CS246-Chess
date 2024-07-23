#pragma once

#include "move.h"
#include "board.h"
#include "status.h"
#include <vector>

class MoveFactory {
public:
    MoveFactory(const Board& board, const Status& status): _board{board}, _status{status} {}
    std::unique_ptr<PossibleMove> get_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> get_all_moves(ChessColour colour) const;
    
    bool is_move_safe(const Move& move) const;
    bool is_capture_valuable(const CaptureMove& move) const;
    bool is_mated() const;
    bool is_drawn() const;
// private:
    void _diagonal_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type = UniqueMove::None) const;
    void _linear_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type = UniqueMove::None) const;
    // We assert corresponding piece exists on the position for the following move generation
    std::unique_ptr<PossibleMove> _rook_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _bishop_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _queen_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _knight_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _pawn_moves(const BoardPosn& posn) const;
    const Board& _board;
    const Status& _status;
};
