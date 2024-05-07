#include "game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Game* game = new Game();
    if(game->init() != 0){
        std::cerr << "Game initialization failed" << std::endl;
        delete game;
        return -1;
    }

    int32_t code = game->start();
    delete game;

    return code;
}
