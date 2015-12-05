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
				//throw "Sprite Error";
			}
		}
		
		
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
	Hand oldHand;
	Deck testDeck; /* = deck;*/	   //testDeck for finding best move
	Deck testDeck2;
	Deck testDeck3;
	int  newHandWins = 0;
	double tempPercent = 0;
	int numTestRuns = 0;
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
		for (int second = first + 1; second < 5; second++)
		{
			newHandWins = 0;
			numTestRuns = 0;
			testDeck = deck;
			testDeck2 = deck;
			testDeck2.deal();

			for (int j = 0; j < deck.cardsInDeck(); j++) //for number of cards in deck
			{
				testHand = Hand(getCards());
				testHand.replaceCard(first, testDeck.deal());
				oldHand = testHand;
				for (int p = 0; p < deck.cardsInDeck() - 1; p++)
				{
					numTestRuns++;

					testHand.replaceCard(second, testDeck2.deal());

					Dealer tempDealer(testHand,testDeck);
					if (!(this->findWinner(&tempDealer)))
					{
						newHandWins++;

					}
					testHand = oldHand;
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
	numTestRuns = 0;
	//check for three card replacements
	for (int first = 0; first < 5; first++)
	{	
		bool secondPassed = false;
		for (int second = first + 1; second < 5 && !secondPassed; second++)
		{
			if (first == 4)
			{
				second = 0;
				secondPassed = true;
			}
			for (int third = second + 1; (third < 5 && second != 4 && !secondPassed) || (secondPassed && third < first); third++)
			{
				newHandWins = 0;
				numTestRuns = 0;
				testDeck  = deck;
				testDeck2 = deck;
				testDeck3 = deck;
				testDeck2.deal();
				testDeck3.deal();
				testDeck3.deal();

				for (int j = 0; j < deck.cardsInDeck(); j++) //for number of cards in deck
				{
					testHand = Hand(getCards());
					testHand.replaceCard(first, testDeck.deal());
					oldHand = testHand;
					for (int p = 0; p < deck.cardsInDeck() - 1; p++)
					{
						testHand.replaceCard(second, testDeck2.deal());
						oldHand = testHand;
						for (int t = 0; t < deck.cardsInDeck() - 2; t++)
						{
							numTestRuns++;

							testHand.replaceCard(third, testDeck3.deal());

							Dealer tempDealer(testHand,testDeck);
							if (!(this->findWinner(&tempDealer)))
							{
								newHandWins++;
							}
							testHand = oldHand;
						}
					}
				}

				tempPercent = static_cast<double>(newHandWins) / numTestRuns;


				if (tempPercent > info.winningPercentage)
				{
					info.index.clear();
					info.numberOfCardsToReplace = 3;
					info.index.push_back(first);
					info.index.push_back(second);
					info.index.push_back(third);
					info.winningPercentage = tempPercent;
				}
			}
		}
	}
	

	//after best hand has been decided make changes
	if (info.winningPercentage > .55)
	{
		for (int j = 0; j < info.numberOfCardsToReplace; j++)
		{
			for (int i = 0; i < 5; i++)
			{
				if (info.index.at(j) != i)
				{
					selectCard(i);
					//selectCard(info.index.at(i));
				}
			}
		}
	}
	

}

//returns true if bet was made
bool Dealer::makeBet()
{
	betPlaced  = true;
	//int  multiplier = 0;

	//if dealer has a pair of jacks always bet
	if (getWorth() == 0)
	{
		//do nothing
		betPlaced = false;
	}
	else if (getWorth() == 1 /*&& getPairValue() == 11*/)
	{
		if (getScore() + 5 >= 5)
			moneyBet += 5;
	}
	else if (getWorth() > 1 && getWorth() < 4)
	{
		if (getScore() + 5 >= 10)
			moneyBet += 10;
	}
	else if (getWorth() > 3 && getWorth() < 7)
	{
		if (getScore() + 5>= 15)
			moneyBet += 15;
	}
	else if (getWorth() > 6 && getWorth() < 9)
	{
		if (getScore() + 5 >= 25)
			moneyBet +=25;
	}
	else
	{
		moneyBet = getScore();
	}


	return betPlaced;
}


//if false dealer folded
bool Dealer::callBet(int betToCall)
{
	bool call = false;
	if (getScore() >= betToCall)
	{
		moneyBet = betToCall;
		call     = true;
	}
	return call;
}

