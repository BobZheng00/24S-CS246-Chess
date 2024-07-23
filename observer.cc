#include "observer.h"

TextDisplay::TextDisplay() {
    board->attach(this);
}

void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const {
    Board::print_board();
}

GraphicDisplay::GraphicDisplay() {
    board->attach(this);
    _window = Xwindow(4000, 4000);
}

void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const {
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
        }
    }
    
}
