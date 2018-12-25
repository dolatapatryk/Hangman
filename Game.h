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
        string getWordForPlayer();
        void setWordForPlayer(string s);
        int getWordLength();
        void setWordLength(int n);
        int getLifes();
        void setLifes(int n);
        bool isStarted();
        void setStarted(bool b);
        map<int, Player*> getPlayers();
        void makeWord();
        void prepareWordForPlayer(string s);
        void addPlayer(Player *player);
        void removePlayer(int clientFd);
        bool checkPlayersReady();
        void setPlayerReady(int clientFd);
        void setAllPlayersUnready();
        string makeRanking();
        void endGame();
        int calculatePoints(char c);
        bool compareWordAndWordForPlayer();
        bool checkIfPlayerIsReady(int clientFd);
        void setPlayersLifes();
        void newGame();
        const int LIFES = 9;
        string pathToWords = "./words";
    private:
        string word;
	    string wordForPlayer;
	    int wordLength;
	    int lifes;
	    bool started;
        map<int, Player*> players;
};
