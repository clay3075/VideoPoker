/*dealer.cpp
 *implementation file
 *
 *@remarks
 *
 *@author Clay Reddick
 *@date   09/10/15
 */

#include "dealer.hpp"
#include <iostream>
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

//will calculate chances of winning and adjust strategy of hand based on it
void Dealer::decideHand()
{
	const int NUMBER_OF_TEST_RUNS = 1000;
	Hand testHand(getCards()); //testHand for finding best move
	Deck testDeck = deck; 	   //testDeck for finding best move
	int  newHandWins = 0;
	double tempPercent = 0;
	
	//check what happens if one card is removed from each possible position
	for (int i = 0; i < 5; i++)
	{
		//test over 1000 deals for each new card
		for (int j = 0; j < NUMBER_OF_TEST_RUNS; j++)
		{
			testHand.replaceCard(i, testDeck.deal());
			Dealer tempDealer(testHand,testDeck); 
			if (!(this->findWinner(&tempDealer)))
			{
				newHandWins++;
			}
			testHand = Hand(getCards());
			testDeck = deck;
		}
		tempPercent = newHandWins / NUMBER_OF_TEST_RUNS;
		newHandWins = 0;
		if (tempPercent > info.winningPercentage)
		{
			info.index.clear();
			info.numberOfCardsToReplace = 1;
			info.index.push_back(i);
			info.winningPercentage = tempPercent;
		}

	}

	//after best hand has been decided make changes
	if (info.winningPercentage > .55)
	{
		int index1 = -1, index2 = -1, index3 = -1;
		if (info.index.size() == 1)
		{
			index1 = info.index.at(0);
		}
		else if (info.index.size() == 2)
		{
			index2 = info.index.at(1);
		}
		else
		{
			index3 = info.index.at(2);
		}

		for (int i = 0; i < 5; i++)
		{
			if (i != index1 && i != index2 && i != index3)
			{
				selectCard(i);
			}
		}
	}
	std::cout << info.winningPercentage << std::endl;

}

