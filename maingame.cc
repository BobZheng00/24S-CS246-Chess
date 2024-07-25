#include "maingame.h"
#include <string>
#include <sstream>
#include <iostream>

// problem1: how to deal with promotion (determine if the promotion move is valid first?)
// problem2: redisplay of the board after each set_up command
// problem3: consider determine win or lose by checking if the curr status == BlackWin/WhiteWin/Draw


// ---------------------------------- helper func start --------------------------------------------

bool is_computer_player(std::string player_type) {
    if ((player_type == "computer1") || (player_type == "computer2") || (player_type == "computer3") ||
        (player_type == "computer4") ) {
            return true;
        }
    return false;
}

const BoardPosn& posn_composed(std::string posn) { // convert a string (e.g. "E6") to BoardPosn
    char col;
    int row;
               
    std::istringstream iss{posn};
    iss >> col >> row;


    int col_adj;
    col_adj = col - 'a';

    return BoardPosn{col_adj, row - 1};
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


//----------------------------------helper func end----------------------------------

void MainGame::run() {

    std::string command;
    std::vector<BoardObserver *> observers;
    while(getline(std::cin, command)) {
       /* if (_game._status.result == Result::WhiteWin) {

        }*/

        if (command.rfind("game", 0) == 0) {
            handle_player_sign_up(command);
        } 
        if (command == "resign") {
                handle_resign();
        } 
        if (command.rfind("move", 0) == 0) {
            handle_move(command);
            //BoardObserver *t = new TextDisplay();
            //observers.emplace_back(t);
        }
        if (command == "setup") {
            handle_set_up();
        } 

    }
}



void MainGame::handle_player_sign_up(std::string command){
    std::string cmd;
    std::istringstream iss{command};
    iss >> cmd >> white_player_type >> black_player_type;
            try{
                _p1 = create_player(white_player_type, _game);
                _p2 = create_player(black_player_type, _game);
                gameRunning = true;
                currentTurn = "White";
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
        _game.set_status(Result::BlackWin);
    } else {
        _game.set_status(Result::WhiteWin);
    }
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
                getline(std::cin, s2);
                std::istringstream iss2{s2};
                iss2 >> setup_command;
                if (setup_command == "done") {
                   if (_game.is_valid_setup()) {
                        break;
                   }
                }
                if (setup_command == "+") {
                    iss2 >> piece_type >> board_pos;
                    
                    const BoardPosn& p = posn_composed(board_pos);
                    
                    if (piece_type == 'K') {
                        _game.set_piece(p, Piece::WhiteKing);
                    }
                    if (piece_type == 'Q') {
                        _game.set_piece(p, Piece::WhiteQueen);
                    }
                    if (piece_type == 'R') {
                        _game.set_piece(p, Piece::WhiteRook);
                    }
                    if (piece_type == 'B') {
                        _game.set_piece(p, Piece::WhiteKnight);
                    }
                    if (piece_type = 'P') {
                        _game.set_piece(p, Piece::WhitePawn);
                    }
                    if (piece_type == 'k') {
                        _game.set_piece(p, Piece::BlackKing);
                    }
                    if (piece_type == 'q') {
                        _game.set_piece(p, Piece::BlackQueen);
                    }
                    if (piece_type == 'r') {
                        _game.set_piece(p,  Piece::BlackRook);
                    }
                    if (piece_type == 'b') {
                        _game.set_piece(p, Piece::BlackBishop);
                    }
                    if (piece_type = 'p') {
                        _game.set_piece(p, Piece::BlackPawn);
                    }
                }
                if (setup_command == "-") {
                    iss2 >> board_pos;
                    const BoardPosn& p = posn_composed(board_pos);
        
                    _game.reset_piece(p);

                }
                
                
                if (setup_command == "=") {
                    std::string color;
                    iss2 >> color;
                    if (color == "white") {
                        _game.set_turn(ChessColour::White);
                    }
                    if (color == "black") {
                        _game.set_turn(ChessColour::Black);
                    }
                }

            }

}


void MainGame::handle_move(std::string command) {
    if(!gameRunning) {
        std::cout << "No game is currently running." << std::endl;
            return;
    }
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
            
    if ( (currentTurn == "white" && white_player_type == "human")  || (currentTurn == "black" && black_player_type == "human")){
        std::string start_pos;
        std::string final_pos;
        iss >> start_pos >> final_pos;
        const BoardPosn& from = posn_composed(start_pos);
        const BoardPosn& to = posn_composed(final_pos);
            _game.execute_move(from, to);
    } 

    if (currentTurn == "white" && is_computer_player(white_player_type) || currentTurn == "black" && is_computer_player(black_player_type)) 

     {
            RawMove white_move = _p1->get_move();
            _game.execute_move(white_move.from, white_move.to);
    }
        currentTurn = (currentTurn == "white") ? "black" : "white"; // switch turn 
}



void MainGame::handle_undo() {
    _game.undo_move();
}



void MainGame::print_score() const {
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << white_score << std::endl;
    std::cout << "Black: " << black_score << std::endl;
}
