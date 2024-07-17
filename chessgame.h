#include "board.h"
#include "status.h"
#include "movefactory.h"
#include <memory>
#include <optional>

class ChessGame {
    ChessGame();

    void regular_init();
    void reset(); 

    // for setup stage
    void set_turn(ChessColour colour);
    void set_piece(const BoardPosn& posn, const Piece& piece);
    bool is_valid_setup() const;

    void set_status(Result result);

    // ADD MORE ACCESSOR FUNCTIONS AS NEEDED
    
    // move execution and undoing must be called by chessgame
    bool execute_move(const BoardPosn& from, const BoardPosn& to, std::optional<PieceType> opt_promotion = std::nullopt); // for computer and player to call
    void undo_move(); // call move_history.undo_last_move
private:
    Status _status;
    Board _board;
    MoveFactory _move_factory;
    friend class ComputerPlayer;
};
