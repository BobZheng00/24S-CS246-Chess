#pragma once

#include "board.h"
#include "status.h"
#include "movefactory.h"
#include "player.h"
#include <memory>
#include <optional>

class ChessGame {
public:
    ChessGame();

    void regular_init();
    void setup_init();
    void reset(); 

    // for setup stage
    void set_turn(ChessColour colour);
    void set_piece(const BoardPosn& posn, const Piece& piece);
    void reset_piece(const BoardPosn& posn);
    bool is_valid_setup() const;

    void set_status(Result result);
    Result get_status() const;

    BoardSubject* get_board_for_observers();

    // ADD MORE ACCESSOR FUNCTIONS AS NEEDED
    
    // move execution and undoing must be called by chessgame
    bool execute_move(const BoardPosn& from, const BoardPosn& to, std::optional<PieceType> opt_promotion = std::nullopt); // for computer and player to call
    void undo_move(); // call move_history.undo_last_move
// private:
    GameStatus _status; // consider initialize to Unstarted status
    Board _board;
    MoveFactory _move_factory;
    friend class ComputerLv1;
    friend class ComputerLv2;
    friend class ComputerLv3;
    friend class ComputerLv4;
};
