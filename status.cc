#include "status.h"

void Status::clear() {
    result = Result::Unstarted;
    move_history.reset();
    cur_turn = ChessColour::White;
    white_can_castle_kingside = true;
    white_can_castle_queenside = true;
    black_can_castle_kingside = true;
    black_can_castle_queenside = true;
    white_last_double_pawn_push = BoardPosn::Invalid;
    black_last_double_pawn_push = BoardPosn::Invalid;
}

ChessColour Status::next_turn() {
    return cur_turn = (to_underlying(cur_turn)) ? ChessColour::Black : ChessColour::White;
}
