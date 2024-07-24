#pragma once

<<<<<<< HEAD
#include "piece.h"
#include "move.h"
#include "board.h"
#include "window.h"
=======
#include "window.h"
#include "piece.h"
#include "move.h"
#include "board.h"
>>>>>>> fd7ec3de7ebc1300eff45a6db7d218cf2b69cbfc
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
        std::shared_ptr<BoardObserver> _p_board_subj;
};

class TextDisplay : public BoardObserver {
    public:
        TextDisplay();
        virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const override;
};

class GraphicDisplay : public BoardObserver {
<<<<<<< HEAD
    public:
        GraphicDisplay();
        virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const override;
    private:
        Xwindow _window;
=======
public:
    GraphicDisplay();
    virtual void update_board(std::unique_ptr<std::vector<BoardPosn>>, Result) const override;
private:
    Xwindow _window;
>>>>>>> fd7ec3de7ebc1300eff45a6db7d218cf2b69cbfc
};
