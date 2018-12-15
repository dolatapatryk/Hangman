#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <sys/shm.h>
#include <error.h>
#include <string.h>
#include <unordered_set>

using namespace std;

int serverSocket;

unordered_set<int> clientFds;

typedef struct {

}game;

// sends data to clientFds excluding fd
void sendToAllBut(int fd, char * buffer, int count);

// converts cstring to port
uint16_t readPort(char * txt);

void setReuseAddr(int sock);

int main(int argc , char *argv[]) {

    
    char *buf;
    auto port = readPort(argv[1]);
    struct sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    struct sockaddr_in client;

    //tworzenie socketa
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    setReuseAddr(serverSocket);

    if(bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        perror(nullptr);
        exit(1);
    } else {
        puts("bind dziala");
    }

    listen(serverSocket, 1);

    while(1) {
        // prepare placeholders for client address
		struct sockaddr_in clientAddr{0};
		socklen_t clientAddrSize = sizeof(clientAddr);
		
		// accept new connection
		auto clientFd = accept(serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
		if(clientFd == -1) error(1, errno, "accept failed");
		
		// add client to all clients set
		clientFds.insert(clientFd);
		
		// tell who has connected
		printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
		
/****************************/
		
		// read a message
		char buffer[255];
		int count = read(clientFd, buffer, 255);
		
		if(count < 1) {
			printf("removing %d\n", clientFd);
			clientFds.erase(clientFd);
			close(clientFd);
			continue;
		} else {
			// broadcast the message
			sendToAllBut(clientFd, buffer, count);
		}
    }

}

uint16_t readPort(char * txt){
	char * ptr;
	auto port = strtol(txt, &ptr, 10);
	if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
	return port;
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

void setReuseAddr(int sock){
	const int one = 1;
	int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	if(res) error(1,errno, "setsockopt failed");
}

