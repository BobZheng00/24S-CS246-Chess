#include "piece.h"
#include <array>
#include <optional>
#include <vector>

class BoardObserver;

class BoardSubject {
  std::vector<BoardObserver*> observers;
 public:
  void attach(BoardObserver* o);
  void detach(BoardObserver* o);
  void notifyObservers();
  virtual ~BoardSubject() = default;
};


class Board: public BoardSubject {
public:
    Board();

    void clear();
    void set_standard_position();
    BoardPosn get_king_posn(ChessColour colour) const;

    virtual std::optional<Piece> get_piece(int row, int col) const;
    virtual std::optional<Piece> get_piece(const BoardPosn& posn) const;
    void set_piece(int row, int col, const Piece& piece);
    void set_piece(const BoardPosn& posn, const Piece& piece);

private:
    std::array<std::array<std::optional<Piece>, 8>, 8> board;
};
