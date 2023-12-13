#include <iostream>
#include "../headers/vector2.h"
#include "../headers/pong.h"

int main() {
    Pong *game = new Pong(60, 100, 100);	// mudar valores das dimensoes
    game->Play();
    delete game;

    return 0;
}