#include "piece.h"
#include "board.h"
#include "move.h"
#include "status.h"
#include "movefactory.h"
#include "chessgame.h"
#include "observer.h"
#include <iostream>
#include <memory>
#include <optional>

int main() {
    Board b;
    GameStatus s;
    MoveFactory mf{b, s};
    // b.standard_init();
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

    // b.set_piece({2, 4}, Piece::WhitePawn);
    // b.set_piece({1, 5}, Piece::BlackPawn);
    // b.set_piece({3, 4}, Piece::BlackPawn);
    // s.black_last_double_pawn_push = std::make_unique<BoardPosn>(3, 4);
    // b.print_board();

    // std::unique_ptr<PossibleMove> pm1 = mf._pawn_moves({2, 4});

    // for (auto& m : pm1->moves) {
    //     m->execute(b, s);
    //     b.print_board();
    //     m->undo(b, s);
    //     b.print_board();
    // }

    // std::unique_ptr<PossibleMove> pm2 = mf._pawn_moves({6, 1});

    // for (auto& m : pm2->moves) {
    //     m->execute(b, s);
    //     b.print_board();
    //     m->undo(b, s);
    //     b.print_board();
    // }

    // std::unique_ptr<PossibleMove> pm3 = mf._pawn_moves({1, 5});

    // for (auto& m : pm3->moves) {
    //     m->execute(b, s);
    //     b.print_board();
    //     m->undo(b, s);
    //     b.print_board();
    // }

    
    // b.set_piece({1, 2}, Piece::BlackPawn);

    // std::unique_ptr<PossibleMove> pm3 = mf.get_all_moves(ChessColour::White);

    // for (auto& m : pm3->moves) {
    //     m->execute(b, s);
    //     b.print_board();
    //     s.print_status();
    //     m->undo(b, s);
    //     b.print_board();
    //     s.print_status();
    // }

    // b.reset_piece({0, 1});
    // std::unique_ptr<Move> move1{new BasicMove{{0, 0}, {0, 4}, Piece::WhiteRook, UniqueMove::UnableWhiteQueenSideCastling}};
    // move1->execute(b, s);
    // b.print_board();
    // s.print_status();
    // move1->undo(b, s);
    // b.print_board();
    // s.print_status();

    // std::cout << mf.is_move_safe(*move1) << std::endl;

    // b.set_piece({7, 2}, Piece::BlackQueen);

    // b.set_piece({0, 0}, Piece::WhiteRook);
    // // b.set_piece({1, 0}, Piece::WhiteKnight);
    // // b.set_piece({2, 0}, Piece::WhiteBishop);
    // // b.set_piece({3, 0}, Piece::WhiteQueen);
    // b.set_piece({4, 0}, Piece::WhiteKing);
    // // b.set_piece({5, 0}, Piece::WhiteBishop);
    // // b.set_piece({6, 0}, Piece::WhiteKnight);
    // b.set_piece({7, 0}, Piece::WhiteRook);

    // b.set_piece({0, 7}, Piece::BlackRook);
    // // b.set_piece({1, 7}, Piece::BlackKnight);
    // // b.set_piece({2, 7}, Piece::BlackBishop);
    // // b.set_piece({3, 7}, Piece::BlackQueen);
    // b.set_piece({4, 7}, Piece::BlackKing);
    // // b.set_piece({5, 7}, Piece::BlackBishop);
    // // b.set_piece({6, 7}, Piece::BlackKnight);
    // b.set_piece({7, 7}, Piece::BlackRook);

    // b.print_board();
    // s.print_status();

    // std::unique_ptr<PossibleMove> pm5 = mf.get_moves({7, 0});
    // pm5->moves[1]->execute(b, s);
    // b.print_board();
    // s.print_status();
    
    // std::unique_ptr<PossibleMove> pm4 = mf.get_moves({4, 0});
    // for (auto& m : pm4->moves) {
    //     m->execute(b, s);
    //     b.print_board();
    //     s.print_status();
    //     m->undo(b, s);
    //     b.print_board();
    //     s.print_status();
    // }

    // pm5->moves[1]->undo(b, s);
    // b.print_board();
    // s.print_status();

    // b.set_piece({2, 7}, Piece::BlackKing);

    // b.set_piece({1, 7}, Piece::BlackRook);
    // b.set_piece({3, 7}, Piece::BlackRook);
    // b.set_piece({2, 5}, Piece::WhiteQueen);


    // s.cur_turn = ChessColour::Black;

    // b.print_board();
    // s.print_status();

    // std::cout << mf.is_in_check(ChessColour::Black) << std::endl;
    // std::cout << mf.is_checkmated() << std::endl;
    // std::cout << mf.is_stalemated() << std::endl;

    ChessGame cg;
    std::unique_ptr<TextDisplay> td = std::make_unique<TextDisplay>(cg.get_board_for_observers());
    

    cg.setup_init();
    cg.set_status(Result::Unfinished);

    cg.set_piece({2, 7}, Piece::BlackKing);

    cg.set_piece({1, 7}, Piece::BlackRook);
    cg.set_piece({3, 7}, Piece::BlackRook);
    cg.set_piece({0, 4}, Piece::WhiteQueen);
    cg.set_piece({2, 0}, Piece::WhiteKing);

    cg._board.print_board();
    cg._status.print_status();
    std::cout << cg.execute_move({0, 4}, {2, 6}) << std::endl;
    // cg._board.print_board();
    cg._status.print_status();
    std::cout << cg.execute_move({2, 7}, {2, 6}) << std::endl;
    // cg._board.print_board();
    cg._status.print_status();

    cg.undo_move();
    // cg._board.print_board();
    cg._status.print_status();
    cg.undo_move();
    // cg._board.print_board();
    cg._status.print_status();
    // std::cout << cg.execute_move({1, 6}, {1, 4}) << std::endl;
    // cg._board.print_board();
    // cg._status.print_status();
    // // std::cout << cg.is_valid_setup() << std::endl;

    // cg._status.move_history->undo_last_move(cg._board, cg._status);
    // cg._board.print_board();
    // cg._status.print_status();
    // cg._status.move_history->undo_last_move(cg._board, cg._status);
    // cg._board.print_board();
    // cg._status.print_status();
    // cg._status.move_history->undo_last_move(cg._board, cg._status);
}
