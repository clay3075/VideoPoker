/* pokerplayer.cpp
 * implementation file for PokerPlayer class
 *
 * @author Clay Reddick
 */

#include "pokerplayer.hpp"

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
	}
	return;
}

//shows hand to screen
void PokerPlayer::show(sf::Sprite& sprite, sf::RenderWindow* window, sf::Texture& texture, PokerPlayer* player)
{
	vector<Card> tempCards;
	tempCards = player->getCards();
	//draw cards to screen
	for (unsigned int i = 0; i < player->getNumberOfCardsInDeck(); i++) //for each card in hand
	{
		if (!texture.loadFromFile("images/" + tempCards.at(i).getFace() + tempCards.at(i).getSuit() + ".jpg"))
		{
			throw "Sprite Error";
		}
		//
		//
		sprite.setTexture(texture); //set image to sprite
		sprite.setScale(sf::Vector2f(2.5f, 2.5f)); //make card bigger //this is scale needed
		sprite.setPosition(sf::Vector2f(70.0f * (i+2), 600.0f)); //this is position needed 
		window->draw(sprite); //show sprite to screen
	}
	//
	//
	return;
}