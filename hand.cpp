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

//function to calculate the worth of the cards in hand
void Hand::calcWorth()
{
	if (cards.empty()) //if no cards are in hand
	{
		worth = 0;
	}
	else
	{
		int points = 0; //accumulator
		for (unsigned int i = 0; i < cards.size(); i++) //for each card
		{
			points += cards.at(i).getValue(); //add up card values
		}
		worth = points;
	}
	return;
} 

//public

//default constructor used to initialize hand to blank values 
//does so by calling clear() function
Hand::Hand()
{
	clear();
}

//mirrors the drawing of a card from a deck and adds it to the cards in hand and then calculates the hands worth
//@param card Is a Card value used to mirror the existence of a card being drawn
void Hand::drawCard(Card card)
{
	cards.push_back(card);
	calcWorth();
	if (cards.size() != 0)
		sort();
	return;
}
//returns a vector holding the cards in hand
vector<Card>& Hand::getCards()
{
	return cards;
}

//removes all cards from hand
void Hand::clear()
{
	cards.clear();
	calcWorth();
	for (unsigned int i = 0; i < cards.size(); i++)
	{
		if (cards.at(i).isSelected() == true)
		{
			cards.at(i).select();
		}
	}
	return;
}

//returns the worth of the cards in hand
int Hand::getWorth()
{
	return worth;
}

//if ace is in hand changes ace value
void Hand::changeAceValue()
{
	for (unsigned int i = 0; i < cards.size(); i++)
	{
		if (cards.at(i).getValue() == 1)
		{
			cards.at(i).changeValue(11);
		}
		else if (cards.at(i).getValue() == 11)
		{
			cards.at(i).changeValue(1);
		}
	}
	calcWorth();
}

//select card
void Hand::selectCard(int i)
{
	cards.at(i).select();
}

bool Hand::cardSelected(int i)
{
	return cards.at(i).isSelected();
}

//replace chosen cards
void Hand::replaceCard(int i, Card card)
{
	cards.at(i) = card;
	sort();
}

/*
//overloaded assignment operator
bool Hand::operator= (const Hand& hand)
{
	this->cards = hand.cards;
	this->calcWorth();
}
*/

//checks for royal flush etc.. returns as a string the result of the hand
string Hand::evaluate()
{
	string result = "Crappy Hand";

	//check for royal flush
	//first find lowest value card in hand
	Card lowest = cards.at(0);

	if (lowest.getValue() == 1) //if ace is lowest card and there's no two change value of ace
	{	
		bool hasTwo = false;
		for (int i = 0; i < static_cast<int>(cards.size()); i++)
		{
			if (cards.at(i).getValue() == 2)
				hasTwo = true;
		}
		if (!hasTwo)
			changeAceValue();
	}
	int highestCount = 0;
	int highestCount2 = 0;
	int secondeHighestcount = 0;
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
			secondeHighestcount = highestCount2;
			highestCount2 = highestCount;
		}
	}
	if (highestCount2 == 2)
	{
		result = "Pair";
	}
	if (highestCount2 == 3)
	{
		result = "Three of a kind";
	}
	if (highestCount2 == 4)
	{
		result = "Four of a kind";
	}
	if (secondeHighestcount == 2 && highestCount2 == 3)
	{
		result = "Full House.";
	}
	//if ((highestCount == 2 && count2 == 1) || (highestCount == 1 && count2 == 2))
		//result = "Full House";

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
		straight = true;

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


//bubble sort cards in hand
void Hand::sort()
{
	Card temp;
	for (int i = 0; i < static_cast<int>(cards.size());i++)
	{

		for (int j = 0; j < static_cast<int>(cards.size() - 1);j++)
		{
			if (cards.at(j + 1) < cards.at(j))
			{
				temp = cards.at(j+1);
				cards.at(j+1) = cards.at(j);
				cards.at(j) = temp;
			}
		}
		
	}
	
}
