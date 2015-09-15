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