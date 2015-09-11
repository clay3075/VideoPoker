videopoker : main.o game.o dealer.o pokerplayer.o player.o hand.o deck.o card.o
	g++ -o videopoker -Wall -Wextra -std=c++11 -pedantic -lsfml-graphics -lsfml-window -lsfml-system main.o game. dealer.o pokerplayer.o player.o hand.o deck.o card.o
card.o : card.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c card.cpp
deck.o : deck.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c deck.cpp
hand.o : hand.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c hand.cpp
player.o : player.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c player.cpp
pokerplayer.o : pokerplayer.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c pokerplayer.cpp
dealer.o : dealer.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c dealer.cpp
game.o : game.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c game.cpp
main.o : main.cpp
	g++ -Wall -Wextra -std=c++11 -pedantic -c main.cpp