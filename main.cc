#include "piece.h"
#include "board.h"
#include "move.h"
#include <iostream>
#include <memory>
#include <optional>

int main() {
    Board b;
    b.standard_init();
    b.print_board();

    std::unique_ptr<Move> move1{new BasicMove{{0, 1}, {0, 3}, Piece::WhitePawn}};

    move1->execute(b);
    b.print_board();
    move1->undo(b);
    b.print_board();

    std::unique_ptr<Move> move2{new PromotionMove{{0, 1}, {1, 4}, Piece::WhitePawn, std::make_optional<Piece>(Piece::BlackPawn), Piece::WhiteQueen}};
    
    move2->execute(b);
    b.print_board();
    // move2->undo(b);
    // b.print_board();

    MoveHistory mh;
    mh.push_move(std::move(move2));
    mh.pop_last_move()->undo(b);
    b.print_board();

}
