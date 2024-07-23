#include "move.h"
#include "board.h"
#include "status.h"
#include "observer.h"

Status::Status() {
    clear();
}

void Status::clear() {
    result = Result::Unstarted;
    move_history = std::make_unique<MoveHistory>();
    move_history->reset();
    cur_turn = ChessColour::White;
    white_can_castle_kingside = true;
    white_can_castle_queenside = true;
    black_can_castle_kingside = true;
    black_can_castle_queenside = true;
    white_last_double_pawn_push = std::make_unique<BoardPosn>(-1, -1);
    black_last_double_pawn_push = std::make_unique<BoardPosn>(-1, -1);
}

void Status::print_status() const {
    switch (result) {
    case Result::Unstarted:
        std::cout << "Game has not started yet." << std::endl;
        break;
    case Result::Setup:
        std::cout << "Game is in setup mode." << std::endl;
        break;
    case Result::Unfinished:
        std::cout << "Game is in progress." << std::endl;
        break;
    case Result::WhiteInCheck:
        std::cout << "White is in check." << std::endl;
        break;
    case Result::BlackInCheck:
        std::cout << "Black is in check." << std::endl;
        break;
    case Result::WhiteWin:
        std::cout << "White wins." << std::endl;
        break;
    case Result::BlackWin:
        std::cout << "Black wins." << std::endl;
        break;
    case Result::Draw:
        std::cout << "Game is a draw." << std::endl;
        break;
    }

    std::cout << "Next turn: " << (cur_turn == ChessColour::White ? "White" : "Black") << std::endl;
    std::cout << "White can castle kingside: " << (white_can_castle_kingside ? "Yes" : "No") << std::endl;
    std::cout << "White can castle queenside: " << (white_can_castle_queenside ? "Yes" : "No") << std::endl;
    std::cout << "Black can castle kingside: " << (black_can_castle_kingside ? "Yes" : "No") << std::endl;
    std::cout << "Black can castle queenside: " << (black_can_castle_queenside ? "Yes" : "No") << std::endl;
    std::cout << "White last double pawn push: " << (*white_last_double_pawn_push == BoardPosn::Invalid ? "None" : white_last_double_pawn_push->to_string()) << std::endl;
    std::cout << "Black last double pawn push: " << (*black_last_double_pawn_push == BoardPosn::Invalid ? "None" : black_last_double_pawn_push->to_string()) << std::endl;
}

ChessColour Status::next_turn() {
    return cur_turn = (to_underlying(cur_turn)) ? ChessColour::Black : ChessColour::White;
}
