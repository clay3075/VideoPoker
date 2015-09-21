/*dealer.cpp
 *implementation file
 *
 *@remarks
 *
 *@author Clay Reddick
 *@date   09/10/15
 */

#include "dealer.hpp"

//constructor will initialize all values needed
Dealer::Dealer()
{
	setName("Dealer");
	shuffle();
}

//will randomize cards in deck by calling the Deck class shuffle() function
void Dealer::shuffle()
{
	deck.shuffle();
}

//will return a Card object from the "top" of the deck object
Card Dealer::deal()
{
	return deck.deal();
}  

//shows hand to screen
void Dealer::show(sf::Sprite sprite, sf::RenderWindow* window, sf::Texture& texture, Dealer* player, bool showCards)
{
	vector<Card> tempCards;
	tempCards = player->getCards();
	//draw cards to screen
	for (unsigned int i = 0; i < player->getNumberOfCardsInHand(); i++) //for each card in hand
	{
		if (showCards)
		{
			if (!texture.loadFromFile("images/" + tempCards.at(i).getFace() + tempCards.at(i).getSuit() + ".jpg"))
			{
				throw "Sprite Error";
			}
		}
		else
		{
			if (!texture.loadFromFile("images/card2.jpg"))
			{
				throw "Sprite Error";
			}
		}
		//
		//
		sprite.setTexture(texture); //set image to sprite
		sprite.setScale(sf::Vector2f(3.0f, 3.0f)); //make card bigger //this is scale needed
		sprite.setPosition(sf::Vector2f(75.0f * (3.5 * i+2), 100.0f)); //this is position needed 
		window->draw(sprite); //show sprite to screen
	}
}

