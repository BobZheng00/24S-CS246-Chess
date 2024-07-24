#pragma once

#include "piece.h"
#include <iostream>
#include <array>
#include <optional>
#include <vector>
#include <memory>

class BoardObserver;
struct BoardPosn;
enum class Result;

class BoardSubject {
    std::vector<BoardObserver*> observers;
public:
    void attach(BoardObserver*);
    void detach(BoardObserver*);
    virtual std::optional<Piece> get_piece(int file, int rank) const = 0;
    virtual std::optional<Piece> get_piece(const BoardPosn& posn) const = 0;
    void notify_observers(std::unique_ptr<std::vector<BoardPosn>>, Result) const;
    virtual ~BoardSubject() = default;
};


class Board: public BoardSubject {
public:
    Board() = default;

    void standard_init();
    void clear();
    BoardPosn get_king_posn(ChessColour colour) const;

    virtual std::optional<Piece> get_piece(int file, int rank) const override;
    virtual std::optional<Piece> get_piece(const BoardPosn& posn) const override;
    void set_piece(int file, int rank, const Piece& piece);
    void set_piece(const BoardPosn& posn, const Piece& piece);
    void reset_piece(int file, int rank);
    void reset_piece(const BoardPosn& posn);
    void print_board(); // just for testing

private:
    // std::array<std::optional<Piece>, 8> is a file (column), denoted by letter a-h
    // for file a, std::array<std::optional<Piece>, 8> contains BoardPosn a1, a2, ..., a8
    
    std::array<std::array<std::optional<Piece>, 8>, 8> _board;
};

template <PieceType Type>
inline constexpr std::array<std::optional<Piece>, 8> board_file {
    Piece(Type, ChessColour::White), 
    Piece(PieceType::Pawn, ChessColour::White), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(PieceType::Pawn, ChessColour::Black),
    Piece(Type, ChessColour::Black)
};

