#include "maingame.h"
#include <string>
#include <optional>
#include <sstream>
#include <iostream>
#include <memory>
#include "observer.h"

// problem1: how to deal with promotion (determine if the promotion move is valid first?)
// problem2: how to redisplay of the board after each set_up command (which place to place observers)
// problem3: try to determine win or lose by checking if the curr status == BlackWin/WhiteWin/Draw (but it says expression has a int type?? same thing happen in status class)

// ---------------------------------- helper func start --------------------------------------------

bool has_more_words(std::istringstream &iss)
{
    return !iss.eof();
}

bool is_computer_player(std::string player_type)
{
    if ((player_type == "computer1") || (player_type == "computer2") || (player_type == "computer3") ||
        (player_type == "computer4"))
    {
        return true;
    }
    return false;
}

BoardPosn posn_composed(std::string posn) { // convert a string (e.g. "E6") to BoardPosn
    char col;
    int row;

    std::istringstream iss{posn};
    iss >> col >> row;

    int col_adj;
    col_adj = col - 'a';
    

    return BoardPosn{col_adj, row - 1};
}

// Function to create a player based on the input string
std::unique_ptr<BasePlayer> create_player(const std::string &playerType, ChessGame &game)
{
    if (playerType == "human")
    {
        return std::make_unique<HumanPlayer>();
    }
    else if (playerType == "computer1")
    {
        return std::make_unique<ComputerLv1>(&game);
    }
    else if (playerType == "computer2")
    {
        return std::make_unique<ComputerLv2>(&game);
    }
    else if (playerType == "computer3")
    {
        return std::make_unique<ComputerLv3>(&game);
    }
    else if (playerType == "computer4")
    {
        return std::make_unique<ComputerLv4>(&game);
    }
    else
    {
        throw std::invalid_argument("Invalid player type");
    }
}

//----------------------------------helper func end----------------------------------

MainGame::MainGame(): _p1{nullptr}, _p2{nullptr}, _game{}, _text_observer{nullptr}, _graphics_observer{nullptr}, white_score{0}, black_score{0}, currentTurn{"white"}, white_player_type{}, black_player_type{}{
    _text_observer = std::make_unique<TextDisplay>(_game.get_board_for_observers());
    // _graphics_observer = std::make_unique<GraphicDisplay>(_game.get_board_for_observers());
}

void MainGame::run() {

    std::string command;
    while (getline(std::cin, command))
    {
        if (_game.get_status() == Result::WhiteWin)
        {
            std::cout << "White wins." << std::endl;
            break;
        }
        if (_game.get_status() == Result::BlackWin)
        {
            std::cout << "Black wins." << std::endl;
            break;
        }
        if (_game.get_status() == Result::Draw)
        {
            std::cout << "Game is a draw." << std::endl;
            break;
        }
        
        if (_game.get_status() == Result::WhiteInCheck)
        {
            std::cout << "White is in check." << std::endl;
        }
        if (_game.get_status() == Result::BlackInCheck)
        {
            std::cout << "Black is in check." << std::endl;
        }
        

        if (command.rfind("game", 0) == 0)
        {
            handle_player_sign_up(command);
        }
        if (command == "resign")
        {
            handle_resign();
        }
        if (command.rfind("move", 0) == 0)
        {
            handle_move(command);
            // BoardObserver *t = new TextDisplay();
            // observers.emplace_back(t);
        }
        if (command == "setup")
        {
            handle_set_up();
        }
        if (command == "undo")
        {
            handle_undo();  
        }
    }

    print_score();
}

