//hand.cpp
//Hand class implementation file
//Composed of Card class

#include "card.hpp" //include Card class
#include "hand.hpp"
#include <vector>
	using std::vector;
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cstdlib>
//protected

/* Method used to calculate the worth of the cards in the hand.
 * Will give the hand strength numeric values 0-9.
 */
void Hand::calcWorth()
{
	if (cards.empty()) //if no cards are in hand
	{
		worth = 0;
	}
	else
	{
		//implementation for blackjack
		/*int points = 0; //accumulator
		for (unsigned int i = 0; i < cards.size(); i++) //for each card
		{
			points += cards.at(i).getValue(); //add up card values
		}
		worth = points;*/
	

		//custom implementation for poker
		string nameOfHand = getNameOfHand();

		//give the hand a numeric value
		//
		if(nameOfHand == "Royal Flush")
		{
			worth = 9;
		}
		else if(nameOfHand == "Straight Flush")
		{
			worth = 8;
		}
		else if(nameOfHand == "Four of a kind")
		{
			worth = 7;
		}
		else if(nameOfHand == "Full House")
		{
			worth = 6;
		}
		else if(nameOfHand == "Flush")
		{
			worth = 5;
		}
		else if(nameOfHand == "Straight")
		{
			worth = 4;
		}
		else if(nameOfHand == "Three of a kind")
		{
			worth = 3;
		}
		else if(nameOfHand == "Two Pair")
		{
			worth = 2;
		}
		else if(nameOfHand == "Pair")
		{
			worth = 1;
		}
		else
		{
			worth = 0;
		}
		//
	}
	return;
} 

//public

/*default constructor used to initialize hand to blank values 
 *does so by calling clear() function
 */
Hand::Hand()
{
	clear();
}

/*mirrors the drawing of a card from a deck and adds it to the cards in hand and then calculates the hands worth
 *@param card Is a Card value used to mirror the existence of a card being drawn
 */
void Hand::drawCard(Card card)
{
	cards.push_back(card); //add card to deck
	//sort cards as they are accumulated
	if (cards.size() != 0)
		sort();
	calcWorth();
	return;
}

 /*@returns a vector holding the cards in hand
  */
vector<Card>& Hand::getCards() 
{
	return cards;
}

/*removes all cards from hand
 */
void Hand::clear()
{
	cards.clear();
	calcWorth();
	for (unsigned int i = 0; i < cards.size(); i++) //for each card
	{
		if (cards.at(i).isSelected() == true) //deselect card
		{
			cards.at(i).select();
		}
	}
	return;
}

/*returns the worth of the cards in hand
 */
int Hand::getWorth()
{
	return worth;
}

/*if ace is in hand changes ace value
 *@param val The value to change the ace to
 */
void Hand::changeAceValue(int val)
{
	for (unsigned int i = 0; i < cards.size(); i++) //for each card
	{
		if (cards.at(i).getValue() == 1) //if an ace has value 1 change it
		{
			cards.at(i).changeValue(val);
		}
		else if (cards.at(i).getValue() == val) //if it has value trying to be changed change it to one
		{
			cards.at(i).changeValue(1);
		}
	}
	calcWorth();
}

/* Method used to select a card
 *@param i The index of the card to select
 */
void Hand::selectCard(int i)
{
	cards.at(i).select();
}

/* Method used to see if a card is selected
 *@param i The index of the card you want to check
 *@param returns true if the card is selected
 */
bool Hand::cardSelected(int i)
{
	return cards.at(i).isSelected();
}

/* Method used to replace a card
 *@param i The location of the card to replace
 *@param card The card to replace it with.
 */
void Hand::replaceCard(int i, Card card)
{
	cards.at(i) = card;
	sort();
	calcWorth();
}


/*bool Hand::evaluate()
{
	string nameOfHand = getNameOfHand();
	return;
}*/

/*checks for royal flush etc.. returns as a string the result of the hand
 */
