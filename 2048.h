#ifndef __2048__
#define __2048__

class G2048 {
private:
	int board[4][4];
	int score;
	
	void addTile();
	char getInput();
	void move(char direction);
	void drawBoard();
	bool hasReached2048();
	bool hasMove();
	void loadGame(const char*);
	void saveGame(const char*);

	const char* file;
	bool hasChange;
	int highscore;
	void resetBoard();
	void compressor(char direction);
	void clear();

public:
	G2048(const char* filename);
	void startGame();
};

#endif
