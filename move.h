#pragma once

#include "piece.h"
#include "board.h"
#include <vector>
#include <memory>
#include <optional>

class Board;

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
    Piece moved_piece;

    virtual ~Move() = default;
    virtual std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const = 0;
    virtual std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const = 0;
};

struct BasicMove: public Move {
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct CaptureMove: public Move {
    Piece captured_piece;
    BoardPosn captured_posn;

    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct PromotionMove: public Move {
    std::optional<Piece> captured_piece;
    Piece promoted_piece;

    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct CastlingMove: public Move {
    BoardPosn rook_from;
    BoardPosn rook_to;

    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
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

