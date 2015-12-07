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
#include <random>

/*constructor will initialize all values needed
 */
Dealer::Dealer()
{
	setName("Dealer");
	shuffle();
}

/*will randomize cards in deck by calling the Deck class shuffle() function
 */
void Dealer::shuffle()
{
	deck.shuffle();
}

/*will return a Card object from the "top" of the deck object
 */
Card Dealer::deal()
{
	return deck.deal();
}  

/* Renders dealers cards to the screen
 */
void Dealer::show(sf::Sprite sprite, sf::RenderWindow* window, sf::Texture& texture, Dealer* player, bool showCards)
{
	vector<Card> tempCards;
	tempCards = player->getCards();
	//draw cards to screen
	for (unsigned int i = 0; i < player->getNumberOfCardsInHand(); i++) //for each card in hand
	{
		//if dealer is allowed to show cards show face values
		if (showCards)
		{
			if (!texture.loadFromFile("images/" + tempCards.at(i).getFace() + tempCards.at(i).getSuit() + ".jpg"))
			{
				throw "Sprite Error";
			}
		}
		else //otherwise just show back of card
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

/*will calculate chances of winning and adjust strategy of hand based on it
 *does so by checking all possibilities (takes a while)
 */
void Dealer::decideHand()
{
	info.index.clear();
	info.winningPercentage = 0;
	info.numberOfCardsToReplace = 0;

	Hand testHand; //testHand for finding best move
	Hand oldHand;
	Deck testDeck; //testDeck for finding best move
	Deck testDeck2;
	Deck testDeck3;
	int  newHandWins = 0;
	double tempPercent = 0;
	int numTestRuns = 0;

	//check what happens if one card is removed from each possible position
	for (int i = 0; i < 5; i++) //for each card in hand
	{
		newHandWins = 0; //reset wins
		testDeck = deck; //reset testDeck
		
		testHand = Hand(getCards()); //reset testHand
		
		int temp = testDeck.cardsInDeck(); //get num of cards in deck
		for (int j = 0; j < temp; j++) //for each card in deck
		{
			numTestRuns++; //increment

			testHand.replaceCard(i, testDeck.deal()); //make replacement
			Dealer tempDealer(testHand,testDeck); //create dealer to compare hands
			if (!(this->findWinner(&tempDealer))) //new hand won
			{
				newHandWins++; //increment
			}
		}

		tempPercent = static_cast<double>(newHandWins) / numTestRuns; //get winning percentage

		numTestRuns = 0;
		newHandWins = 0;
		if (tempPercent > info.winningPercentage) //if new winning percentage is better swap
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
		for (int second = first + 1; second < 5; second++) //for last 4 cards
		{
			//reset values
			newHandWins = 0;
			numTestRuns = 0;
			testDeck = deck;
			testDeck2 = deck;
			testDeck2.deal(); //get rid of one card from testdeck

			for (int j = 0; j < deck.cardsInDeck(); j++) //for number of cards in deck
			{
				testHand = Hand(getCards());
				testHand.replaceCard(first, testDeck.deal());
				oldHand = testHand;
				for (int p = 0; p < deck.cardsInDeck() - 1; p++) //for number of cards in deck - 1 card
				{
					numTestRuns++; //increment

					testHand.replaceCard(second, testDeck2.deal()); //make replacement

					Dealer tempDealer(testHand,testDeck); //temp player to compare hand to
					if (!(this->findWinner(&tempDealer))) //if new hand won
					{
						newHandWins++; //increment

					}
					testHand = oldHand;
				}
			}
			tempPercent = static_cast<double>(newHandWins) / numTestRuns; //get percent times won

			if (tempPercent > info.winningPercentage) //if new winning percentage is better swap
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
	for (int first = 0; first < 5; first++) //for cards in hand
	{	
		bool secondPassed = false; //set flag
		for (int second = first + 1; second < 5 && !secondPassed; second++) //for last 4 cards unless flag flipped
		{
			if (first == 4) //if second card has passed
			{
				second = 0;
				secondPassed = true;
			}
			for (int third = second + 1; (third < 5 && second != 4 && !secondPassed) || (secondPassed && third < first); third++)
			{
				//reset values
				newHandWins = 0;
				numTestRuns = 0;
				testDeck  = deck;
				testDeck2 = deck;
				testDeck3 = deck;
				testDeck2.deal(); //discard one card 
				testDeck3.deal(); //discard two cards
				testDeck3.deal(); //

				for (int j = 0; j < deck.cardsInDeck(); j++) //for number of cards in deck
				{
					testHand = Hand(getCards());
					testHand.replaceCard(first, testDeck.deal()); //make replacement
					oldHand = testHand;
					for (int p = 0; p < deck.cardsInDeck() - 1; p++) //for number of cards in deck - the first
					{
						testHand.replaceCard(second, testDeck2.deal()); //make replacement
						oldHand = testHand;
						for (int t = 0; t < deck.cardsInDeck() - 2; t++) //for cards in the deck minus the first two
						{
							numTestRuns++; //increment

							testHand.replaceCard(third, testDeck3.deal()); //make replacement

							Dealer tempDealer(testHand,testDeck); //
							if (!(this->findWinner(&tempDealer))) //test new hand against old hand if it wins 
							{
								newHandWins++; //increment
							}
							testHand = oldHand;
						}
					}
				}

				tempPercent = static_cast<double>(newHandWins) / numTestRuns;


				if (tempPercent > info.winningPercentage) //if new winning percentage is better swap
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
	if (info.winningPercentage > .55) //if percent improvements greater than 55%
	{
		for (int j = 0; j < info.numberOfCardsToReplace; j++) //for each card that needs to be replaced
		{
			for (int i = 0; i < 5; i++) //go through hand
			{
				if (info.index.at(j) != i) //make neccessary replacements
				{
					selectCard(i);
				}
			}
		}
	}
	

}

/* Method used to determine whether dealer should bet
 *@return Returns true if bet was made.
 */
bool Dealer::makeBet()
{
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<int> dist(1, 15);
	betPlaced  = true;

	//if crappy hand make no bet unless random
	if (getWorth() == 0 || 5 == dist(mt))
	{
		//do nothing
		betPlaced = false;
	}
	else if ((getWorth() == 1) || 11 == dist(mt)) //if hand is pair or random bet $5
	{
		if (getScore() + 5 >= 5)
			moneyBet += 5;
	}
	else if ((getWorth() > 1 && getWorth() < 4) || 13 == dist(mt)) //if hand is 2 pair or 2 of a kind or random bet $10 
	{
		if (getScore() + 5 >= 10)
			moneyBet += 10;
	}
	else if ((getWorth() > 3 && getWorth() < 7) || 2 == dist(mt)) //if hand is straight, flush, full house, or random bet $15
	{
		if (getScore() + 5>= 15)
			moneyBet += 15;
	}
	else if ((getWorth() > 6 && getWorth() < 9) || 10 == dist(mt)) //if hand is 4 of a kind or straight flush or random bet $25
	{
		if (getScore() + 5 >= 25)
			moneyBet +=25;
	}
	else if (7 == dist(mt)) //if random bet $50
	{
		if (getScore() + 5 >= 25)
			moneyBet +=50;
	}
	else //if royal flush go all in
	{
		moneyBet = getScore();
	}


	return betPlaced;
}


/* Method used to determine whether dealer should call bet
 *@param betToCall The bet to decide to call
 *@return Returns true if bet was made.
 */
bool Dealer::callBet(int betToCall)
{
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<int> dist(1, 20);
	betPlaced  = true;

	bool call = false;
	if (betToCall == 0 || (getWorth() == 0 && (betToCall + getBet()) < 15) || dist(mt) == 10)
	{
		//do nothing
		call = true;
	}
	else if ((betToCall >= 5 && betToCall <=10 && getWorth() == 1) || dist(mt) == 3)
	{
		call = true;
	}
	else if ((betToCall >= 15 & betToCall <= 20 && getWorth() >=2 && getWorth() <= 3) || dist(mt) == 12)
	{
		call = true;
	}
	else if ((betToCall >= 25 & betToCall <= 35 && getWorth() >=4 && getWorth() <= 5) || dist(mt) == 20)
	{
		call = true;
	}
	else if ((betToCall >= 40 & betToCall <= 55 && getWorth() >=6 && getWorth() <=7) || dist(mt) == 17)
	{
		call = true;
	}
	else if ((betToCall >= 60 & betToCall <= 100 && getWorth() == 8) || dist(mt) == 9)
	{
		call = true;
	}
	else if ((betToCall >= 100 && getWorth() > 8) || dist(mt) == 1)
	{
		call = true;
	}

	if (call) //if the decision to call bet has been made
	{
		if (getScore() >= betToCall + getBet()) //make bet if dealer has enough money
		{
			moneyBet += betToCall;
		}
		else //otherwise go all in
		{
			moneyBet = getScore();
		}
	}
	return call;
}

