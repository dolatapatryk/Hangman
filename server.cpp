#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <thread>
#include <unordered_set>
#include <signal.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <poll.h>
#include <time.h>
#include "Game.h"

using namespace std;

#define BUFFOR_LENGTH 50
#define LIFES 10
#define MAX_PLAYERS 16

char PLAYER_READY = '1';
string SEMICOLON = ";";
string GAME_STARTED = "1";
string SEND_FD_TO_PLAYER = "2";
string SEND_RANKING = "3";
string GAME_ENDED = "0";
string GAME_WIN = "1";
string GAME_LOSS = "0";

string pathToWords = "./words";

// server socket
int servFd;
//struct gameProperties game;
Game *game = new Game();

int fdsCounter = 1;

// client sockets
std::unordered_set<int> clientFds;
pollfd whatToWait[MAX_PLAYERS + 1] {};
map<char, map<int, long>> lettersSent;
map<char, map<int, bool>> confirmationAboutDisablingLetter;

// handles SIGINT
void ctrl_c(int);

// converts cstring to port
uint16_t readPort(char * txt);

// sets SO_REUSEADDR
void setReuseAddr(int sock);

void acceptNewConnection();

bool checkPlayersReady();

void readMessage(int fd);

void sendToAll(char *buffer, int count);

void send(int fd, char *buffer, int count);

void sendWordToAll();

void sendFdToPlayer(int clientFd);

void sendRanking();

void readPoll();

void removePlayer(int clientFd);

void setPlayerReady(int clientFd);

void handleLetter(char letter, int clientFd);

void getLetterSendTime(char * buffer, int clientFd);

void setConfirmationAboutDisablingLetter(char letter, int clientFd);

bool checkIfEachPlayerDisableButton(char letter);

int checkWhoWasFirst(char letter);

void sendWordAndRanking();

void sendLetterWordRanking(char letter);

void sendEndGameAndWinOrLoss(bool win);

int main(int argc, char ** argv){
	game->setStarted(false);
	// get and validate port number
	if(argc != 2) error(1, 0, "Need 1 arg (port)");
	auto port = readPort(argv[1]);
	
	// create socket
	servFd = socket(AF_INET, SOCK_STREAM, 0);
	if(servFd == -1) error(1, errno, "socket failed");
	
	// graceful ctrl+c exit
	signal(SIGINT, ctrl_c);
	// prevent dead sockets from throwing pipe errors on write
	signal(SIGPIPE, SIG_IGN);
	
	setReuseAddr(servFd);
	
	// bind to any address and port provided in arguments
	sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
	int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
	if(res) error(1, errno, "bind failed");
	
	// enter listening mode
	res = listen(servFd, 1);
	if(res) error(1, errno, "listen failed");
	
	std::thread readThread(readPoll);
	whatToWait[0].fd = servFd;
	whatToWait[0].events = POLLIN;
	printf("odpalono serwer\n");
	printf("czekamy na graczy\n");
	
	while(true){
		if(game->getPlayers().size() < 2) {
			continue;
		}
		
		if(game->checkPlayersReady()) {
			puts("gracze gotowi");
			game->setStarted(true);
			lettersSent.clear();
			puts("started");
			game->makeWord();
			puts("zrobione haslo");
			printf("haslo: %s\n", game->getWord());
			sendWordAndRanking();
			while(game->isStarted()) {
				if(game->getLifes() == 0) {
					puts("przegrana");
					game->endGame();
					sendEndGameAndWinOrLoss(false);
					break;
				}
				if(game->compareWordAndEncoded()) {
					puts("wygrana");
					game->endGame();
					sendEndGameAndWinOrLoss(true);
					break;
				}
			}
		} else {
			continue;
		}
		
	}

	puts("koniec");	
	
	exit(0);
}


//definicje metod
uint16_t readPort(char * txt){
	char * ptr;
	auto port = strtol(txt, &ptr, 10);
	if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
	return port;
}

void setReuseAddr(int sock){
	const int one = 1;
	int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	if(res) error(1,errno, "setsockopt failed");
}

void ctrl_c(int){
	for(int clientFd : clientFds)
		close(clientFd);
	close(servFd);
	delete game;
	printf("Closing server\n");
	exit(0);
}

