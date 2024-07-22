#include "board.h"

void BoardSubject::attach(std::shared_ptr<BoardObserver> o) {
    observers.emplace_back(o);
}

void BoardSubject::detach(std::shared_ptr<BoardObserver> o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

void BoardSubject::notify_observers(std::unique_ptr<std::vector<BoardPosn>> posns, Result res) const {
    for (auto obs: observers) {
        obs->update_board(std::move(posns), res);
    }
}

void Board::standard_init() {
    _board = {
        board_file<PieceType::Rook>,
        board_file<PieceType::Knight>,
        board_file<PieceType::Bishop>,
        board_file<PieceType::King>,
        board_file<PieceType::Queen>,
        board_file<PieceType::Bishop>,
        board_file<PieceType::Knight>,
        board_file<PieceType::Rook>
    };
}

void Board::clear() {
    for (auto& file: _board) {
        for (auto& tile: file) {
            tile.reset();
        }
    }
}

BoardPosn Board::get_king_posn(ChessColour colour) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (_board[row][col] && _board[row][col].value() == Piece(PieceType::King, colour)) {
                return BoardPosn(row, col);
            }
        }
    }
    return BoardPosn::Invalid;
}

std::optional<Piece> Board::get_piece(int row, int col) const {
    return _board[row][col];
}

std::optional<Piece> Board::get_piece(const BoardPosn& posn) const {
    return _board[posn.row][posn.col];
}

void Board::set_piece(int row, int col, const Piece& piece) {
    _board[row][col] = piece;
}

void Board::set_piece(const BoardPosn& posn, const Piece& piece) {
    _board[posn.row][posn.col] = piece;
}

void Board::reset_piece(int row, int col) {
    _board[row][col].reset();
}

void Board::reset_piece(const BoardPosn& posn) {
    _board[posn.row][posn.col].reset();
}

void Board::print_board() {
    for (int col = 7; col >= 0; --col) {
        std::cout << col+1 << ' ';
        for (int row = 0; row < 8; ++row) {
            if (_board[row][col]) {
                std::cout << _board[row][col].value().to_char() << ' ';
            } 
            else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}
