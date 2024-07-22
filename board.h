#pragma once

#include "piece.h"
#include "move.h"
#include "observer.h"
#include <iostream>
#include <array>
#include <optional>
#include <vector>
#include <memory>

class BoardObserver;

class BoardSubject {
    std::vector<std::shared_ptr<BoardObserver>> observers;
public:
    void attach(std::shared_ptr<BoardObserver>);
    void detach(std::shared_ptr<BoardObserver>);
    void notify_observers(std::unique_ptr<std::vector<BoardPosn>>, Result) const;
    virtual ~BoardSubject() = default;
};


class Board: public BoardSubject {
public:
    Board() = default;

    void standard_init();
    void clear();
    void set_standard_position();
    BoardPosn get_king_posn(ChessColour colour) const;

    std::optional<Piece> get_piece(int row, int col) const;
    std::optional<Piece> get_piece(const BoardPosn& posn) const;
    void set_piece(int row, int col, const Piece& piece);
    void set_piece(const BoardPosn& posn, const Piece& piece);
    void reset_piece(int row, int col);
    void reset_piece(const BoardPosn& posn);
    void print_board(); // just for testing

private:
    // std::array<std::optional<Piece>, 8> is a column, denoted by letter a-h
    // for column a, std::array<std::optional<Piece>, 8> contains BoardPosn a1, a2, ..., a8
    
    std::array<std::array<std::optional<Piece>, 8>, 8> _board;
};

template <PieceType Type>
inline constexpr std::array<std::optional<Piece>, 8> board_file {
    Piece(Type, ChessColour::White), 
    Piece(PieceType::Pawn, ChessColour::White), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceType::Pawn, ChessColour::Black),
    Piece(Type, ChessColour::Black)
};

