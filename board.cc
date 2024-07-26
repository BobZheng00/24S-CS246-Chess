#include "move.h"
#include "board.h"
#include "status.h"
#include "observer.h"

void BoardSubject::attach(BoardObserver* o) {
    observers.emplace_back(o);
}

void BoardSubject::detach(BoardObserver* o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

void BoardSubject::notify_observers(std::unique_ptr<std::vector<BoardPosn>> posns, Result res) const {
    std::shared_ptr<std::vector<BoardPosn>> posns_shared = std::move(posns);
    for (auto obs: observers) {
        obs->update_board(posns_shared, res);
    }
}

void Board::standard_init() {
    _board = {
        board_file<PieceType::Rook>,
        board_file<PieceType::Knight>,
        board_file<PieceType::Bishop>,
        board_file<PieceType::Queen>,
        board_file<PieceType::King>,
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
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            if (_board[file][rank] && _board[file][rank].value() == Piece(PieceType::King, colour)) {
                return BoardPosn(file, rank);
            }
        }
    }
    return BoardPosn::Invalid;
}

std::optional<Piece> Board::get_piece(int file, int rank) const {
    return _board[file][rank];
}

std::optional<Piece> Board::get_piece(const BoardPosn& posn) const {
    return _board[posn.file][posn.rank];
}

void Board::set_piece(int file, int rank, const Piece& piece) {
    _board[file][rank] = piece;
}

void Board::set_piece(const BoardPosn& posn, const Piece& piece) {
    _board[posn.file][posn.rank] = piece;
}

void Board::reset_piece(int file, int rank) {
    _board[file][rank].reset();
}

void Board::reset_piece(const BoardPosn& posn) {
    _board[posn.file][posn.rank].reset();
}

void Board::print_board() {
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank+1 << ' ';
        for (int file = 0; file < 8; ++file) {
            if (_board[file][rank]) {
                std::cout << _board[file][rank].value().to_char() << ' ';
            } 
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl << std::endl;
}
