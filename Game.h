#include <string>
#include <map>
#include "Player.h"

using namespace std;

class Game {
    public:
        Game();
        ~Game();
        char* getWord();
        void setWord(char* c);
        char* getEncoded();
        void setEncoded(char* c);
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
        const int LIFES = 10;
        string pathToWords = "./words";
    private:
        char* word;
	    char* encoded;
	    int wordLength;
	    int lifes;
	    bool started;
        map<int, Player*> players;
};
