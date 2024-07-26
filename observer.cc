#include <iostream>
#include "status.h"
#include "observer.h"
#include "piece.h"
#include "move.h"
#include "board.h"

TextDisplay::TextDisplay(BoardSubject* p_board_subj): _p_board_subj{p_board_subj} {
    _p_board_subj->attach(this);
}

TextDisplay::~TextDisplay() {
    _p_board_subj->detach(this);
}

void TextDisplay::update_board(std::shared_ptr<std::vector<BoardPosn>> b, Result r) const {
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << ' ';
        for (int file = 0; file < 8; ++file) {
            if (_p_board_subj->get_piece(file, rank)) {
                std::cout << _p_board_subj->get_piece(file, rank).value().to_char() << ' ';
            } 
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl << std::endl;
}


GraphicDisplay::GraphicDisplay(BoardSubject* p_board_subj): _window{Xwindow(400, 400)}, _p_board_subj{p_board_subj} {
    _p_board_subj->attach(this);
    const int squareSize = 50;
    const int boardSize = 8;
    _window.fillRectangle(0, 0, squareSize * boardSize, squareSize * boardSize, Xwindow::White);
    for (int rank = 0; rank < boardSize; ++rank) {
        for (int file = 0; file < boardSize; ++file) {
            bool isWhiteSquare = ((rank + file) % 2 != 0);
            int x = file * squareSize;
            int y = (7-rank) * squareSize;
            if (isWhiteSquare) {
                _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::White);
            } else {
                _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
            }
            int textX = x + squareSize / 2;
            int textY = y + squareSize / 2;
            if (_p_board_subj->get_piece(file, rank)) {
                std::string pieceChar = std::string(1, _p_board_subj->get_piece(file, rank).value().to_char());
                _window.drawString(textX, textY, pieceChar, Xwindow::Red);
            }
        }
    }
}

GraphicDisplay::~GraphicDisplay() {
    _p_board_subj->detach(this);
}

void GraphicDisplay::update_board(std::shared_ptr<std::vector<BoardPosn>> b, Result r) const {
    const int squareSize = 50;
    for (auto& posn : *b) {
        int file = posn.file;
        int rank = posn.rank;
        bool isWhiteSquare = ((rank + file) % 2 != 0);
        int x = file * squareSize;
        int y = (7-rank) * squareSize;
        if (isWhiteSquare) {
            _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::White);
        } else {
            _window.fillRectangle(x, y, squareSize, squareSize, Xwindow::Black);
        }
        if (_p_board_subj->get_piece(file, rank)) {
            std::string pieceChar = std::string(1, _p_board_subj->get_piece(file, rank).value().to_char());
            int textX = x + squareSize / 2;
            int textY = y + squareSize / 2;
            _window.drawString(textX, textY, pieceChar, Xwindow::Red);
        }
    }
}
