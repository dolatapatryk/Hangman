class Game {
    public:
        Game();
        ~Game();
        char[WORD_LENGTH] getWord();
        void setWord(char[WORD_LENGTH] c);
        char[WORD_LENGTH] getEncoded();
        void setWord(char[WORD_LENGTH] c);
        int getWordLength();
        void setWordLength(int n);
        int getLifes();
        void setLifes(int n);
        bool isStarted();
        void setStarted(bool b);
        const WORD_LENGTH = 50;
        const int LIFES = 10;
    private:
        char word[WORD_LENGTH];
	    char encoded[WORD_LENGTH];
	    int wordLength;
	    int lifes;
	    bool started;
};