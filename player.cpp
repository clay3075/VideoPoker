//player.cpp
//Player class implementation file

#include "player.hpp"
#include <string>
	using std::string;

/*constructor will initialize name and set score to 0
 */
Player::Player(string nme)
{
	name = nme;
	score = 0;
}

/* Method used to set players score.
 * if negative value a string exception is thrown
 *@param scr The new score
 */
void Player::setScore(int scr)
{	
	if (scr >= 0)					
	{								
		score = scr;				
	}								
	else
	{
		string exception = "Negative Value";
		throw exception;
	}
}

/* Method to return players current score
 */
int Player::getScore()
{
	return score;
}

/* Method to return players name
 */
string Player::getName()
{
	return name;
}