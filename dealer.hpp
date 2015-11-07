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
 #include <vector>
 	using std::vector;

class Dealer: public PokerPlayer
{
	//structure for holding which cards need to be replaced basec on the ai
	struct handReplaceInfo
	{
		int numberOfCardsToReplace = 0;
		vector<int> index;
		double winningPercentage; //will hold the chance this hand will beat the current one
	};

	private:
		Deck deck; 
		handReplaceInfo info;
	public:	
		Dealer(); //constructor will initialize all values needed
		Dealer(Hand& hand, Deck deck) : PokerPlayer(hand)
		{
			this->deck = deck;
		}
		void shuffle(); //will randomize cards in deck by calling the Deck class shuffle() function
		Card deal();   //will return a Card object from the "top" of the deck object
		void show(sf::Sprite sprite, sf::RenderWindow* window, sf::Texture& texture, Dealer* player, bool showCards);
		void decideHand(); //will calculate chances of winning and adjust strategy of hand based on it
		int  cardsInDeck(){return deck.cardsInDeck();}
};

 #endif