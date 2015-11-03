/* pokerplayer.hpp
 * specification file for PokerPlayer class
 * Inherits from Player class and Hand class
 * Composed of Hand class
 *
 * @author Clay Reddick 
 */

#ifndef POKERPLAYER_H
#define POKERPLAYER_H

#include "player.hpp"
#include "hand.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
	using std::vector;

class PokerPlayer : public Player, public Hand
{
	protected:
		int  moneyBet;
		bool betPlaced;
	public:
		PokerPlayer(int score = 200);
		PokerPlayer(Hand &hand) : Hand(hand.getCards()){};
		void placeBet(const int betAmount = 0); //will be used to place a bet
		void calcScore(bool wonOrLost); //will be used to keep score in a valid state as influences are made on it
		int  getBet() //returns the current total the bet is at
			{return moneyBet;}
		void clearBet(); //will null the moneyBet variable and change betPlaced to false
		bool isBetPlaced(); //will return true if player has put in a bet false otherwise
		bool findWinner(PokerPlayer* otherPlayer); //returns true if this player had winning hand
		void show(sf::Sprite* sprite, sf::RenderWindow* window, sf::Texture& texture, PokerPlayer* player); //shows hand to screen
};

#endif
