#include "chessgame.h"
#include "move.h"
#include "movefactory.h"

class BasePlayer {
public:
    BasePlayer();
    virtual ~BasePlayer() = default;
    virtual RawMove get_move() = 0;
};

class HumanPlayer : public BasePlayer {
public:
    RawMove get_move() override;
};

class ComputerPlayer: public BasePlayer {
public:
    virtual RawMove get_move() const = 0;
private:
    ChessGame& game;
};

class ComputerLv1: public ComputerPlayer {
public:
    ComputerLv1(ChessGame& game);
    RawMove get_move() const override;
};

class ComputerLv2: public ComputerPlayer {
public:
    ComputerLv2(ChessGame& game);
    RawMove get_move() const override;
};

class ComputerLv3: public ComputerPlayer { 
public:
    ComputerLv3(ChessGame& game);
    RawMove get_move() const override;
};

class ComputerLv4: public ComputerPlayer {
public:
    ComputerLv4(ChessGame& game);
    RawMove get_move() const override;
};
