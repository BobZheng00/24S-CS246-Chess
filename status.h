#pragma once

#include "piece.h"
#include "move.h"
#include <memory>

class MoveHistory;
struct BoardPosn; 

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
    Status();
    void clear();
    void print_status() const;
    ChessColour next_turn();

    Result result;
    std::unique_ptr<MoveHistory> move_history;
    ChessColour cur_turn;
    bool white_can_castle_kingside = true;
    bool white_can_castle_queenside = true;
    bool black_can_castle_kingside = true;
    bool black_can_castle_queenside = true;
    std::unique_ptr<BoardPosn> white_last_double_pawn_push;
    std::unique_ptr<BoardPosn> black_last_double_pawn_push;
};
