#pragma once

#include "window.h"
#include <vector>
#include <optional>
#include <memory>

enum class Result;

class BoardSubject;
class BoardPosn;

class BoardObserver {
public:
    virtual ~BoardObserver() = default;
    virtual void update_board(std::shared_ptr<std::vector<BoardPosn>>, Result) const = 0;
};

class TextDisplay : public BoardObserver {
public:
    TextDisplay(BoardSubject* p_board_subj);
    virtual ~TextDisplay() override;
    virtual void update_board(std::shared_ptr<std::vector<BoardPosn>>, Result) const override;
private:
    BoardSubject* _p_board_subj;
};

class GraphicDisplay : public BoardObserver {
public:
    GraphicDisplay(BoardSubject* p_board_subj);
    virtual ~GraphicDisplay() override;
    virtual void update_board(std::shared_ptr<std::vector<BoardPosn>>, Result) const override;
private:
    mutable Xwindow _window;
    BoardSubject* _p_board_subj;
};
