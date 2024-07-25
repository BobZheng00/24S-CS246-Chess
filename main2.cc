#include "piece.h"
#include "board.h"
#include "move.h"
#include "chessgame.h"
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <optional>





int main(){
    std::string s1;
    std::string command;
    ChessGame g;
    std::unique_ptr<BasePlayer> whitePlayer, blackPlayer;
   // std::vector<Observer *> observers;
    while(true) {
        getline(std::cin, command);


        if(command.rfind("game", 0) == 0) { // see if string command contain substring game or not
           
        }



        else if(command == "resign") {
            if(!gameRunning) {
                std::cout << "No game is currently running." << std::endl;
                return;
            }
            if(currentTurn == "White") {
                g.set_status(Result::BlackWin);
            } else {
                g.set_status(Result::WhiteWIn);
            }
            
            g.print_status();
            gameRunning = false;
        }

        else if(command.rfind("move", 0) == 0) {
            if(!gameRunning) {
                std::cout << "No game is currently running." << std::endl;
                return;
            }

            std::string origin_pos;
            std::string final_pos;
            std::string cmd;
            iss1 >> cmd >> origin_pos >> final_pos;

            // beginning position
            char col_begin;
            int row_begin;
                    
            std::istringstream iss1{origin_pos};
            iss1 >> col_begin >> row_begin;
            int col_char_to_int_begin;
            col_char_to_int_begin = col_begin - 97;
            
            // final positon
            char col_final;
            int row_final;
                
            std::istringstream iss2{final_pos};
            iss2 >> col_final >> row_final;
            int col_char_to_int_final;
            col_char_to_int_final = col_final - 97;
                    

            BoardPosn p_init{col_char_to_int_begin, row_begin};
            BoardPosn p_final{col_char_to_int_final, row_final};

            g.execute_move(p_init, p_final)
            currentTurn = (currentTurn == "white") ? "black" : "white"; // switch turn 

        }

        else if(command == "setup") {
            if(gameRunning) {
                std::cout << "Cannot enter setup mode while a game is running." << std::endl;
                return;
            }
            inSetupMode = true;
            std::cout << "Setup mode entered." << std::endl;

            std::string s2;
            std::string setup_command;
            
            char piece_type;
            std::string board_pos;

            while(getline(std::cin, s2)) {
                std::istringstream iss2{s2};
                iss2 >> setup_command;
                if (setup_command == "+") {
                    iss2 >> piece_type >> board_pos;

                    char col;
                    int row;
                    
                    std::istringstream iss3{board_pos};
                    iss3 >> col >> row;
                    int col_char_to_int;
                    col_char_to_int = col - 97; // convert int to char 
                    
                    
                    if (piece_type == 'K') {
                        g.set_piece(col_char_to_int, row, Piece::WhiteKing);
                    }
                    if (piece_type == 'Q') {
                        g.set_piece(col_char_to_int, row, Piece::WhiteQueen);
                    }
                    if (piece_type == 'R') {
                        g.set_piece(col_char_to_int, row, Piece::WhiteRook);
                    }
                    if (piece_type == 'B') {
                        g.set_piece(col_char_to_int, row, Piece::WhiteKnight);
                    }
                    if (piece_type = 'P') {
                        g.set_piece(col_char_to_int, row, Piece::WhitePawn);
                    }
                    if (piece_type == 'k') {
                        g.set_piece(col_char_to_int, row, Piece::BlackKing);
                    }
                    if (piece_type == 'q') {
                        g.set_piece(col_char_to_int, row, Piece::BlackQueen);
                    }
                    if (piece_type == 'r') {
                        g.set_piece(col_char_to_int, row, Piece::BlackRook);
                    }
                    if (piece_type == 'b') {
                        g.set_piece(col_char_to_int, row, Piece::BlackBishop);
                    }
                    if (piece_type = 'p') {
                        g.set_piece(col_char_to_int, row, Piece::BlackPawn);
                    }

                }
                if (setup_command == "-") {
                    iss2 >> board_pos;

                    char col;
                    int row;
                    
                    std::istringstream iss3{board_pos};
                    iss3 >> col >> row;
                    int col_char_to_int;
                    col_char_to_int = col - 97; // convert int to char 
                    g.reset_piece(col_char_to_int, row);

                }
                
                
                if (setup_command == "=") {
                    std::string color;
                    iss2 >> color;
                    g.set_turn(color);
                }

            }
        }
        
        
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}