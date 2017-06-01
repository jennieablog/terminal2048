#include "2048.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

G2048::G2048(const char* fileName){

	this->file = fileName;

	clear();

	if (fileName != NULL){

		ifstream inFile(fileName);

		if (inFile.fail()){
			cout << "You have decided to start a new game." << endl;
			cout << "Press ENTER key to continue...";
			cin.get();
			resetBoard();
			saveGame(fileName);
		}

		else{
			loadGame(fileName);
			cout << "Previous gameplay has been loaded." << endl;
			cout << "Press ENTER key to continue...";
			cin.get();
		}

	}

	if (fileName==NULL){
		cout << "WARNING: You did not input any filename." << endl;
		cout << "Press ENTER key to start a new game..." << endl;
		cin.get();
		resetBoard();
		this->file = "temp.dat";
		saveGame("temp.dat");
	}

}

void G2048::startGame(){

	ifstream inFile("hs");

	if (inFile.fail()){
		ofstream outFile("hs");
		outFile << "0";
		this->highscore = 0;
	}

	else{
		inFile >> this->highscore;
	}

	while (!hasReached2048()) {
		drawBoard();
		if (hasMove()){
			move(getInput());
			if (this->hasChange)
				addTile();
		}
		else{
			cout << "Game over! " << endl;
			if (this->score > this->highscore){
				this->highscore = this->score;
				cout << "New high score: " << this->highscore << endl;
				ofstream outFile("hs");
				outFile << this->highscore;
				outFile.close();
			}
			break;
		}
	}
	
	if (hasReached2048()){
		cout << "\nCongratulations! You have reached 2048." << endl;
		if (this->score > this->highscore){
				this->highscore = this->score;
				cout << "New high score: " << this->highscore << endl;
				ofstream outFile("hs");
				outFile << this->highscore;
				outFile.close();
			}
	}
	
	return;
}

void G2048::drawBoard(){

	clear();
	cout << "+==========COMMANDLINE==========+" << endl;
	cout << "+===========-2-0-4-8-===========+" << endl;
	cout << "+     BASED ON THE GAME BY:     +" << endl;
	cout << "+       Gabriele Cirulli        +" << endl;
	cout << "+       (c) Jennie Ablog        +" << endl;
	cout << "+===============================+" << endl;
	cout << "|SCORE:" << setw(5) << this->score << "         HS:" << setw(8) << this->highscore;
	cout << "|" << endl;
	cout << "+=======+=======+=======+=======+" << endl;

	for (int i = 0; i < 4; i++){

		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 4; k++){
				if ((j == 1) && (board[i][k] != 0)){
					cout << "| " << setw(4) << board[i][k];
					cout << "  ";
				}
				else
					cout << "|       ";
			}
			cout << "|\n";
		}
		
		for (int l = 0; l < 4; l++){
			cout << "+=======";
		}
		cout << "+\n";
	}

}

void G2048::resetBoard(){

	this->score = 0;
	
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			board[i][j] = 0;
		}
	}
	addTile();
	addTile();
}

void G2048::addTile(){

	int tiles[10] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 };
	int random_r, random_c, tile;

	srand(time(NULL));
	
	do{

		random_r = rand() % 4;
		random_c = rand() % 4;

	} while (board[random_r][random_c] != 0);
	
	tile = tiles[rand() % 10];
	board[random_r][random_c] = tile;

}

bool G2048::hasMove(){
	
	bool valid = false;
	int row, col, pivot;

	// Test if there is a zero anywhere on the board!
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (board[i][j] == 0){
				valid = true;
				break;
			}
		}
	}
	
	// Test rows if there are adjacent tiles of the same value!
	for (row = 0; row < 4; row++){
		col = 0;
		while (col < 3){
			pivot = board[row][col];
			if (pivot == board[row][col + 1]){
				valid = true;
				break;
			}
			col++;
		}
	}

	// Now, test the columns!
	for (col = 0; col < 4; col++){
		row = 0;
		while (row < 3){
			pivot = board[row][col];
			if (pivot == board[row + 1][col]){
				valid = true;
				break;
			}
			row++;
		}
	}
	return valid;
}

bool G2048::hasReached2048(){

	bool won = false;

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (board[i][j] == 2048){
				won = true;
				break;
			}
		}
	}

	return won;

}

char G2048::getInput(){

	char input;
	cout << "+===========CONTROLS============+" << endl;
	cout << "+     [W] - up   [A] - left     +" << endl;
	cout << "+     [S] - down [D] - right    +" << endl;
	cout << "+     [Q] - save  and  quit     +" << endl;
	cout << "+===============================+" << endl;
	cout << "\n   ENTER YOUR MOVE/S:  ";
	cin >> input;

	return input;

}

