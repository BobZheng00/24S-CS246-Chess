#pragma once

#include "move.h"

class ChessGame;

class BasePlayer {
public:
    BasePlayer() = default;
    virtual ~BasePlayer() = default;
    virtual RawMove get_move() const = 0;
};

class HumanPlayer : public BasePlayer {
public:
    HumanPlayer() = default;
    virtual ~HumanPlayer() = default;
    RawMove get_move() const override;
};

class ComputerPlayer: public BasePlayer {
public:
    ComputerPlayer(ChessGame* game);
    virtual RawMove get_move() const = 0;
protected:
    ChessGame* game;
};

class ComputerLv1: public ComputerPlayer {
public:
    ComputerLv1(ChessGame* game);
    virtual RawMove get_move() const override;
};

class ComputerLv2: public ComputerPlayer {
public:
    ComputerLv2(ChessGame* game);
    virtual RawMove get_move() const override;
};

class ComputerLv3: public ComputerPlayer { 
public:
    ComputerLv3(ChessGame* game);
    virtual RawMove get_move() const override;
};

class ComputerLv4: public ComputerPlayer {
public:
    ComputerLv4(ChessGame* game);
    virtual RawMove get_move() const override;
};
