#include <string>
#include <map>
#include "Player.h"

using namespace std;

class Game {
    public:
        Game();
        ~Game();
        string getWord();
        void setWord(string s);
        string getEncoded();
        void setEncoded(string s);
        int getWordLength();
        void setWordLength(int n);
        int getLifes();
        void setLifes(int n);
        bool isStarted();
        void setStarted(bool b);
        map<int, Player*> getPlayers();
        void makeWord();
        void encode(string s);
        void addPlayer(Player *player);
        bool checkPlayersReady();
        void setPlayerReady(int clientFd);
        void setAllPlayersUnready();
        string makeRanking();
        void endGame();
        int calculatePoints(char c);
        bool compareWordAndEncoded();
        bool checkIfPlayerIsReady(int clientFd);
        const int LIFES = 10;
        string pathToWords = "./words";
    private:
        string word;
	    string encoded;
	    int wordLength;
	    int lifes;
	    bool started;
        map<int, Player*> players;
};
