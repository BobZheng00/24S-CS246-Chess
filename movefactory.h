#include "move.h"
#include "board.h"
#include "status.h"
#include <vector>

class MoveFactory {
public:
    MoveFactory(const Board& board, const Status& status);
    std::unique_ptr<PossibleMove> get_moves(const BoardPosn& posn) const;
    std::unique_ptr<PossibleMove> get_all_moves(ChessColour colour) const;
    
    bool is_move_safe(const Move& move) const; // usually be called 
    bool is_capture_valuable(const CaptureMove& move) const;
    bool is_mated() const;
    bool is_drawn() const;
private:
    const Board& _board;
    const Status& _status;
};
