/* pokerplayer.cpp
 * implementation file for PokerPlayer class
 *
 * @author Clay Reddick
 */

#include "pokerplayer.hpp"
#include <iostream>

/* Method used to reset all attributes related to betting.
 */
void PokerPlayer::clearBet()
{
	moneyBet = 0;
	betPlaced = false;
}

/* Method used to check if player has made a bet.
 * returns true if player has made bet false otherwise
 */
bool PokerPlayer::isBetPlaced()
{
	return betPlaced;
}

/* Method used to keep score in a valid state as influences are made on it.
 *@param wonOrLost If this is true then player will gain the amount of money he has bet
 *                 if it is false he will lose.
 */
void PokerPlayer::calcScore(bool wonOrLost)
{
	bool won = true;
	if (wonOrLost == won) //if player won increase score
	{
		score += moneyBet;
	}
	else //if player lost decrease score
	{
		score -= moneyBet;
	}
	return;
} 

/* Constructor used to initialize player.
 *@param score The starting score the player will start with (must be positive)
 */
PokerPlayer::PokerPlayer(int score) : Player(), Hand()
{
	betPlaced = false;
	if (score > 0)
	{
		this->score = score;
	}
	else
	{
		throw "Invalid score entry";
	}
}

/* Method to allow player to place a bet as long as they have enough money
 *@param betAmount The amount of money to bet
 */
void PokerPlayer::placeBet(const int betAmount)
{
	//if player has enough money make bet
	if (betAmount > 0 && (betAmount + moneyBet) <= score)
	{
		moneyBet += betAmount;
		betPlaced = true;
	}
	return;
}

/* Method used to compare two players hands and determine who the 
 * winner is.
 *@param otherPlayer The player to be compared to
 *@return Will return true if this player won or false otherwise
 */
bool PokerPlayer::findWinner(PokerPlayer* otherPlayer)
{
	bool thisPlayerWon = true;

	if (otherPlayer->getWorth() > this->getWorth())
		thisPlayerWon = false;
	if (this->getWorth() == otherPlayer->getWorth()) //if players have same hand
	{
		//see which pair is higher
		if (otherPlayer->getPairValue() > this->getPairValue())
		{
			thisPlayerWon = false;
		}
		//if both players had two pair
		else if ((otherPlayer->getPairValue() > this->getPairValue()) && otherPlayer->checkForSecondPairandValue() > this->checkForSecondPairandValue())
		{
			thisPlayerWon = false;
		}
		else //if players dont have pairs check for highes card
		{
			//check for higher card
			//if players have same high card check next highest card and so on
			int i = getNumberOfCardsInHand();
			do
			{
				i--;
				if (otherPlayer->getCardValue(i) > this->getCardValue(i))
				{
					thisPlayerWon = false;
				}
			} while(i > 0 && (this->getCardValue(i) == otherPlayer->getCardValue(i)));
		}
	}

	return thisPlayerWon;
}

/* Method used to render the players cards to the screen.
 *@param 	  sprite  The sprite that is to be rendered
 *@param 	  window  The window to render to
 *@param[out] texture The picture to be shown on the sprite
 *@param      player  The player whos cards are to be shown
 */
void PokerPlayer::show(sf::Sprite* sprite, sf::RenderWindow* window, sf::Texture& texture, PokerPlayer* player)
{
	vector<Card> tempCards;
	tempCards = player->getCards();
	//draw cards to screen
	for (unsigned int i = 0; i < player->getNumberOfCardsInHand(); i++) //for each card in hand
	{
		if (!texture.loadFromFile("images/" + tempCards.at(i).getFace() + tempCards.at(i).getSuit() + ".jpg"))
		{
			throw "Sprite Error";
		}
		//
		//
		sprite[i].setTexture(texture); //set image to sprite
		sprite[i].setScale(sf::Vector2f(3.0f, 3.0f)); //make card bigger //this is scale needed
		sprite[i].setPosition(sf::Vector2f(75.0f * (3.5 * i+2), 600.0f)); //this is position needed 
		window->draw(sprite[i]); //show sprite to screen
	}
	//
	//
	return;
}