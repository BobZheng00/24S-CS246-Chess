#include "chessgame.h"
#include "player.h"
#include <string>

class MainGame {
public:
    MainGame();
    ~MainGame();

    void run();

private:
    void handle_player_sign_up();
    void handle_resign();
    void handle_set_up();
    void handle_move();
    void handle_undo();
    void print_score() const;

    std::unique_ptr<BasePlayer> _p1, _p2;
    ChessGame _game;
    double white_score;
    double black_score;
};
