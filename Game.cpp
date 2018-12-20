#include "Game.h"

Game::Game() {
    wordLength = 0;
    lifes = LIFES;
    started = false;
}

Game::~Game() {}

char[WORD_LENGTH] Game::getWord() {
    return word;
}

void Game::setWord(char c[WORD_LENGTH]) {
    word = c;
}

char[WORD_LENGTH] Game::getEncoded() {
    return encoded;
}

void Game::setEncoded(char[WORD_LENGTH] c) {
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