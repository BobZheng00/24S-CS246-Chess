#pragma once

#include "chessgame.h"
#include "player.h"
#include <string>

class MainGame {
public:
    MainGame();
    ~MainGame() = default;

    void run();

private:
    void handle_player_sign_up(std::string command);
    void handle_resign();
    void handle_set_up();
    void handle_move(std::string command);
    void handle_undo();
    void print_score() const;
    //void reset_game();

    std::unique_ptr<BasePlayer> _p1, _p2;
    ChessGame _game;
    std::unique_ptr<BoardObserver> _text_observer;
    std::unique_ptr<BoardObserver> _graphics_observer;
    double white_score;
    double black_score;

    std::string currentTurn;
    std::string white_player_type;
    std::string black_player_type;
    
};
