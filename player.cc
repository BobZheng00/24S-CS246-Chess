#include "player.h"
#include "movefactory.h"
#include "chessgame.h"
#include "move.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

RawMove HumanPlayer::get_move() const {
    std::string from, to;
    std::cout << "Enter move: ";
    std::cin >> from >> to;
    return { BoardPosn::Invalid, BoardPosn::Invalid };
}

ComputerPlayer::ComputerPlayer(ChessGame* game) : game(game) {}

ComputerLv1::ComputerLv1(ChessGame* game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv1::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = game->_move_factory.get_all_moves(game->_status.cur_turn);
    std::unique_ptr<PossibleMove> possible_king_moves = game->_move_factory.get_moves(game->_board.get_king_posn(game->_status.cur_turn));
    for (auto& move : possible_king_moves->moves) {
        possible_moves->add_move(std::move(move));
    }
    std::vector<std::unique_ptr<PossibleMove>> become_checked_moves;
    std::vector<std::unique_ptr<PossibleMove>> legal_moves;
    for (auto& move : possible_moves->moves) {
        if (game->_move_factory.will_move_result_check(*move)) {
            become_checked_moves.emplace_back(std::move(move));
        } else {
            legal_moves.emplace_back(std::move(move));
        }
    }
    if (legal_moves.empty()) {
        return { BoardPosn::Invalid, BoardPosn::Invalid };
    }
    int move_index = std::rand() % legal_moves.size();
    RawMove selected_move = possible_moves->moves[move_index]->get_raw_move();
    return selected_move;
}

ComputerLv2::ComputerLv2(ChessGame* game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv2::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = game->_move_factory.get_all_moves(game->_status.cur_turn);
    std::unique_ptr<PossibleMove> possible_king_moves = game->_move_factory.get_moves(game->_board.get_king_posn(game->_status.cur_turn));
    for (auto& move : possible_king_moves->moves) {
        possible_moves->add_move(std::move(move));
    }
    std::vector<std::unique_ptr<Move>> become_checked_moves;
    std::vector<std::unique_ptr<Move>> checking_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (game->_move_factory.will_move_result_check(*move)) {
            become_checked_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.will_move_check_opponent(*move)) {
            checking_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!checking_moves.empty()) {
        int move_index = std::rand() % checking_moves.size();
        return checking_moves[move_index]->get_raw_move();
    } else if (!capturing_moves.empty()) {
        int move_index = std::rand() % capturing_moves.size();
        return capturing_moves[move_index]->get_raw_move();
    } else if (!other_moves.empty()) {
        int move_index = std::rand() % other_moves.size();
        return other_moves[move_index]->get_raw_move();
    } else {
        return { BoardPosn::Invalid, BoardPosn::Invalid };
    }
}

ComputerLv3::ComputerLv3(ChessGame* game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv3::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = game->_move_factory.get_all_moves(game->_status.cur_turn);
    std::unique_ptr<PossibleMove> possible_king_moves = game->_move_factory.get_moves(game->_board.get_king_posn(game->_status.cur_turn));
    for (auto& move : possible_king_moves->moves) {
        possible_moves->add_move(std::move(move));
    }
    std::vector<std::unique_ptr<Move>> become_checked_moves;
    std::vector<std::unique_ptr<Move>> evading_capture_moves;
    std::vector<std::unique_ptr<Move>> safe_moves;
    std::vector<std::unique_ptr<Move>> checking_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (game->_move_factory.will_move_result_check(*move)) {
            become_checked_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.is_move_evading_capture(*move)) {
            evading_capture_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.is_move_safe(*move)) {
            safe_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.will_move_check_opponent(*move)) {
            checking_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!evading_capture_moves.empty()) {
        int move_index = std::rand() % evading_capture_moves.size();
        return evading_capture_moves[move_index]->get_raw_move();
    } else if (!safe_moves.empty()) {
        int move_index = std::rand() % safe_moves.size();
        return safe_moves[move_index]->get_raw_move();
    } else if (!checking_moves.empty()) {
        int move_index = std::rand() % checking_moves.size();
        return checking_moves[move_index]->get_raw_move();
    } else if (!capturing_moves.empty()) {
        int move_index = std::rand() % capturing_moves.size();
        return capturing_moves[move_index]->get_raw_move();
    } else if (!other_moves.empty()) {
        int move_index = std::rand() % other_moves.size();
        return other_moves[move_index]->get_raw_move();
    } else {
        return { BoardPosn::Invalid, BoardPosn::Invalid };
    }
}

ComputerLv4::ComputerLv4(ChessGame* game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv4::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = game->_move_factory.get_all_moves(game->_status.cur_turn);
    std::unique_ptr<PossibleMove> possible_king_moves = game->_move_factory.get_moves(game->_board.get_king_posn(game->_status.cur_turn));
    for (auto& move : possible_king_moves->moves) {
        possible_moves->add_move(std::move(move));
    }
    std::vector<std::unique_ptr<Move>> become_checked_moves;
    std::vector<std::unique_ptr<Move>> forking_moves;
    std::vector<std::unique_ptr<Move>> valuable_moves;
    std::vector<std::unique_ptr<Move>> evading_capture_moves;
    std::vector<std::unique_ptr<Move>> safe_moves;
    std::vector<std::unique_ptr<Move>> checking_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (game->_move_factory.will_move_result_check(*move)) {
            become_checked_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.will_attack_multi_pieces_next(*move)) {
            forking_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get()) && game->_move_factory.is_capture_valuable(*dynamic_cast<CaptureMove*>(move.get()))) {
            std::cout << "valuable" << std::endl;
            valuable_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.is_move_evading_capture(*move)) {
            evading_capture_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.is_move_safe(*move)) {
            safe_moves.emplace_back(std::move(move));
        } else if (game->_move_factory.will_move_check_opponent(*move)) {
            checking_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!valuable_moves.empty()) {
        int move_index = std::rand() % valuable_moves.size();
        return valuable_moves[move_index]->get_raw_move();
    } else if (!forking_moves.empty()) {
        int move_index = std::rand() % forking_moves.size();
        return forking_moves[move_index]->get_raw_move();
    } else if (!evading_capture_moves.empty()) {
        int move_index = std::rand() % evading_capture_moves.size();
        return evading_capture_moves[move_index]->get_raw_move();
    } else if (!safe_moves.empty()) {
        int move_index = std::rand() % safe_moves.size();
        return safe_moves[move_index]->get_raw_move();
    } else if (!checking_moves.empty()) {
        int move_index = std::rand() % checking_moves.size();
        return checking_moves[move_index]->get_raw_move();
    } else if (!capturing_moves.empty()) {
        int move_index = std::rand() % capturing_moves.size();
        return capturing_moves[move_index]->get_raw_move();
    } else if (!other_moves.empty()) {
        int move_index = std::rand() % other_moves.size();
        return other_moves[move_index]->get_raw_move();
    } else {
        return { BoardPosn::Invalid, BoardPosn::Invalid };
    }
}
