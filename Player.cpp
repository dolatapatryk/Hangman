#include "Player.h"

Player::Player() {}

Player::Player(int n) {
    this->fd = n;
    lifes = LIFES;
    points = 0;
    ready = false;
}

Player::~Player() {}

int Player::getFd() {
    return this->fd;
}

void Player::setFd(int n) {
    fd = n;
}

int Player::getLifes() {
    return lifes;
}

void Player::setLifes(int n) {
    lifes = n;
}

int Player::getPoints() {
    return points;
}

void Player::setPoints(int n) {
    points = n;
}

bool Player::isReady() {
    return ready;
}

void Player::setReady(bool b) {
    ready = b;
}

void Player::addPoints(int points) {
    this->points += points;
}

void Player::subtractPoints(int points) {
    this-> points -= points;
}

void Player::loseLife() {
    this->lifes -= 1;
}