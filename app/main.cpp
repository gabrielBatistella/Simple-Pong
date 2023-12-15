#include <iostream>
#include "../headers/vector2.h"
#include "../headers/pong.h"

int main(int argc, char* args[]) {
    Pong *game = new Pong(60, 640, 480);
    game->Play();
    delete game;
    return 0;
}