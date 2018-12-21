#include "Game.h"
#include <fstream>
#include <vector>
#include <cstring>

Game::Game() {
    wordLength = 0;
    lifes = LIFES;
    started = false;
}

Game::~Game() {}

char* Game::getWord() {
    return this->word;
}

void Game::setWord(char* c) {
    this->word = c;
}

char* Game::getEncoded() {
    return this->encoded;
}

void Game::setEncoded(char* c) {
    this->encoded = c;
}

int Game::getWordLength() {
    return this->wordLength;
}

void Game::setWordLength(int n) {
    this->wordLength = n;
}

int Game::getLifes() {
    return this->lifes;
}

void Game::setLifes(int n) {
    this->lifes = n;
}

bool Game::isStarted() {
    return this->started;
}

void Game::setStarted(bool b) {
    this->started = b;
}

map<int, Player*> Game::getPlayers() {
    return this->players;
}

void Game::makeWord() {
    puts("zaraz otwieram plik");
    ifstream file;
	file.open(pathToWords.c_str());
    puts("plik otwarty");
	string line;

	vector<string> lines;
	while(getline(file, line)) {
		lines.push_back(line);
	}
	file.close();
    puts("plik zamkniety");

	srand(time(NULL));
	string temp = lines[rand()%lines.size()];
	this->wordLength = temp.length();

	encode(temp);
}

void Game::encode(string s) {
    puts("zamierzam enkodowac");
    this->word = new char[this->wordLength + 1];
    this->encoded = new char[this->wordLength + 1];
    
    for(int i = 0; i < this->wordLength; i++) {
        printf("%c\n", s[i]);
		this->word[i] = s[i];
		if(this->word[i] <= 'Z' && this->word[i] >= 'A')
			this->encoded[i] = '_';
	}
    puts("z enkodowane");
}

void Game::addPlayer(Player *player) {
    this->players.insert(make_pair(player->getFd(), player));
}

bool Game::checkPlayersReady() {
    if(this->players.size() > 0) {
        for(map<int, Player*>::iterator it=this->players.begin(); it!=this->players.end(); ++it) {
            if(it->second->isReady() == false) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}