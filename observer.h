#include "piece.h"
#include "move.h"
#include "status.h"
#include "window.h"
#include <optional>

class BoardObserver {
public:
    BoardObserver() = default;
    virtual ~BoardObserver() = default;

    virtual void update_tile(const BoardPosn&, Result) const = 0;
    virtual void update_tile(int, int, Result) const = 0;
    virtual void update_board(Result) const = 0;
private:
    const Board*  _board_subject;
};

class TextDisplay : public BoardObserver {
public:
    TextDisplay();
    void update_tile(const BoardPosn&, Result) const override;
    void update_tile(int, int, Result) const override;
    virtual void update_board(Result) const override;
};

class GraphicDisplay : public BoardObserver {
public:
    GraphicDisplay();
    void update_tile(const BoardPosn&, Result) const override;
    void update_tile(int, int, Result) const override;
    virtual void update_board(Result) const override;
private:
    XWindow _window;
};
