#include "piece.h"
#include "board.h"
#include <vector>
#include <memory>
#include <optional>

struct BoardPosn {
    int row;
    int col;

    BoardPosn(int row, int col) : row(row), col(col) {}
    bool operator==(const BoardPosn& other) const;

    static const BoardPosn Invalid;
};

struct RawMove {
    BoardPosn from;
    BoardPosn to;
};

struct Move: public RawMove {
    PieceType movedPieceType;

    virtual ~Move() = default;
    virtual void execute(Board& board) const = 0;
    virtual void undo(Board& board) const = 0;
};

struct BasicMove : public Move {
    void execute(Board& board) const override;
    void undo(Board& board) const override;
};

struct CaptureMove : public Move {
    PieceType capturedPieceType;

    void execute(Board& board) const override;
    void undo(Board& board) const override;
};

struct PromotionMove : public Move {
    std::optional<PieceType> capturedPieceType;
    PieceType promotedPieceType;

    void execute(Board& board) const override;
    void undo(Board& board) const override;
};

struct CastlingMove : public Move {
    BoardPosn rookFrom;
    BoardPosn rookTo;

    void execute(Board& board) const override;
    void undo(Board& board) const override;
};

class MoveHistory {
public:
    void add_move(std::unique_ptr<Move> move);

    void undo_last_move(Board& board);

private:
    std::vector<std::unique_ptr<Move>> moves;

};

class PossibleMove {
public:
    bool is_possible_move(const BoardPosn& posn) const;
private:
    std::vector<std::unique_ptr<Move>> moves;
};