void MainGame::handle_player_sign_up(std::string command)
{
    std::string cmd;
    std::istringstream iss{command};
    iss >> cmd >> white_player_type >> black_player_type;
    try
    {
        _p1 = create_player(white_player_type, _game);
        _p2 = create_player(black_player_type, _game);

        _game.set_status(Result::Unstarted);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void MainGame::handle_resign()
{
    if ((_game.get_status() == Result::Unstarted) || (_game.get_status() == Result::Setup))
    {
        std::cout << "No game is currently running." << std::endl;
        return;
    }
    if (_game.get_turn() == ChessColour::White)
    {
        _game.set_status(Result::BlackWin);
    }
    else
    {
        _game.set_status(Result::WhiteWin);
    }
}

void MainGame::handle_set_up()
{
    if (((_game.get_status() == Result::Unfinished) || (_game.get_status() == Result::WhiteInCheck) || (_game.get_status() == Result::BlackInCheck)))
    {
        std::cout << "Cannot enter setup mode while a game is running." << std::endl;
        return;
    }
    _game.set_status(Result::Setup);
    std::cout << "Setup mode entered." << std::endl;

    std::string s2;
    std::string setup_command;

    char piece_type;
    std::string board_pos;

    while (true)
    {
        getline(std::cin, s2);
        std::istringstream iss2{s2};
        iss2 >> setup_command;
        if (setup_command == "done")
        {
            if (_game.is_valid_setup())
            {
                _game.set_status(Result::Unfinished);
                break;
            }
        }
        if (setup_command == "+")
        {
            iss2 >> piece_type >> board_pos;

            const BoardPosn &p = posn_composed(board_pos);

            if (piece_type == 'K')
            {
                _game.set_piece(p, Piece::WhiteKing);
            }
            if (piece_type == 'Q')
            {
                _game.set_piece(p, Piece::WhiteQueen);
            }
            if (piece_type == 'B')
            {
                _game.set_piece(p, Piece::WhiteBishop);
            }
            if (piece_type == 'R')
            {
                _game.set_piece(p, Piece::WhiteRook);
            }
            if (piece_type == 'N')
            {
                _game.set_piece(p, Piece::WhiteKnight);
            }
            if (piece_type == 'P')
            {
                _game.set_piece(p, Piece::WhitePawn);
            }
            if (piece_type == 'k')
            {
                _game.set_piece(p, Piece::BlackKing);
            }
            if (piece_type == 'q')
            {
                _game.set_piece(p, Piece::BlackQueen);
            }
            if (piece_type == 'r')
            {
                _game.set_piece(p, Piece::BlackRook);
            }
            if (piece_type == 'b')
            {
                _game.set_piece(p, Piece::BlackBishop);
            }
             if (piece_type == 'n')
            {
                _game.set_piece(p, Piece::BlackKnight);
            }
            if (piece_type == 'p')
            {
                _game.set_piece(p, Piece::BlackPawn);
            }
        }
        if (setup_command == "-")
        {
            iss2 >> board_pos;
            const BoardPosn &p = posn_composed(board_pos);

            _game.reset_piece(p);
        }

        if (setup_command == "=")
        {
            std::string color;
            iss2 >> color;
            if (color == "white")
            {
                _game.set_turn(ChessColour::White);
            }
            if (color == "black")
            {
                _game.set_turn(ChessColour::Black);
            }
        }
    }
}

void MainGame::handle_move(std::string command)
{
    if (_game.get_status() == Result::Setup)
    {
        std::cout << "Cannot make a move while in setup mode." << std::endl;
        return;
    }

    if ((_game.get_status() == Result::Unstarted) && ((!_p1.get()) || (!_p2.get())))
    {
        std::cout << "No players have signed up." << std::endl;
        return;
    }
    else if (_game.get_status() == Result::Unstarted)
    {
        _game.regular_init();
        _game.set_status(Result::Unfinished);
    }

    std::cout << command << std::endl;
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if ((_game.get_turn() == ChessColour::White && white_player_type == "human") || (_game.get_turn() == ChessColour::Black && black_player_type == "human"))
    {   
        std::cout << command << std::endl;
        std::string start_pos;
        std::string final_pos;
        iss >> start_pos >> final_pos;
        const BoardPosn &from = posn_composed(start_pos);
        const BoardPosn &to = posn_composed(final_pos);

        char piece_type;
        if (!iss.eof()) {
            iss >> piece_type;
            if ((piece_type == 'Q') || (piece_type == 'q'))
            {
                 _game.execute_move(from, to, std::make_optional<PieceType>(PieceType::Queen));
            }
            if ((piece_type == 'R') || (piece_type == 'r'))
            {
               _game.execute_move(from, to, std::make_optional<PieceType>(PieceType::Rook));
            }
            if ((piece_type == 'B') || (piece_type == 'b'))
            {
                _game.execute_move(from, to, std::make_optional<PieceType>(PieceType::Bishop));
            }
            if ((piece_type == 'N') || (piece_type == 'n'))
            {
                _game.execute_move(from, to, std::make_optional<PieceType>(PieceType::Knight));
            }
        } else {
             _game.execute_move(from, to);
        }
    }

    else if ((_game.get_turn() == ChessColour::White && is_computer_player(white_player_type)) || (_game.get_turn() == ChessColour::Black && is_computer_player(black_player_type)))
    {
        std::cout << "computer is supposed to move now" << std::endl;
        RawMove move = _p2->get_move();
        std::cout << move.from.file << " " << move.from.rank << " " << move.to.file << " " << move.to.rank << std::endl;
        _game.execute_move(move.from, move.to);
    }
}

void MainGame::handle_undo()
{
    _game.undo_move();
}

void MainGame::print_score() const
{
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << white_score << std::endl;
    std::cout << "Black: " << black_score << std::endl;
}