string Hand::getNameOfHand()
{
	string result = "Crappy Hand";

	if (cards.size())
	{
		//first find lowest value card in hand
		Card lowest  = cards.at(0);
		//find highest value card in hand
		Card highest = cards.at(cards.size() - 1);

		if (lowest.getValue() == 1) //if ace is lowest card and there's no two change value of ace
		{	
			bool hasTwo = false; //flag
			for (int i = 0; i < static_cast<int>(cards.size()); i++) //go through hand
			{
				if (cards.at(i).getValue() == 2) //if theres a two in hand
					hasTwo = true;
			}
			if (!hasTwo) //if theres a two in hand
			{
				changeAceValue();
				sort();
			}
		}
		else if (highest.getValue() == 14) //if ace is highest card make sure theres a king
		{								   //if their isnt change ace value to lowest card
			bool hasKing = false; //flag
			for (int i = 0; i < static_cast<int>(cards.size()); i++) //go through hand
			{
				if (cards.at(i).getValue() == 14) //if theres a king
					hasKing = true;
			}
			if (!hasKing) //if theres a king
			{
				changeAceValue();
				sort();
			}
		}
	}

	int highestCount = 0;
	int highestCount2 = 0;
	int secondHighestcount = 0;
	bool encountered[] = {false, false, false, false, false};
	//check for pair, 2 pairs, three of a kind, 4 of a kind, and full house
	for (int i = 0; i < static_cast<int>(cards.size()); i++)
	{
		if (!encountered[i])
		{
			encountered[i] = true;
			highestCount = 1;
			for (int j = 1; j < static_cast<int>(cards.size());j++)
			{
				if((!encountered[j]) && (cards.at(i).getValue() == cards.at(j).getValue()))
				{
					encountered[j] = true;
					highestCount++;
				}
			}
		}
		if (highestCount > highestCount2)
		{
			secondHighestcount = highestCount2;
			highestCount2 = highestCount;
		}
		
	}
	if (highestCount2 == 2)
	{
		result = "Pair";
		if (checkForSecondPairandValue() != -1)
		{
			result = "Two Pair";
		}
	}
	if (highestCount2 == 3)
	{
		result = "Three of a kind";
	}
	if (highestCount2 == 4)
	{
		result = "Four of a kind";
	}
	if (secondHighestcount == 2 && highestCount2 == 3)
	{
		result = "Full House.";
	}

	//then see if the next 4 cards are one value higher and same suit
	//check for straight flush and royal flush
	bool sameSuit = true;
	for (int i = 1; i < static_cast<int>(cards.size()); i++)
	{
		if (cards.at(0).getSuit() != cards.at(i).getSuit())
		{
			sameSuit = false;
		}	
	}
	//check for flush
	int iterator = 0;
	int next = cards.at(0).getValue() + 1;
	bool straight = false;
	while (iterator < static_cast<int>(cards.size()))
	{
		if (cards.at(iterator).getValue() == next)
		{
			next++;
			iterator = 0;
		}
		else
		{
			iterator++;
		}
	}
	if (next == (cards.at(0).getValue() + 5))
	{
		straight = true;
		result = "Straight";
	}

	if (sameSuit)
		result = "Flush";
	if (straight && sameSuit)
	{
		result = "Straight Flush";
	}
	else if (straight && (cards.at(0).getValue() == 10) && sameSuit)
	{
		result = "Royal Flush";
	}


	return result;
}

/* Method used to find a pair and return the value
 *@return returns -1 if no pairs
 */
int Hand::getPairValue()
{
	int pairValue = -1;

	for (int i = 0; i < static_cast<int>(cards.size()) - 1; i++) //for each card
	{
		//check for pairs
		if (cards.at(i).getValue() == cards.at(i+1).getValue())
		{
			pairValue = cards.at(i).getValue();
			i++;
		}
	}

	return pairValue;
}

/* Method used to find 2 pair values and return the second value
 *@return returns -1 if no second pair is found
 */
int Hand::checkForSecondPairandValue()
{
	int pairValue = -1;
	int otherPairValue = getPairValue();

	for (int i = 0; i < static_cast<int>(cards.size()) - 1; i++) //for each card
	{
		//see if there is a pair to it other than the first pair
		if ((cards.at(i).getValue() != otherPairValue) && (cards.at(i).getValue() == cards.at(i+1).getValue())) 
		{
			pairValue = cards.at(i).getValue();
			i++;
		}
	}
	return pairValue;
}

/*bubble sort cards in hand 
 */
void Hand::sort()
{
	Card temp;
	for (int i = 0; i < static_cast<int>(cards.size()-1);i++) //for each card
	{
		for (int j = 0; j < static_cast<int>(cards.size() - 1);j++) //move through the hand
		{
			if (cards.at(j + 1) < cards.at(j)) //if a card is less than another 
			{
				temp = cards.at(j+1);        //
				cards.at(j+1) = cards.at(j); //swap cards
				cards.at(j) = temp;          //
			}
		}
		
	}
	
}
