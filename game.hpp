/*game.hpp
 *specification file
 *
 *@remarks This class will be used to encapsulate the entire game into 
 *         one method for the user to use known as play().
 *
 *@author Clay Reddick
 *@date   09/10/15
 */

#ifndef GAME_H
#define GAME_H

#include "SFML/Graphics.hpp" //needed for gui
#include "card.hpp"
#include "deck.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "pokerplayer.hpp"
#include "dealer.hpp"
#include <thread>



class Game
{
	private:
		static const int CARD1 = 0;
		static const int CARD2 = 1;
		static const int CARD3 = 2;
		static const int CARD4 = 3;
		static const int CARD5 = 4;
		PokerPlayer* player = nullptr;
		Dealer* 	 dealer = nullptr;
		bool playerFolded;
		bool dealerFolded;
		int  nameCharCount;
		bool playersTurnToBet;
		bool gameMenuNeeded;
		bool textEntered;
		bool textBoxNeeded;
		bool secondDealAllowed;
		bool betMenuNeeded;
		bool betCalled;
		sf::RenderWindow* window = nullptr;
		sf::Event event;
		bool mousePressed;
		sf::Vector2i* mousePos = nullptr;
		sf::Texture texture;
		sf::Sprite cardSprites[5]; 
		sf::Sprite dealerCardSprite;
		sf::Sprite callSprite;
		sf::Sprite raiseSprite;
		sf::Sprite dealSprite;
		sf::Sprite checkSprite;
		sf::Sprite foldSprite;
		sf::Sprite newGameSprite;
		sf::Sprite resumeSprite;
		sf::Sprite doneBettingSprite;
		sf::Sprite clearBetSprite;
		sf::Sprite chip5Sprite;
		sf::Sprite chip15Sprite;
		sf::Sprite chip25Sprite;
		sf::Sprite allInSprite;
		sf::Font font;
		sf::Text text;
		std::thread decide;
		void processEvents(); //used to handle all events that occur in the game
		void update();		  //will update the game based events that occured
		void render(); 		  //shows changes to game on screen
		void initializePlayer(); //will read data from "savedinfo.txt" if it exist into player on game
		void drawMenu();         //draw game menu when it is opened
		void drawButtons();		 //draw bet1, bet5, deal buttons
		void drawScore();		 //draw players credits
		void drawCards();
		void drawHold();   
		void drawBetMenu();      //draws betting options to the screen
		void drawNewHandInstructions();
		void drawDealersMove();
		void restartHand();
		void secondDeal(bool p = true, bool d = true);
		void drawWinnerAndHandSpecifications();
		void renderTextBox();    //draw text box to screen and any characters typed in
		void saveGame();		 //will save game information to a file called "savedinfo.txt"
		bool savedGameExists();  //will return true if there is a "savedinfo.txt" file false otherwise
		void adjustScore();
		void placeBet(int amount);
	public:	
		Game(); //default constructor //sets up game graphics
		void run(); //will be used to play the game
		~Game(); //destructor to free allocated memory
};

#endif