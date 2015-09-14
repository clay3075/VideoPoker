//player.hpp
//Player class specification file


#ifndef PLAYER_H
#define PLAYER_H

#include <string>
	using std::string;

class Player
{
protected:
	int score; //will hold player score in this case used to hold players cash balance
	string name;  //will hold player name
public:	
	Player(){};
	Player(string nme); //constructor will initialize name and set score to 0
	void setScore(int scr = 1500); //accepts a floating point number to set to score value
	int getScore(); //returns players current score
	void calcScore() //to be overloaded
		{return;}
	string getName(); //returns players name
	void setName(string nme = "") //takes string as argument and saves string to name member variable
		{name = nme;}

};

#endif