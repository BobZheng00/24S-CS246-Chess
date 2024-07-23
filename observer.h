#pragma once

<<<<<<< HEAD
// #include "piece.h"
// #include "move.h"
// #include "board.h"
// #include "window.h"
=======
#include "piece.h"
#include "move.h"
#include "board.h"
#include "window.h"
>>>>>>> d609bde322fdff200d0e056f5e0396e2880ac801
#include <vector>
#include <optional>
#include <memory>

enum class Result;

class BoardSubject;
class BoardPosn;

class BoardObserver {
public:
    BoardObserver() = default;
    virtual ~BoardObserver() = default;
    virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const = 0;
private:
    BoardSubject*  _p_board_subj;
};

class TextDisplay : public BoardObserver {
public:
    TextDisplay();
    virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const override;
};

class GraphicDisplay : public BoardObserver {
public:
    GraphicDisplay();
    virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const override;
// private:
//     XWindow _window;
};
