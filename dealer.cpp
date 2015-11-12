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
		//for testing purposes need to see cards
		//
		//if (showCards)
		{
			if (!texture.loadFromFile("images/" + tempCards.at(i).getFace() + tempCards.at(i).getSuit() + ".jpg"))
			{
				throw "Sprite Error";
			}
		}
		//else
		{
			//if (!texture.loadFromFile("images/card2.jpg"))
			{
				//throw "Sprite Error";
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
	info.index.clear();
	info.winningPercentage = 0;
	info.numberOfCardsToReplace = 0;

	//const int NUMBER_OF_TEST_RUNS = 1000;
	Hand testHand/*(getCards())*/; //testHand for finding best move
	Deck testDeck; /* = deck;*/	   //testDeck for finding best move
	Deck testDeck2;
	int  newHandWins = 0;
	double tempPercent = 0;
	int numTestRuns = 0;
	int debugCounter = 0;
	//check what happens if one card is removed from each possible position
	for (int i = 0; i < 5; i++)
	{
		newHandWins = 0;
		testDeck = deck;
		//for (int x = 0; x < i; x++)
		//	testDeck.deal();
		testHand = Hand(getCards());
		
		int temp = testDeck.cardsInDeck();
		for (int j = 0; j < temp; j++)
		{
			numTestRuns++;
			testHand.replaceCard(i, testDeck.deal());
			Dealer tempDealer(testHand,testDeck);
			if (!(this->findWinner(&tempDealer)))
			{
				newHandWins++;
			}
		}

		tempPercent = static_cast<double>(newHandWins) / numTestRuns;

		numTestRuns = 0;
		newHandWins = 0;
		if (tempPercent > info.winningPercentage)
		{
			info.index.clear();
			info.numberOfCardsToReplace = 1;
			info.index.push_back(i);
			info.winningPercentage = tempPercent;
		}
	}  
	//check for two card replacements
	
	for (int first = 0; first < 4; first++) //for first four cards
	{
		std::cout << first << std::endl;
		for (int second = first + 1; second < 5; second++)
		{
			newHandWins = 0;
			numTestRuns = 0;
			testDeck = deck;
			testDeck2 = deck;
			testDeck2.deal();

			for (int j = 0; j < deck.cardsInDeck(); j++) //for number of cards in deck
			{
				
				testHand.replaceCard(first, testDeck.deal());
				for (int p = 0; p < deck.cardsInDeck() - 1; p++)
				{
					numTestRuns++;
					testHand = Hand(getCards());

					testHand.replaceCard(second, testDeck2.deal());

					Dealer tempDealer(testHand,testDeck);
					if (!(this->findWinner(&tempDealer)))
					{
						newHandWins++;
					}
				}
			}
			tempPercent = static_cast<double>(newHandWins) / numTestRuns;

			if (tempPercent > info.winningPercentage)
			{
				info.index.clear();
				info.numberOfCardsToReplace = 2;
				info.index.push_back(first);
				info.index.push_back(second);
				info.winningPercentage = tempPercent;
			}
		}
	}

	//check for three card replacements
	
	

	//after best hand has been decided make changes
	if (info.winningPercentage > .55)
	{
		for (int j = 0; j < info.numberOfCardsToReplace; j++)
		{
			std::cout << "replace" << info.numberOfCardsToReplace << std::endl;
			for (int i = 0; i < 5; i++)
			{
				if (info.index.at(j) != i)
				{
					std::cout << i << std::endl;
					selectCard(i);
					//selectCard(info.index.at(i));
				}
			}
		}
	}
	

}

