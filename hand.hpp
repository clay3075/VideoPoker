//hand.hpp
//Hand Class specification file
//Composed from Card Class

#ifndef HAND_H
#define HAND_H

#include "card.hpp" //include Card class
#include <vector>
	using std::vector;
#include <string>
	using std::string;
#include "SFML/Graphics.hpp"


class Hand
{
protected:
	vector<Card> cards; //vector to symbolize cards in hand
	int worth;          //holds the integer value of what the cards are worth
	void calcWorth();   //function to calculate the worth of the cards in hand
public:	
	Hand(); 				  //default constructor used to initialize hand to blank values
	Hand(const vector<Card>& cards) //constructor used for initilizing hand of 5
			{this->cards = cards;}
	void drawCard(Card card); //mirrors the drawing of a card from a deck and adds it to the cards in hand and then calculates the hands worth
	vector<Card>& getCards();  //returns a vector holding the cards in hand
	void clear();             //removes all cards from hand
	void show(){}; 			  //function shows card to screen
	int getWorth();	   	 	  //returns the worth of the cards in hand
	unsigned int getNumberOfCardsInHand() //returns how many cards are in hand
		{return cards.size();}
	void changeAceValue();     //if ace is in hand changes ace value
	void selectCard(int i);    //selects or deselects card
	bool cardSelected(int i);  //returns true if card is selected
	void replaceCard(int i, Card card); //takes index and card that is to replace card of location i
	string evaluate(); //checks for royal flush etc.. returns as a string the result of the hand
	//bool operator= (const Hand& hand); //overloaded assignment operator
	void sort();
};

#endif