#pragma once

#include "piece.h"
#include "board.h"
#include <vector>
#include <stack>
#include <memory>
#include <optional>

class Board;

struct BoardPosn {
    int file;
    int rank;

    BoardPosn(int file, int rank) : file(file), rank(rank) {}
    bool operator==(const BoardPosn& other) const;
    BoardPosn& operator+=(const BoardPosn& other);
    BoardPosn operator+(const BoardPosn& other);
    bool on_board() const;

    static const BoardPosn Invalid;
};

struct RawMove {
    BoardPosn from;
    BoardPosn to;
};

struct Move: public RawMove {
    Piece moved_piece;

    Move(const BoardPosn& from, const BoardPosn& to, const Piece& p): RawMove{from, to}, moved_piece{p} {}
    virtual ~Move() = default;
    virtual std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const = 0;
    virtual std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const = 0;
};

struct BasicMove: public Move {
    BasicMove(const BoardPosn& from, const BoardPosn& to, const Piece& p): Move{from, to, p} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct CaptureMove: public Move {
    Piece captured_piece;
    BoardPosn captured_posn;

    CaptureMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const Piece& captured, const BoardPosn& captured_posn):
        Move{from, to, moved}, captured_piece{captured}, captured_posn{captured_posn} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct PromotionMove: public Move {
    std::optional<Piece> captured_piece;
    Piece promoted_piece;
    PromotionMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const std::optional<Piece>& captured, const Piece& promoted):
        Move{from, to, moved}, captured_piece{captured}, promoted_piece{promoted} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

struct CastlingMove: public Move {
    BoardPosn rook_from;
    BoardPosn rook_to;
    CastlingMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const BoardPosn& rook_from, const BoardPosn& rook_to):
        Move{from, to, moved}, rook_from{rook_from}, rook_to{rook_to} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board) const override;
};

class MoveHistory {
public:
    void push_move(std::unique_ptr<Move> move);
    void reset();
    std::unique_ptr<Move> pop_last_move();

private:
    std::stack<std::unique_ptr<Move>> moves;

};

class PossibleMove {
public:
    bool is_possible_move(BoardPosn from, BoardPosn to) const;
    void add_move(std::unique_ptr<Move> move);
// private:
    std::vector<std::unique_ptr<Move>> moves;
};

