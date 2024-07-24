#pragma once

#include "window.h"
#include "piece.h"
#include "move.h"
#include "board.h"
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
    protected:
        std::shared_ptr<BoardSubject> _p_board_subj;
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
private:
    mutable Xwindow _window;
};
