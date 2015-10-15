/* pokerplayer.cpp
 * implementation file for PokerPlayer class
 *
 * @author Clay Reddick
 */

#include "pokerplayer.hpp"
#include <iostream>
//will null the moneyBet variable and change betPlaced to false
void PokerPlayer::clearBet()
{
	moneyBet = 0;
	betPlaced = false;
}

//will return true if player has put in a bet false otherwise
bool PokerPlayer::isBetPlaced()
{
	return betPlaced;
}
//will be used to keep score in a valid state as influences are made on it
void PokerPlayer::calcScore(bool wonOrLost)
{
	bool won = true;
	if (wonOrLost == won)
	{
		score += moneyBet;
	}
	else
	{
		score -= moneyBet;
	}
	return;
} 

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

//will be used to place a bet
void PokerPlayer::placeBet(const int betAmount)
{
	if (betAmount >= 0)
	{
		moneyBet += betAmount;
		betPlaced = true;
	}
	return;
}

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
		else
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


//shows hand to screen
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