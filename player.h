#include "chessgame.h"
#include "move.h"

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
    RawMove get_move() const override;
};

class ComputerLv2: public ComputerPlayer {
public:
    RawMove get_move() const override;
};

class ComputerLv3: public ComputerPlayer { 
public:
    RawMove get_move() const override;
};

class ComputerLv4: public ComputerPlayer {
public:
    RawMove get_move() const override;
};