void sendToAll(char * buffer, int count){
	int res;
	decltype(clientFds) bad;
	for(int clientFd : clientFds){
		res = write(clientFd, buffer, count);
		if(res!=count)
			bad.insert(clientFd);
	}
	for(int clientFd : bad){
		printf("removing %d\n", clientFd);
		clientFds.erase(clientFd);
		close(clientFd);
	}
}

void send(int fd, char * buffer, int count){
	int res;
	res = write(fd, buffer, count);
	if(res != count) {
		printf("removing %d\n", fd);
		clientFds.erase(fd);
		close(fd);
	}
}

void acceptNewConnection() {
		// prepare placeholders for client address
		sockaddr_in clientAddr{0};
		socklen_t clientAddrSize = sizeof(clientAddr);
		
		// accept new connection
		auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
		if(clientFd == -1) error(1, errno, "accept failed");
		
		// add client to all clients set
		clientFds.insert(clientFd);
		whatToWait[fdsCounter].fd = clientFd;
		whatToWait[fdsCounter].events = POLLIN;
		fdsCounter++;

		Player *newPlayer = new Player(clientFd);
		game->addPlayer(newPlayer);
		sendFdToPlayer(clientFd);
		sendRanking();
		
		printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
}

void readPoll() {
	while(true) {
		poll(whatToWait, MAX_PLAYERS + 1, -1);
		for(int i = 0; i < fdsCounter; i++) {
			if(whatToWait[i].revents & POLLIN) {
				if(whatToWait[i].fd == servFd) {
					acceptNewConnection();
				} else {
					readMessage(whatToWait[i].fd);
				}
			}
		}
	}
}

void readMessage(int fd) {
	char buffer[255];
	int count = read(fd, buffer, 255);
	if(count < 1) {
		removePlayer(fd);
	} else {
		printf("buffer: %s\n", buffer);
		if(buffer[0] == PLAYER_READY) {
			setPlayerReady(fd);
			sendRanking();
		} else if (buffer[0] >= 'A' && buffer[0] <= 'Z' && game->isStarted()) {
			puts("dostalem se jakas literke, jescze nie mam jej czasu\n");
			getLetterSendTime(buffer, fd);
			handleLetter(buffer[0], fd);
		} 
	}
}

void removePlayer(int clientFd) {
	printf("removing %d\n", clientFd);
	clientFds.erase(clientFd);
	map<int, Player*>::iterator it;
	it = game->getPlayers().find(clientFd);
	game->getPlayers().erase(it);
	close(clientFd);
	if(game->getPlayers().size() < 2) {
		game->setStarted(false);
	}
}

void setPlayerReady(int clientFd) {
	puts("gotowy\n");
	game->getPlayers().find(clientFd)->second->setReady(true);
}

void handleLetter(char letter, int clientFd) {
	printf("dostałem literke: %c\n", letter);
	int points = game->decode(letter);
	int fd = checkWhoWasFirst(letter);
	if(points == 0) {
		game->getPlayers().find(fd)->second->loseLife();
	} else {
		game->getPlayers().find(fd)->second->addPoints(points);
	}
	if(game->isStarted())
		sendLetterWordRanking(letter);
}

void sendWordToAll() {
	string wordLengthString = to_string(game->getWordLength());
	int length = game->getWordLength() + 2 + to_string(wordLengthString.length()).length();
	char buffer[length];

	strcpy(buffer, GAME_STARTED.c_str());
	strcat(buffer, wordLengthString.c_str());
	strcat(buffer, SEMICOLON.c_str());
	for(int i = 0; i < game->getWordLength(); i++) {
		buffer[i+2+wordLengthString.length()] = game->getEncoded()[i];
	}
	printf("wiadomosc: %s\n", buffer);
	sendToAll(buffer, length);
}

void sendFdToPlayer(int clientFd) {
	string fdString = to_string(clientFd);
	int length = fdString.length() + 2 + to_string(fdString.length()).length();
	char buffer[length];
	strcpy(buffer, SEND_FD_TO_PLAYER.c_str());
	strcat(buffer, to_string(fdString.length()).c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, fdString.c_str());
	send(clientFd, buffer, length);
}

void sendRanking() {
	string ranking = game->makeRanking();
	int length = ranking.length() + 2 + to_string(ranking.length()).length();
	char buffer[length];
	strcpy(buffer, SEND_RANKING.c_str());
	strcat(buffer, to_string(ranking.length()).c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, ranking.c_str());
	sendToAll(buffer, length);
}

