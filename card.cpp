//card.cpp
//Card class implementation file

#include "card.hpp"
#include <string>
	using std::string;
#include <cstdlib>

//contructor used to initialize values
//does not check integer value due to fact that some games require cards to be negative points
Card::Card(string fce, string s, int val)
{
	face = fce;
	suit = s;
	value = val;
	selected = false;
}

//returns the string face value of the card
string Card::getFace()
{
	return face;
}

//returns the suit of the card as a string
string Card::getSuit()
{
	return suit;
}

//returns the actual value of the card as an int
int Card::getValue()
{
	return value;
}

//changes value of card
void Card::changeValue(int val)
{
	value = val;
}

//will mark this card as selected unless already selected it will then be deselected
void   Card::select()
{
	selected = !selected; //reverse truth value
}

//will return whether card is selected or not true for yes false for no
bool   Card::isSelected()
{
	return selected;
}

