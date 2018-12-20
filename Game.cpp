#include "Game.h"
#include <fstream>
#include <vector>

Game::Game() {
    wordLength = 0;
    lifes = LIFES;
    started = false;
}

Game::~Game() {}

char* Game::getWord() {
    return word;
}

void Game::setWord(char* c) {
    word = c;
}

char* Game::getEncoded() {
    return encoded;
}

void Game::setEncoded(char* c) {
    encoded = c;
}

int Game::getWordLength() {
    return wordLength;
}

void Game::setWordLength(int n) {
    wordLength = n;
}

int Game::getLifes() {
    return lifes;
}

void Game::setLifes(int n) {
    lifes = n;
}

bool Game::isStarted() {
    return started;
}

void Game::setStarted(bool b) {
    started = b;
}

void Game::makeWord() {
    ifstream file;
	file.open(pathToWords.c_str());
	string line;

	vector<string> lines;
	while(getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	srand(time(NULL));
	string temp = lines[rand()%lines.size()];
	this->wordLength = temp.length();

	encode(temp);
}

void Game::encode(string s) {
    for(int i = 0; i < this->wordLength; i++) {
		this->word[i] = s[i];
		if(this->word[i] <= 'Z' && this->word[i] >= 'A')
			this->encoded[i] = '_';
	}
}