#include "piece.h"
#include "board.h"
#include "move.h"
#include "status.h"
#include "movefactory.h"
#include "chessgame.h"
#include "observer.h"
#include "maingame.h"
#include <iostream>
#include <memory>
#include <optional>

int main() {
    MainGame mg;
    mg.run();
}
