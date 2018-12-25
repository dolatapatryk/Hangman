#include "Game.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <functional>
#include <iostream>

Game::Game() {
    wordLength = 0;
    lifes = LIFES;
    started = false;
}

Game::~Game() {
    for(map<int, Player*>::iterator it=this->players.begin(); it!=this->players.end(); ++it) {
        delete it->second;
    }
    this->players.clear();
}

string Game::getWord() {
    return this->word;
}

void Game::setWord(string s) {
    this->word = s;
}

string Game::getWordForPlayer() {
    return this->wordForPlayer;
}

void Game::setWordForPlayer(string s) {
    this->wordForPlayer = s;
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
    cout<<"słowo z pliku: "<<temp<<flush;
	this->wordLength = temp.length();
    printf("game.cpp word length: %d\n", this->wordLength);
	prepareWordForPlayer(temp);
}

void Game::prepareWordForPlayer(string s) {
    this->word = s;
    this->wordForPlayer = s;
    for(int i = 0; i < this->wordLength; i++) {
        printf("%c\n", s[i]);
		this->word[i] = s[i];
		if(this->word[i] <= 'Z' && this->word[i] >= 'A')
			this->wordForPlayer[i] = '_';
	}
    cout<<"game.cpp word: "<<this->word<<" encoded: "<<this->wordForPlayer<<endl<<flush;
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

string Game::makeRanking() {
    typedef function<bool(pair<int, Player*>, pair<int, Player*>)> Comparator;

    Comparator comparator = 
        [](pair<int, Player*> elem1, pair<int, Player*> elem2) {
            if(elem1.second->getPoints() == elem2.second->getPoints())
                return elem1.first < elem2.first;
            else
                return elem1.second->getPoints() > elem2.second->getPoints();
        };
    set<pair<int, Player*>, Comparator> setOfPlayers(this->players.begin(), this->players.end(), comparator);

    stringstream ss;
    int i = 1;
    for(pair<int, Player*> player : setOfPlayers) {
        ss << to_string(i) << ". PLAYER " << to_string(player.first) << " - " << to_string(player.second->getPoints())
            << " p - ";
        if(player.second->isReady())
            ss<<"READY\n";
        else
            ss<<"UNREADY\n";
        i++;
    }
    return ss.str();
}

void Game::setAllPlayersUnready() {
    for(map<int, Player*>::iterator it=this->players.begin(); it!=this->players.end(); ++it) {
        it->second->setReady(false);
    }
}

void Game::endGame() {
    this->started = false;
    this->lifes = LIFES;
    this->wordLength = 0;
    setAllPlayersUnready();
}

int Game::calculatePoints(char c) {
    int points = 0;
    bool present = false;
    for(int i = 0; i < this->wordLength; i++) {
        if(this->word[i] == c) {
            points++;
            this->wordForPlayer[i] = c;
            present = true;
        }
    }

    if(!present)
        this->lifes--;
    printf("zycia: %d\n", this->lifes);
    
    return points;
}

bool Game::compareWordAndWordForPlayer() {
    if((this->word.compare(this->wordForPlayer)) == 0)
        return true;
    else
        return false;
}

bool Game::checkIfPlayerIsReady(int clientFd) {
    return this->players.find(clientFd)->second->isReady();
}

void Game::setPlayerReady(int clientFd) {
    this->players.find(clientFd)->second->setReady(true);
}