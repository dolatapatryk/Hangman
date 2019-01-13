# Hangman

Server-client application to play Hangman!

Run instruction:
 - to compile server go to directory "Serwer" and type in terminal:
      g++ -Wall -pthread server.cpp Game.cpp Player.cpp -o server
 - to run server type in terminal:
      ./server [port] (for example: ./server 1234)
 - to run client go to directory "build-Hangman-Desktop-Debug" and then type:
      ./Hangman
