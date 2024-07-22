#pragma once

#include "move.h"
#include "piece.h"
#include <memory>

enum class Result {
    Unstarted,
    Setup,
    Unfinished,
    WhiteInCheck,
    BlackInCheck,
    WhiteWin,
    BlackWin,
    Draw
};

struct Status {
    void clear();
    ChessColour next_turn();

    Result result = Result::Unstarted;
    MoveHistory move_history;
    ChessColour cur_turn = ChessColour::White;
    bool white_can_castle_kingside = true;
    bool white_can_castle_queenside = true;
    bool black_can_castle_kingside = true;
    bool black_can_castle_queenside = true;
    BoardPosn white_last_double_pawn_push = BoardPosn::Invalid;
    BoardPosn black_last_double_pawn_push = BoardPosn::Invalid;
};
