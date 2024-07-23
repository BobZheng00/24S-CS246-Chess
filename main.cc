#include "piece.h"
#include "board.h"
#include "move.h"
#include "status.h"
#include "movefactory.h"
#include <iostream>
#include <memory>
#include <optional>

int main() {
    Board b;
    Status s;
    MoveFactory mf{b, s};
    b.standard_init();
    b.print_board();

    // std::unique_ptr<Move> move1{new BasicMove{{0, 1}, {0, 3}, Piece::WhitePawn}};

    // move1->execute(b);
    // b.print_board();
    // move1->undo(b);
    // b.print_board();

    // std::unique_ptr<Move> move2{new PromotionMove{{0, 1}, {1, 4}, Piece::WhitePawn, std::make_optional<Piece>(Piece::BlackPawn), Piece::WhiteQueen}};
    
    // move2->execute(b);
    // b.print_board();
    // // move2->undo(b);
    // // b.print_board();

    // MoveHistory mh;
    // mh.push_move(std::move(move2));
    // mh.pop_last_move()->undo(b);
    // b.print_board();

    // std::unique_ptr<PossibleMove> pm1 = mf._diagonal_moves({5, 1});

    // for (auto& m : pm1->moves) {
    //     m->execute(b);
    //     b.print_board();
    //     m->undo(b);
    //     b.print_board();
    // }

    // std::unique_ptr<PossibleMove> pm2 = mf._linear_moves({1, 4});

    // for (auto& m : pm2->moves) {
    //     m->execute(b);
    //     b.print_board();
    //     m->undo(b);
    //     b.print_board();
    // }

    // b.set_piece({0, 6}, Piece::WhitePawn);
    // b.reset_piece({0, 7});
    // b.reset_piece({1, 7});
    // b.print_board();

    // std::unique_ptr<PossibleMove> pm1 = mf._pawn_moves({0, 6});

    // for (auto& m : pm1->moves) {
    //     m->execute(b);
    //     b.print_board();
    //     m->undo(b);
    //     b.print_board();
    // }

    b.set_piece({2, 4}, Piece::WhitePawn);
    b.set_piece({1, 5}, Piece::BlackPawn);
    b.set_piece({3, 4}, Piece::BlackPawn);
    s.black_last_double_pawn_push = {3, 4};
    b.print_board();

    std::unique_ptr<PossibleMove> pm1 = mf._pawn_moves({2, 4});

    for (auto& m : pm1->moves) {
        m->execute(b, s);
        b.print_board();
        m->undo(b, s);
        b.print_board();
    }

    std::unique_ptr<PossibleMove> pm2 = mf._pawn_moves({6, 1});

    for (auto& m : pm2->moves) {
        m->execute(b, s);
        b.print_board();
        m->undo(b, s);
        b.print_board();
    }

    std::unique_ptr<PossibleMove> pm3 = mf._pawn_moves({1, 5});

    for (auto& m : pm3->moves) {
        m->execute(b, s);
        b.print_board();
        m->undo(b, s);
        b.print_board();
    }

}