void G2048::move(char dir){

	this->hasChange = false;

	switch (dir){

	case 'W':
	case 'w':
		compressor('u');
		for (int c = 0; c < 4; c++){
			for (int r = 0; r < 3; r++){
				if ((board[r][c] == board[r + 1][c])&&(board[r][c] != 0)){
					board[r][c] += board[r + 1][c];
					board[r + 1][c] = 0;
					this->score += board[r][c];
					this->hasChange = true;
					compressor('u');
				}
			}
		}
		break;

	case 'A':
	case 'a':
		compressor('l');
		for (int r = 0; r < 4; r++){
			for (int c = 0; c < 3; c++){
				if ((board[r][c] == board[r][c + 1])&&(board[r][c] != 0)){
					board[r][c] += board[r][c + 1];
					board[r][c + 1] = 0;
					this->score += board[r][c];
					this->hasChange = true;
					compressor('l');
				}
			}
		}
		break;

	case 'S':
	case 's':
		compressor('d');
		for (int c = 0; c < 4; c++){
			for (int r = 3; r > 0; r--){
				if ((board[r][c] == board[r - 1][c])&&(board[r][c] != 0)){
					board[r][c] += board[r - 1][c];
					board[r - 1][c] = 0;
					this->score += board[r][c];
					this->hasChange = true;
					compressor('d');
				}
			}
		}
		break;

	case 'D':
	case 'd':
		compressor('r');
		for (int r = 0; r < 4; r++){
			for (int c = 3; c > 0; c--){
				if ((board[r][c] == board[r][c - 1])&&(board[r][c] != 0)){
					board[r][c] += board[r][c - 1];
					board[r][c - 1] = 0;
					this->score += board[r][c];
					this->hasChange = true;
					compressor('r');
				}
			}
		}
		break;
	case 'Q':
	case 'q':
		saveGame(this->file);
		if (this->file == "temp.dat"){
			string file_s;
			cout << "\nPlease input your name: ";
			cin >> file_s;
			rename("temp.dat", file_s.c_str());
		}
		cout << "\nSaving your game... Goodbye!" << endl;
		exit(1);
	}
}

void G2048::compressor(char dir){

	int sub_c,sub_r;

	switch (dir){
	
	case 'r':
		for (int r = 0; r < 4; r++){
			for (int c = 3; c >= 0; c--){
				if (board[r][c] == 0){
					sub_c = c;
					while (sub_c >= 0){
						if (board[r][sub_c] != 0){
							board[r][c] = board[r][sub_c];
							board[r][sub_c] = 0;
							this->hasChange = true;
							break;
						}
						sub_c--;
					}
				}
			}
		}
		break;

	case 'l':
		for (int r = 0; r < 4; r++){
			for (int c = 0; c < 4; c++){
				if (board[r][c] == 0){
					sub_c = c;
					while (sub_c < 4){
						if (board[r][sub_c] != 0){
							board[r][c] = board[r][sub_c];
							board[r][sub_c] = 0;
							this->hasChange = true;
							break;
						}
						sub_c++;
					}
				}
			}
		}
		break;

	case 'd':
		for (int c = 0; c < 4; c++){
			for (int r = 3; r >= 0; r--){
				if (board[r][c] == 0){
					sub_r = r;
					while (sub_r >= 0){
						if (board[sub_r][c] != 0){
							board[r][c] = board[sub_r][c];
							board[sub_r][c] = 0;
							this->hasChange = true;
							break;
						}
						sub_r--;
					}
				}
			}
		}
		break;

	case 'u':
		for (int c = 0; c < 4; c++){
			for (int r = 0; r < 4; r++){
				if (board[r][c] == 0){
					sub_r = r;
					while (sub_r < 4){
						if (board[sub_r][c] != 0){
							board[r][c] = board[sub_r][c];
							board[sub_r][c] = 0;
							this->hasChange = true;
							break;
						}
						sub_r++;
					}
				}
			}
		}
		break;
	}

}

void G2048::clear(){
	#ifdef __linux
		system("clear");
	#elif __APPLE__
		system("clear");
	#elif __unix
		system("clear");
	#elif __posix
		system("clear");
	#else
		system("cls");
	#endif
}

void G2048::loadGame(const char* file_name){

	ifstream inFile(file_name);

	while (!inFile.eof()){

		inFile >> this->score;
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				inFile >> board[i][j];
			}
		}
	}
}

void G2048::saveGame(const char* file_name){

	ofstream outFile(file_name);

	outFile << this->score << '\n';

	for (int r = 0; r < 4; r++){
		for (int c = 0; c < 4; c++){
			outFile << board[r][c] << '\n';
		}
	}

}
