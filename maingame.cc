#include "maingame.h"
#include <string>
#include <sstream>
#include <iostream>


MainGame::run() {
    std::string command;
    while(getline(std::cin, command);) {
        if (command.rfind("game", 0) == 0) {
            hand_player_signup(command);
        }
        if (command == "resign") {
                handle_resign();
        }
        if (command.rfind("move", 0) == 0) {
            handle_move(command);
        } 
        if (command == "setup") {
            handle_set_up();
        }
    }
}



// Function to create a player based on the input string
std::unique_ptr<BasePlayer> create_player(const std::string& playerType, ChessGame& game) {
    if (playerType == "human") {
        return std::make_unique<HumanPlayer>();
    } else if (playerType == "computer1") {
        return std::make_unique<ComputerLv1>(game);
    } else if (playerType == "computer2") {
        return std::make_unique<ComputerLv2>(game);
    } else if (playerType == "computer3") {
        return std::make_unique<ComputerLv3>(game);
    } else if (playerType == "computer4") {
        return std::make_unique<ComputerLv4>(game);
    } else {
        throw std::invalid_argument("Invalid player type");
    }
}

void MainGame::hand_player_signup(std::string command){
    std::string cmd;
    istringstream iss{command};
    iss >> cmd >> white_player_type >> black_player_type;
            try{
                _p1 = create_player(white_player_type, _game);
                _p2 = create_player(black_player_type, _game);

            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << std::endl;
            }

}

void MainGame::handle_resign() {
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

void MainGame::handle_set_up() {
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

            while(true) {
                getline(std::cin, s2)
                std::istringstream iss2{s2};
                iss2 >> setup_command;
                if (setup_command == "done") {
                   if (_game.is_valid_setup()) {
                        break;
                   }
                }
                if (setup_command == "+") {
                    iss2 >> piece_type >> board_pos;

                    char col;
                    int row;
                    
                    std::istringstream iss3{board_pos};
                    iss3 >> col >> row;
                    int col_char_to_int;
                    col_char_to_int = col - 97; // convert int to char 
                    
                    
                    if (piece_type == 'K') {
                        _game.set_piece(col_char_to_int, row, Piece::WhiteKing);
                    }
                    if (piece_type == 'Q') {
                        _game.set_piece(col_char_to_int, row, Piece::WhiteQueen);
                    }
                    if (piece_type == 'R') {
                        _game.set_piece(col_char_to_int, row, Piece::WhiteRook);
                    }
                    if (piece_type == 'B') {
                        _game.set_piece(col_char_to_int, row, Piece::WhiteKnight);
                    }
                    if (piece_type = 'P') {
                        _game.set_piece(col_char_to_int, row, Piece::WhitePawn);
                    }
                    if (piece_type == 'k') {
                        _game.set_piece(col_char_to_int, row, Piece::BlackKing);
                    }
                    if (piece_type == 'q') {
                        _game.set_piece(col_char_to_int, row, Piece::BlackQueen);
                    }
                    if (piece_type == 'r') {
                        _game.set_piece(col_char_to_int, row, Piece::BlackRook);
                    }
                    if (piece_type == 'b') {
                        _game.set_piece(col_char_to_int, row, Piece::BlackBishop);
                    }
                    if (piece_type = 'p') {
                        _game.set_piece(col_char_to_int, row, Piece::BlackPawn);
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
                    _game.reset_piece(col_char_to_int, row);

                }
                
                
                if (setup_command == "=") {
                    std::string color;
                    iss2 >> color;
                    _game.set_turn(color);
                }

            }

}

const BoardPosn& posn_composed(std::string posn) { // convert a string (e.g. "E6") to BoardPosn
    char col;
    int row;
               
    istringstream iss{command};
    iss >> col >> row;


    int col_adj;
    col_adj = col - 97;

    BoardPosn p{col_adj, row};
    return p;
}

void Maingame::handle_move(std::string command) {
     if(!gameRunning) {
                std::cout << "No game is currently running." << std::endl;
                return;
            }
            

            if (currentTurn == "White") {
                if (white_player_type == "human") {
                    std::string cmd;
                    std::string start_pos;
                    std::string final_pos;
                    istringstream iss(command);
                    iss >> cmd >> start_pos >> final_pos;
                    const BoardPosn& from = posn_composed(start_pos);
                    const BoardPosn& to = posn_composed(final_pos);
                    _game.execute_move(from, to);
                } else {
                     _p1.get_move();
                }
            }
            if (currentTurn == "Black") {
                if (white_player_type == "human") {
                    std::string cmd;
                    std::string start_pos;
                    std::string final_pos;
                    istringstream iss(command);
                    iss >> cmd >> start_pos >> final_pos;
                    const BoardPosn& from = posn_composed(start_pos);
                    const BoardPosn& to = posn_composed(final_pos);
                    _game.execute_move(from, to);
                    _game.execute_move(p_init, p_final);
                } else {
                     _p2.get_move();
                }
            }
            currentTurn = (currentTurn == "white") ? "black" : "white"; // switch turn 
}

void Maingame::handle_undo() {
    _game.undo_move();
}



void Maingame::print_score() const {
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << white_score << std::endl;
    std::cout << "Black: " << Black_score << std::endl;
}
