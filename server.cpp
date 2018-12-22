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
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include <poll.h>
#include <set>
#include <functional>
#include <sstream>
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

string pathToWords = "./words";

// server socket
int servFd;
//struct gameProperties game;
Game *game = new Game();

int fdsCounter = 1;

// client sockets
std::unordered_set<int> clientFds;
pollfd whatToWait[MAX_PLAYERS + 1] {};

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
	
	//watek przyjmujacy nowe polaczenia
	std::thread readThread(readPoll);
	whatToWait[0].fd = servFd;
	whatToWait[0].events = POLLIN;
	printf("odpalono serwer\n");
	printf("czekamy na graczy\n");
	
	while(true){
		if(game->getPlayers().size() < 1) {
			//continue;
		}
		
		if(game->checkPlayersReady()) {
			puts("gracze gotowi");
			game->setStarted(true);
			puts("started");
			game->makeWord();
			puts("zrobione haslo");
			printf("haslo: %s\n", game->getWord());
			sendWordToAll();
			sendRanking();
			while(true) {
				if(game->getLifes() == 0) {
					puts("przegrana");
					game->setStarted(false);
					//tu jeszcze ze wszyscy gracze niegotowi
				}
				if(game->getWord() == game->getEncoded()) {
					puts("wygrana");
					game->setStarted(false);
					//tu jeszcze ze wszyscy niegotowi
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
		printf("removing %d\n", fd);
		clientFds.erase(fd);
		map<int, Player*>::iterator it;
		it = game->getPlayers().find(fd);
		game->getPlayers().erase(it);
		close(fd);
	} else {
		if(buffer[0] == PLAYER_READY) {
			puts("gotowy");
			game->getPlayers().find(fd)->second->setReady(true);
		}		
	}
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