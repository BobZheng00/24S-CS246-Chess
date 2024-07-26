#pragma once

#include "move.h"
#include "board.h"
#include "status.h"
#include <vector>

bool on_nth_rank(ChessColour colour, const BoardPosn& posn, int rank);

class MoveFactory {
public:
    MoveFactory(const Board& board, const GameStatus& status): _board{board}, _status{status} {}
    std::unique_ptr<PossibleMove> get_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> get_all_moves(ChessColour colour) const; // does not include moves of king
    
    bool is_move_safe(const Move& move) const;
    bool is_capture_valuable(const CaptureMove& move) const;
    bool is_move_evading_capture(const Move& move) const;
    bool will_move_check_opponent(const Move& move) const;
    bool will_move_result_check(const Move& move) const;
    bool will_attack_multi_pieces_next(const Move& move) const;
    bool is_in_check(ChessColour colour) const;
    bool is_checkmated() const;
    bool is_stalemated() const;
private:
    void _diagonal_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type = UniqueMove::Normal) const;
    void _linear_moves(PossibleMove* pm, const BoardPosn& posn, UniqueMove move_type = UniqueMove::Normal) const;
    // We assert corresponding piece exists on the position for the following move generation
    std::unique_ptr<PossibleMove> _rook_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _bishop_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _queen_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _knight_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _pawn_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> _king_moves(const BoardPosn& posn) const;
    const Board& _board;
    const GameStatus& _status;
};