void sendWordAndRanking() {
	string wordLengthString = to_string(game->getWordLength());
	string ranking = game->makeRanking();
	int length = game->getWordLength() + 2 + to_string(wordLengthString.length()).length();
	length += ranking.length() + 2 + to_string(ranking.length()).length();
	char buffer[length];

	strcpy(buffer, GAME_STARTED.c_str());
	strcat(buffer, wordLengthString.c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, game->getEncoded());
	strcat(buffer, SEND_RANKING.c_str());
	strcat(buffer, to_string(ranking.length()).c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, ranking.c_str());
	printf("wiadomosc: %s\n", buffer);
	sendToAll(buffer, length);
}

void sendLetterWordRanking(char letter) {
	string wordLengthString = to_string(game->getWordLength());
	string ranking = game->makeRanking();
	int length = game->getWordLength() + 2 + to_string(wordLengthString.length()).length();
	length += ranking.length() + 2 + to_string(ranking.length()).length() + 1;
	char buffer[length];

	string letterString(1,letter);
	strcpy(buffer, letterString.c_str());
	strcat(buffer, GAME_STARTED.c_str());
	strcat(buffer, wordLengthString.c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, game->getEncoded());
	strcat(buffer, SEND_RANKING.c_str());
	strcat(buffer, to_string(ranking.length()).c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, ranking.c_str());
	printf("wiadomosc: %s\n", buffer);
	sendToAll(buffer, length);
}

void getLetterSendTime(char * buffer, int clientFd) {
	char letter = buffer[0];
	char *timeChar = new char[20];
	int i = 1;
	while(buffer[i] != ';') {
		timeChar[i-1] = buffer[i];
		i++;
	}
	map<char, map<int, long>>::iterator it = lettersSent.find(letter);
	if(it != lettersSent.end()) {
		it->second.insert(make_pair(clientFd, atol(timeChar)));
	} else {
		map<int, long> clientsWithTimes;
		clientsWithTimes.insert(make_pair(clientFd, atol(timeChar)));
		lettersSent.insert(make_pair(letter, clientsWithTimes));
	}

	// map<char, map<int, bool>>::iterator iter = confirmationAboutDisablingLetter.find(letter);
	// if(iter != confirmationAboutDisablingLetter.end()) {
	// 	iter->second.insert(make_pair(clientFd, false));
	// } else {
	// 	map<int, bool> clientsWithConfirmation;
	// 	clientsWithConfirmation.insert(make_pair(clientFd, false));
	// 	confirmationAboutDisablingLetter.insert(make_pair(letter, clientsWithConfirmation));
	// }

}

void setConfirmationAboutDisablingLetter(char letter, int clientFd) {
	puts("aktualizuje potwierdzenie literki\n");
	confirmationAboutDisablingLetter.find(letter)->second[clientFd] = true;
}

bool checkIfEachPlayerDisableButton(char letter) {
	while(true){
		bool check = true;
		map<char, map<int, bool>>::iterator it = confirmationAboutDisablingLetter.find(letter);
		for(map<int, bool>::iterator iter=it->second.begin(); iter!=it->second.end(); ++iter) {
			if(iter->second == false){
				check = false;
				break;
			}
		}
		if(check == true)
			break;
	}
	return true;
}

int checkWhoWasFirst(char letter) {
	int fd = 0;
	map<char, map<int, long>>::iterator it = lettersSent.find(letter);
	long smallestTime = __LONG_MAX__;
	for(map<int, long>::iterator iter=it->second.begin(); iter!=it->second.end(); ++iter) {
		if(iter->second < smallestTime) {
			smallestTime = iter->second;
			fd = iter->first;
		}
	}
	return fd;
}

void sendEndGameAndWinOrLoss(bool win) {
	string ranking = game->makeRanking();
	int length = ranking.length() + 4 + to_string(ranking.length()).length();
	char buffer[length];
	strcpy(buffer, GAME_ENDED.c_str());
	if(win)
		strcat(buffer, GAME_WIN.c_str());
	else
		strcat(buffer, GAME_LOSS.c_str());
	strcat(buffer, SEND_RANKING.c_str());
	strcat(buffer, to_string(ranking.length()).c_str());
	strcat(buffer, SEMICOLON.c_str());
	strcat(buffer, ranking.c_str());
	printf("wiadomosc: %s\n", buffer);
	sendToAll(buffer, length);
}