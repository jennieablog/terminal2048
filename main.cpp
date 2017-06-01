#include "2048.h"
#include <cstdlib>

int main(int argc, char** argv) {
	G2048 game(argc > 1 ? argv[1] : NULL);
	game.startGame();
}