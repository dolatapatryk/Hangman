#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <poll.h> 
#include <thread>
#include <unordered_set>
#include <signal.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

char PLAYER_READY = '1';

struct game{
	string word;
	int lifes = 10;
	bool started = false;
};

struct player{
	int fd;
	string name;
	int points = 0;
	int lifes = 10;
	bool ready = false;
};

// server socket
int servFd;
game game;

// client sockets
std::unordered_set<int> clientFds;
std::vector<player> players;

// handles SIGINT
void ctrl_c(int);

// sends data to clientFds excluding fd
void sendToAllBut(int fd, char * buffer, int count);

// converts cstring to port
uint16_t readPort(char * txt);

// sets SO_REUSEADDR
void setReuseAddr(int sock);

void acceptNewConnection();

bool checkPlayersReady();

void readMessage();

int main(int argc, char ** argv){
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
	std::thread acceptThread(acceptNewConnection);

	printf("odpalono serwer\n");
	printf("czekamy na graczy\n");
	
	while(true){
		if(clientFds.size() < 2) {
			//continue;
		}
		
		readMessage();
		
	}	
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
	printf("Closing server\n");
	exit(0);
}

void sendToAllBut(int fd, char * buffer, int count){
	int res;
	decltype(clientFds) bad;
	for(int clientFd : clientFds){
		if(clientFd == fd) continue;
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

void acceptNewConnection() {
	while(true) {
		// prepare placeholders for client address
		sockaddr_in clientAddr{0};
		socklen_t clientAddrSize = sizeof(clientAddr);
		
		// accept new connection
		auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
		if(clientFd == -1) error(1, errno, "accept failed");
		
		// add client to all clients set
		clientFds.insert(clientFd);
		player newPlayer;
		newPlayer.fd = clientFd;
		players.push_back(newPlayer);
		
		// tell who has connected
		printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
	}
}

bool checkPlayersReady() {
	for(player p: players) {
		if(p.ready == false)
			return false;
	}

	return true;
}

// void readMessage() {
// 	int count = 0;
// 	char buffer[255];
// 	for(int clientFd : clientFds) {
// 		count = read(clientFd, buffer, 255);
// 		if(count < 1) {
// 			printf("removing %d\n", clientFd);
// 			clientFds.erase(clientFd);
// 			close(clientFd);
// 			continue;
// 		} else {
// 			//sendToAllBut(clientFd, buffer, count);
// 			if(buffer[0] == '1') {
// 				for(player p : players) {
// 					if(p.fd == clientFd) {
// 						p.ready = true;
// 						printf("player %d gotowy\n", p.fd);
// 						break;
// 					}
// 				}
// 			}
// 		}		
// 	}
// }

void readMessage() {
	int count = 0;
	char buffer[255];
	for(player p : players) {
		count = read(p.fd, buffer, 255);
		if(count < 1) {
			printf("removing %d\n", p.fd);
			clientFds.erase(p.fd);
			//tu jeszcze usuwanie z wektora
			close(p.fd);
			continue;
		} else {
			//sendToAllBut(clientFd, buffer, count);
			if(buffer[0] == PLAYER_READY) {
				p.ready = true;
				printf("player %d gotowy\n", p.fd);
			}
		}		
	}
}