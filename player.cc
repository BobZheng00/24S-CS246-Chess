#include "player.h"
#include <cstdlib>
#include <ctime>

ComputerLv1::ComputerLv1(ChessGame& game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv1::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = MoveFactory(_board, _status).get_all_moves(game.current_player_color());
    if (possible_moves->moves.empty()) {
        return { BoardPosn::Invalid, BoardPosn::Invalid };
    }
    int move_index = std::rand() % possible_moves->moves.size();
    RawMove selected_move = possible_moves->moves[move_index]->get_raw_move();
    return selected_move;
}

ComputerLv2::ComputerLv2(ChessGame& game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv2::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = MoveFactory(_board, _status).get_all_moves(game.current_player_color());
    std::vector<std::unique_ptr<Move>> check_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (game.is_check_after_move(*move)) {
            check_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!check_moves.empty()) {
        int move_index = std::rand() % check_moves.size();
        return check_moves[move_index]->get_raw_move();
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

ComputerLv3::ComputerLv3(ChessGame& game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv3::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = MoveFactory(_board, _status).get_all_moves(game.current_player_color());
    std::vector<std::unique_ptr<Move>> safe_moves;
    std::vector<std::unique_ptr<Move>> check_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (_status.is_move_safe(*move)) {
            safe_moves.emplace_back(std::move(move));
        } else if (game.is_check_after_move(*move)) {
            check_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!safe_moves.empty()) {
        int move_index = std::rand() % safe_moves.size();
        return safe_moves[move_index]->get_raw_move();
    } else if (!check_moves.empty()) {
        int move_index = std::rand() % check_moves.size();
        return check_moves[move_index]->get_raw_move();
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

ComputerLv4::ComputerLv4(ChessGame& game) : ComputerPlayer(game) {
    std::srand(std::time(nullptr));
}

RawMove ComputerLv4::get_move() const {
    std::unique_ptr<PossibleMove> possible_moves = MoveFactory(_board, _status).get_all_moves(game.current_player_color());
    std::vector<std::unique_ptr<Move>> valuable_moves;
    std::vector<std::unique_ptr<Move>> safe_moves;
    std::vector<std::unique_ptr<Move>> check_moves;
    std::vector<std::unique_ptr<Move>> capturing_moves;
    std::vector<std::unique_ptr<Move>> other_moves;
    for (auto& move : possible_moves->moves) {
        if (dynamic_cast<CaptureMove*>(move.get()) && _status.is_capture_valuable(*move)) {
            valuable_captures.emplace_back(std::move(move));
        } else if (_status.is_move_safe(*move)) {
            safe_moves.emplace_back(std::move(move));
        } else if (game.is_check_after_move(*move)) {
            check_moves.emplace_back(std::move(move));
        } else if (dynamic_cast<CaptureMove*>(move.get())) {
            capturing_moves.emplace_back(std::move(move));
        } else {
            other_moves.emplace_back(std::move(move));
        }
    }
    if (!valuable_captures.empty()) {
        int move_index = std::rand() % valuable_captures.size();
        return valuable_captures[move_index]->get_raw_move();
    } else if (!safe_moves.empty()) {
        int move_index = std::rand() % safe_moves.size();
        return safe_moves[move_index]->get_raw_move();
    } else if (!check_moves.empty()) {
        int move_index = std::rand() % check_moves.size();
        return check_moves[move_index]->get_raw_move();
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
