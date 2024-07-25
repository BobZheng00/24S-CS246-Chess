#pragma once

#include "piece.h"
#include <vector>
#include <stack>
#include <memory>
#include <optional>
#include <iostream>
#include <string>

class Board;
struct GameStatus;

enum class UniqueMove {
    WhiteDoublePawnPush,
    BlackDoublePawnPush,
    UnableWhiteQueenSideCastling,
    UnableBlackQueenSideCastling,
    UnableWhiteKingSideCastling,
    UnableBlackKingSideCastling,
    UnableAllWhiteCastling,
    UnableAllBlackCastling,
    Normal
};

UniqueMove get_unable_kingside_castling(ChessColour colour);
UniqueMove get_unable_queenside_castling(ChessColour colour);
UniqueMove get_unable_all_castling(ChessColour colour);

struct BoardPosn {
    int file;
    int rank;

    BoardPosn(int file, int rank) : file(file), rank(rank) {}
    bool operator==(const BoardPosn& other) const;
    BoardPosn& operator+=(const BoardPosn& other);
    BoardPosn operator+(const BoardPosn& other);
    bool on_board() const;
    std::string to_string() const;

    static const BoardPosn Invalid;
};

struct RawMove {
    BoardPosn from;
    BoardPosn to;
};

struct Move: public RawMove {
    Piece moved_piece;
    UniqueMove move_type;
    Move(const BoardPosn& from, const BoardPosn& to, const Piece& p, UniqueMove move_type = UniqueMove::Normal): RawMove{from, to}, moved_piece{p}, move_type{move_type} {}
    virtual ~Move() = default;
    virtual std::unique_ptr<std::vector<BoardPosn>> execute(Board& board, GameStatus& status) const = 0;
    virtual std::unique_ptr<std::vector<BoardPosn>> undo(Board& board, GameStatus& status) const = 0;
};

struct BasicMove: public Move {
    BasicMove(const BoardPosn& from, const BoardPosn& to, const Piece& p, UniqueMove move_type = UniqueMove::Normal): Move{from, to, p, move_type} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board, GameStatus& status) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board, GameStatus& status) const override;
};

struct CaptureMove: public Move {
    Piece captured_piece;
    BoardPosn captured_posn;

    CaptureMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const Piece& captured, const BoardPosn& captured_posn, UniqueMove move_type = UniqueMove::Normal):
        Move{from, to, moved, move_type}, captured_piece{captured}, captured_posn{captured_posn} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board, GameStatus& status) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board, GameStatus& status) const override;
};

struct PromotionMove: public Move {
    std::optional<Piece> captured_piece;
    Piece promoted_piece;
    PromotionMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const std::optional<Piece>& captured, const Piece& promoted, UniqueMove move_type = UniqueMove::Normal):
        Move{from, to, moved, move_type}, captured_piece{captured}, promoted_piece{promoted} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board, GameStatus& status) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board, GameStatus& status) const override;
};

struct CastlingMove: public Move {
    BoardPosn rook_from;
    BoardPosn rook_to;
    CastlingMove(const BoardPosn& from, const BoardPosn& to, const Piece& moved, const BoardPosn& rook_from, const BoardPosn& rook_to, UniqueMove king_move_type = UniqueMove::Normal):
        Move{from, to, moved, king_move_type}, rook_from{rook_from}, rook_to{rook_to} {}
    std::unique_ptr<std::vector<BoardPosn>> execute(Board& board, GameStatus& status) const override;
    std::unique_ptr<std::vector<BoardPosn>> undo(Board& board, GameStatus& status) const override;
};

class MoveHistory {
public:
    void push_move(std::unique_ptr<Move> move);
    void reset();
    void undo_last_move(Board& board, GameStatus& status);

private:
    std::stack<std::unique_ptr<Move>> moves;
};

class PossibleMove {
public:
    bool is_possible_move(BoardPosn& from, BoardPosn& to) const;
    void add_move(std::unique_ptr<Move> move);
// private:
    std::vector<std::unique_ptr<Move>> moves;
};

