/*dealer.hpp
 *specification file
 *
 *@remarks 
 *
 *@author Clay Reddick
 *@date   09/10/15
 */

 #ifndef DEALER_H
 #define DEALER_H

 #include "pokerplayer.hpp"
 #include "deck.hpp"
 #include <string>
 	using std::string;

class Dealer: public PokerPlayer
{
	private:
		Deck deck; 
	public:	
		Dealer(); //constructor will initialize all values needed
		void shuffle(); //will randomize cards in deck by calling the Deck class shuffle() function
		Card deal();   //will return a Card object from the "top" of the deck object
		void show(sf::Sprite sprite, sf::RenderWindow* window, sf::Texture& texture, Dealer* player);
};

 #endif