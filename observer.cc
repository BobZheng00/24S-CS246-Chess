#include <iostream>
#include "observer.h"

TextDisplay::TextDisplay() {
    _p_board_subj->attach(this);
}

void TextDisplay::update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const {
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank+1 << ' ';
        for (int file = 0; file < 8; ++file) {
            std::optional<Piece> piece = Board::get_piece(file, rank);
            if (piece) {
                if (piece == Piece::WhiteKing) {
                    std::cout << 'K ';
                } else if (piece == Piece::WhiteQueen) {
                    std::cout << 'Q ';
                } else if (piece == Piece::WhiteRook) {
                    std::cout << 'R ';
                } else if (piece == Piece::WhiteBishop) {
                    std::cout << 'B ';
                } else if (piece == Piece::WhiteKnight) {
                    std::cout << 'N ';
                } else if (piece == Piece::WhitePawn) {
                    std::cout << 'P ';
                } else if (piece == Piece::BlackKing) {
                    std::cout << 'k ';
                } else if (piece == Piece::BlackQueen) {
                    std::cout << 'q ';
                } else if (piece == Piece::BlackRook) {
                    std::cout << 'r ';
                } else if (piece == Piece::BlackBishop) {
                    std::cout << 'b ';
                } else if (piece == Piece::BlackKnight) {
                    std::cout << 'n ';
                } else if (piece == Piece::BlackPawn) {
                    std::cout << 'p ';
                }
            } 
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl << std::endl;
}

GraphicDisplay::GraphicDisplay() : _window{Xwindow(4000, 4000)} {
    _p_board_subj->attach(this);
}

void GraphicDisplay::update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const {
    const int squareSize = 500;
    const int boardSize = 8;
    _window.fillRectangle(0, 0, squareSize * boardSize, squareSize * boardSize, Xwindow::White);
    for (int rank = 0; rank < boardSize; ++rank) {
        for (int file = 0; file < boardSize; ++file) {
            bool isWhiteSquare = ((rank + file) % 2 == 0);
            int x = file * squareSize;
            int y = rank * squareSize;
            if (isWhiteSquare) {
                _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::White);
            } else {
                _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
            }
            std::optional<Piece> piece = get_piece(file, rank);

            piece.value()
        }
    }
}
