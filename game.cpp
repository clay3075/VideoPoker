/*game.cpp
 *specification file
 *
 *@remarks Defines all methods needed in order to make the game class work
 *
 *@author Clay Reddick
 *@date   09/10/15
 */



#include "game.hpp"
#include "SFML/graphics.hpp"
#include <fstream>
 	using std::ios;
 	using std::ofstream;
 	using std::ifstream;
 	using std::fstream;
#include <string>
 	using std::string;
#include <iostream>
#include <ctime>
#include <thread>
#include <cstring>


//struct used for writing save game information to a file
struct SaveGameInfo
{
	string name = "";
	int    playerScore = 0;
	int    dealerScore = 0;
};


//used to handle all events that occur in the game
void Game::processEvents()
{
	mousePressed = false;
	//check for any user activity in the game
	while (window->pollEvent(event))
	{
		//if user clicks close button
		if (event.type == sf::Event::Closed)
		{
			saveGame();
			window->close(); //close program
		}
		//if user clicks with their mouse in game
		if (event.type == sf::Event::MouseButtonPressed)
		{
			*mousePos = sf::Mouse::getPosition(*window); //set mouse postion to where user clicked
			mousePressed = true;
		}
		//if the user enters text and the menu is still needed
		if (event.type == sf::Event::TextEntered && textBoxNeeded)
		{
			textEntered = true;
		}
	}
}

//will update the game based events that occured
void Game::update()
{
	if (mousePressed)
	{
		//if player runs out of money
		if ((player->getScore() == 0 || dealer->getScore() == 0 ) && mousePressed)
		{
			player->setScore(200);
			dealer->setScore(200);
			restartHand();
		}

		//if player presses new game button from start menu
		if (newGameSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && gameMenuNeeded)
		{
			gameMenuNeeded = false;
			textBoxNeeded = true;
		}

		//if player presses resume button from start menu
		if (resumeSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && gameMenuNeeded)
		{
			gameMenuNeeded = false; //close menu option
			if (savedGameExists())
			{
				initializePlayer();
			}
		}

		//if player presses check button 
		if (checkSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && secondDealAllowed && !betMenuNeeded && !dealer->isBetPlaced())
		{
			secondDeal(false); //only allow dealer to replace cards
			adjustScore();
			player->clearBet();
			dealer->clearBet();
		}

		//if player presses fold button 
		if (foldSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && secondDealAllowed && !betMenuNeeded)
		{
			playerFolded = true;
			secondDealAllowed = false; //hand is over so do not allow another deal
			adjustScore();
			betCalled = true;
		}

		//if player presses call button 
		if (callSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) /*&& secondDealAllowed*/ && !betMenuNeeded && !betCalled)
		{
			betCalled = true;
			//will match dealers bet if possible
			if (dealer->getBet() <= player->getScore())
			{
				player->clearBet();
				player->placeBet(dealer->getBet());
			}
			else
			{
				player->clearBet();
				player->placeBet(player->getScore());
			}
		}

		//if player presses raise button 
		if (raiseSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && secondDealAllowed && !betMenuNeeded && (betCalled || !playersTurnToBet || checked))
		{
			betMenuNeeded = true;
		}

		//if player presses done on bet screen
		if (doneBettingSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			betMenuNeeded = false;
			if (!dealer->callBet(player->getBet() - dealer->getBet()))
			{
				secondDealAllowed = false;
				dealerFolded      = true;
				adjustScore();
			}
		}

		//if player presses clear on bet screen
		if (clearBetSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			//clear bet
			player->clearBet();
			player->placeBet(dealer->getBet());
		}

		//if player presses $5 chip on bet screen
		if (chip5Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			//add money to bet
			placeBet(5);
		}

		//if player presses $15 chip on bet screen
		if (chip15Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			//add money to bet
			placeBet(15);
		}

		//if player presses $25 chip on bet screen
		if (chip25Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			//add money to bet
			placeBet(25);
		}

		//if player presses all in button on bet screen
		if (allInSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && betMenuNeeded)
		{
			//add money to bet
			placeBet(player->getScore() - player->getBet());
		}

		//if player presses deal button 
		if (dealSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded && (betCalled || !playersTurnToBet || checked))
		{
			if (secondDealAllowed) //if its first round allow another round to be dealt
			{
				secondDeal();
				adjustScore();
			}
			else				   //if hand is over restart hand
				restartHand();
		}

		//player presses card 1
		if (cardSprites[CARD1].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded)
		{
			player->selectCard(CARD1);
		}

		//player presses card 2
		if (cardSprites[CARD2].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded)
		{
			player->selectCard(CARD2);
		}

		//player presses card 3
		if (cardSprites[CARD3].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded)
		{
			player->selectCard(CARD3);
		}

		//player presses card 4
		if (cardSprites[CARD4].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded)
		{
			player->selectCard(CARD4);
		}

		//player presses card 5
		if (cardSprites[CARD5].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && !betMenuNeeded)
		{
			player->selectCard(CARD5);
		}
	}
	//if player enters text
	else if (textBoxNeeded && textEntered)
	{
		//if text is a printable character
		if (event.text.unicode < 128 && nameCharCount < 16)
		{	
			if (static_cast<char>(event.text.unicode) != '\n')
			{
				player->setName(player->getName() + static_cast<char>(event.text.unicode)); //add text to players name
				textEntered = false;
				nameCharCount += 1;
			}
			if (static_cast<char>(event.text.unicode) == '\n')
			{
				textBoxNeeded = false;
			}	
			if (static_cast<char>(event.text.unicode) == '\b')
			{
				if (nameCharCount > 1)
				{
					string temp = player->getName();
					temp = temp.substr(0, temp.size() - 2);
					player->setName(temp);
					nameCharCount -= 2;
					std::cout << nameCharCount;
					text.setPosition(sf::Vector2f(800 + (nameCharCount * 10), 445.0f));
				}
			}
		}
		else
		{
			textBoxNeeded = false;
			textEntered   = false;
		}
	}
}

//shows changes to game on screen
void Game::render()
{
	//clear screen with color of table background (Blue for now)
	window->clear(sf::Color::Blue);

	if (gameMenuNeeded) //if game just started
	{
		drawMenu();
	}
	else if (textBoxNeeded)
	{
		renderTextBox();
	}
	else
	{
		//drawDealersMove();
		drawDealersMove();
		text.setCharacterSize(40);
		text.setColor(sf::Color::Black);
		drawButtons();
		drawScore();
		drawCards();
		drawHold();
		drawNewHandInstructions();
		drawWinnerAndHandSpecifications();
		if(betMenuNeeded)
			drawBetMenu();
	}

	//show window to screen
	window->display();
}

void Game::drawDealersMove()
{
	if (playersTurnToBet)
	{
		text.setPosition(sf::Vector2f(635.0f, 420.0f));
		if (dealer->isBetPlaced())
			text.setString("Dealer bet $" + std::to_string((dealer->getBet() - 5)) + ".\nMake your move.");
		else
			text.setString("Dealer checked.\nIt's your turn.");

		/*//draw border of information menu
		text.setCharacterSize(40);
		text.setColor(sf::Color::Yellow);
		text.setPosition(sf::Vector2f(590.0f, 410.0f));
		sf::RectangleShape infoBorder(sf::Vector2f(400.0f, 150.0f));
		infoBorder.setFillColor(sf::Color::Black);
		infoBorder.setPosition(sf::Vector2f(580.0f, 400.0f));
		window->draw(infoBorder);
		window->draw(text);*/
	}
	else
	{
		text.setPosition(sf::Vector2f(690.0f, 435.0f));
		text.setString("Your turn.");
	}
	//draw border of information menu
	text.setCharacterSize(40);
	text.setColor(sf::Color::Yellow);
	sf::RectangleShape infoBorder(sf::Vector2f(400.0f, 150.0f));
	infoBorder.setFillColor(sf::Color::Black);
	infoBorder.setPosition(sf::Vector2f(580.0f, 400.0f));
	window->draw(infoBorder);
	window->draw(text);
}
//will read data from "savedinfo.txt" if it exist into player on game
//if it does not exist defualt data will be used
void Game::initializePlayer()
{
	fstream infoStream("savedinfo.txt", ios::in | ios::binary);
	SaveGameInfo info;
	if (infoStream.is_open())
	{
		infoStream.seekg(0, ios::beg);
		infoStream.read(reinterpret_cast<char*>(&info), sizeof(info));
		infoStream.close();
	}
	else
	{
		info.name = "Player1";
		info.playerScore = 200;
		info.dealerScore = 200;
	}

	player->setName(info.name);
	player->setScore(info.playerScore);
	dealer->setScore(info.dealerScore);
}

//draw game menu when it is opened
void Game::drawMenu()
{
	//draw border of option menu
	sf::RectangleShape border(sf::Vector2f(1100.0f, 600.0f));
	border.setFillColor(sf::Color::Black);
	border.setPosition(sf::Vector2f(250.0f, 200.0f));
	window->draw(border);
	//

	//draw text "Menu" above options
	text.setString("Menu\n");
	text.setPosition(sf::Vector2f(700.0f, 300.0f));
	text.setCharacterSize(80);
	text.setColor(sf::Color::White);
	window->draw(text);

	//draw new game button
	if (!texture.loadFromFile("images/NewGame.png")) //load image
	{
		throw "New Game Button Error."; 
	}
	newGameSprite.setTexture(texture); //create button sprite
	newGameSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	newGameSprite.setPosition(sf::Vector2f(200.0f, 300.0f));   //set postion
	window->draw(newGameSprite);
	//
	//

	//draw resume game button
	if (!texture.loadFromFile("images/Resume.png")) //load image
	{
		throw "Resume Game Button Error."; 
	}
	resumeSprite.setTexture(texture); //create button sprite
	resumeSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	resumeSprite.setPosition(sf::Vector2f(750.0f, 300.0f));   //set postion
	window->draw(resumeSprite);
	//
	//
}

//draws betting options to the screen
void Game::drawBetMenu()
{
	//draw border of option menu
	sf::RectangleShape border(sf::Vector2f(1100.0f, 600.0f));
	border.setFillColor(sf::Color::Black);
	border.setPosition(sf::Vector2f(250.0f, 200.0f));
	window->draw(border);
	//

	//draw box for amount bet
	sf::RectangleShape amountBetBox(sf::Vector2f(550.0f, 75.0f));
	amountBetBox.setFillColor(sf::Color::White);
	amountBetBox.setPosition(sf::Vector2f(530.0f, 220.0f));
	window->draw(amountBetBox);
	//

	//draw amount bet inside box
	text.setColor(sf::Color::Black);
	text.setCharacterSize(40);
	text.setString(std::to_string(player->getBet()));
	//text.setPosition(sf::Vector2f(800 - (sizeof() * 10), 235.0f));
    text.setPosition(sf::Vector2f(780.0f, 235.0f));
	window->draw(text);
	//
	//

	//draw done button
	if (!texture.loadFromFile("images/done.png")) //load image
	{
		throw "done sprite error."; 
	}
	doneBettingSprite.setTexture(texture); //create button sprite
	doneBettingSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	doneBettingSprite.setPosition(sf::Vector2f(1030.0f, 700.0f));   //set postion
	window->draw(doneBettingSprite);
	//
	//

	//draw clear button
	if (!texture.loadFromFile("images/clear.png")) //load image
	{
		throw "clear sprite error."; 
	}
	clearBetSprite.setTexture(texture); //create button sprite
	clearBetSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	clearBetSprite.setPosition(sf::Vector2f(270.0f, 700.0f));   //set postion
	window->draw(clearBetSprite);
	//
	//

	//draw $5 chip button
	if (!texture.loadFromFile("images/PokerChip5.png")) //load image
	{
		throw "PokerChip5 sprite error."; 
	}
	chip5Sprite.setTexture(texture); //create button sprite
	chip5Sprite.setScale(sf::Vector2f(.3f, .3f));            //set size
	chip5Sprite.setPosition(sf::Vector2f(480.0f, 400.0f));   //set postion
	window->draw(chip5Sprite);
	//
	//

	//draw $15 chip button
	if (!texture.loadFromFile("images/PokerChip15.png")) //load image
	{
		throw "PokerChip15 sprite error."; 
	}
	chip15Sprite.setTexture(texture); //create button sprite
	chip15Sprite.setScale(sf::Vector2f(.3f, .3f));            //set size
	chip15Sprite.setPosition(sf::Vector2f(740.0f, 400.0f));   //set postion
	window->draw(chip15Sprite);
	//
	//

	//draw $25 chip button
	if (!texture.loadFromFile("images/PokerChip25.png")) //load image
	{
		throw "PokerChip25 sprite error."; 
	}
	chip25Sprite.setTexture(texture); //create button sprite
	chip25Sprite.setScale(sf::Vector2f(.3f, .3f));            //set size
	chip25Sprite.setPosition(sf::Vector2f(985.0f, 400.0f));   //set postion
	window->draw(chip25Sprite);
	//
	//

	//draw all in button
	if (!texture.loadFromFile("images/silverAllInButton.png")) //load image
	{
		throw "All in button sprite error."; 
	}
	allInSprite.setTexture(texture); //create button sprite
	allInSprite.setScale(sf::Vector2f(.8f, .8f));            //set size
	allInSprite.setPosition(sf::Vector2f(640.0f, 530.0f));   //set postion
	window->draw(allInSprite);
	//
	//
}

//draw text box to screen and any characters typed in
void Game::renderTextBox()
{
	//draw border of option menu
	sf::RectangleShape border(sf::Vector2f(1100.0f, 600.0f));
	border.setFillColor(sf::Color::Black);
	border.setPosition(sf::Vector2f(250.0f, 200.0f));
	window->draw(border);
	//
	//
	sf::RectangleShape textBorder(sf::Vector2f(900.0f, 100.0f));
	textBorder.setFillColor(sf::Color::White);
	textBorder.setPosition(sf::Vector2f(350.0f, 420.0f));
	window->draw(textBorder);
	//
	
	//draw text to screen
	text.setString("What's your name?\n");
	text.setPosition(sf::Vector2f(450.0f, 250.0f));
	text.setCharacterSize(80);
	text.setColor(sf::Color::White);
	window->draw(text);
	text.setColor(sf::Color::Black);
	text.setCharacterSize(40);
	text.setString(player->getName());
	text.setPosition(sf::Vector2f(800 - (nameCharCount * 10), 445.0f));
	window->draw(text);
	//
}

void Game::drawButtons()
{
	if (!secondDealAllowed)
		window->clear(sf::Color::Blue);

	//draw call button
	if (!texture.loadFromFile("images/call2.png")) //load image
	{
		throw "call sprite error."; 
	}
	callSprite.setTexture(texture); //create button sprite
	callSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	callSprite.setPosition(sf::Vector2f(625.0f, 900.0f));   //set postion
	window->draw(callSprite);
	//
	//

	//draw raise button
	if (!texture.loadFromFile("images/raise2.png")) //load image
	{
		throw "raise sprite error."; 
	}
	raiseSprite.setTexture(texture); //create button sprite
	raiseSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	raiseSprite.setPosition(sf::Vector2f(1070.0f, 900.0f));   //set postion
	window->draw(raiseSprite);
	//
	//

	//draw check button
	if (!texture.loadFromFile("images/check2.png")) //load image
	{
		throw "check sprite error."; 
	}
	checkSprite.setTexture(texture); //create button sprite
	checkSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	checkSprite.setPosition(sf::Vector2f(400.0f, 900.0f));   //set postion
	window->draw(checkSprite);
	//
	//

	//draw fold button
	if (!texture.loadFromFile("images/fold2.png")) //load image
	{
		throw "fold sprite error."; 
	}
	foldSprite.setTexture(texture); //create button sprite
	foldSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	foldSprite.setPosition(sf::Vector2f(850.0f, 900.0f));   //set postion
	window->draw(foldSprite);
	//
	//

	//draw deal button
	if (!texture.loadFromFile("images/deal2.png")) //load image
	{
		throw "deal sprite error."; 
	}
	dealSprite.setTexture(texture); //create button sprite
	dealSprite.setScale(sf::Vector2f(1.0f, 1.0f));            //set size
	dealSprite.setPosition(sf::Vector2f(1290.0f, 900.0f));   //set postion
	window->draw(dealSprite);
	//
	//
}

//draw players credits
void Game::drawScore()
{
	//draw text to screen
	text.setString("-" + std::to_string(player->getBet()));
	text.setPosition(sf::Vector2f(10.0f, 860.0f));
	text.setCharacterSize(45);
	text.setColor(sf::Color::Red);
	window->draw(text);
	text.setString("-" + std::to_string(dealer->getBet()));
	text.setPosition(sf::Vector2f(1450.0f, 100.0f));
	window->draw(text);
	text.setString(/*"Score: " + */std::to_string(player->getScore()) + " :Score ");
	text.setPosition(sf::Vector2f(10.0f, 900.0f));
	text.setCharacterSize(70);
	text.setColor(sf::Color::Yellow);
	window->draw(text);
	text.setString("Score: " + std::to_string(dealer->getScore()));
	text.setPosition(sf::Vector2f(1200.0f,10.0f));
	window->draw(text);
}

//if round is over inform user what to do
//will tell user to press deal for next hand.
void Game::drawNewHandInstructions()
{
	if (!secondDealAllowed)
	{
		text.setString("Press deal for new hand.");
		text.setColor(sf::Color::Red);
		text.setCharacterSize(60);
		text.setPosition(sf::Vector2f(450.0f,450.0f));
		window->draw(text);
	}
	//tell player if they are out of money
	if (player->getScore() == 0 || dealer->getScore() == 0)
	{
		//draw border of option menu
		sf::RectangleShape border(sf::Vector2f(850.0f, 400.0f));
		border.setFillColor(sf::Color::Black);
		border.setPosition(sf::Vector2f(350.0f, 200.0f));
		window->draw(border);
		//

		text.setString("            Game Over!\nClick anywhere to play again."); 
		text.setPosition(sf::Vector2f(385.0f, 315.0f)); //i think is good 
		window->draw(text);
	}
}

void Game::drawWinnerAndHandSpecifications()
{
	if (!secondDealAllowed)
	{
		//draws to screen type of hand player had
		text.setString(player->getNameOfHand());
		text.setColor(sf::Color::Black);
		text.setCharacterSize(90);
		text.setPosition(sf::Vector2f(625.0f,750.0f));
		window->draw(text);

		//draws to screen type of hand dealer had
		text.setString(dealer->getNameOfHand());
		text.setPosition(sf::Vector2f(625.0f,250.0f));
		window->draw(text);

		//************************* Now determine winner **************************
		//player is winner
		if (!playerFolded && !dealerFolded)
		{
			if (player->findWinner(dealer))
			{
				text.setString(player->getName() + " Won");
			}
			else /*if (dealer->findWinner(player)) //if other player won*/
			{
				text.setString(dealer->getName() + " Won");
				//std::cout << "Dealer\n";
			}
		}
		else if (playerFolded)
		{
			text.setString(player->getName() + " Folded");
		}
		else /*if (dealerFolded)*/
		{
			text.setString(dealer->getName() + " Folded");
		}
		/*else
		{
			dealerFolded = false;
			playerFolded = false;
		}*/
		
		text.setColor(sf::Color::Red);
		text.setCharacterSize(60);
		text.setPosition(sf::Vector2f(650.0f,20.0f));
		window->draw(text);
	}
}

//draw cards to screen
void Game::drawCards()
{
	player->show(cardSprites, window, texture, player);
	dealer->show(dealerCardSprite, window, texture, dealer, !secondDealAllowed);
}

//if player selects card let them know
void Game::drawHold()
{
	for (int i = 0; i < 5; i++)
	{
		if (player->cardSelected(i))
		{
			text.setString("Hold");
			text.setColor(sf::Color::Yellow);
			text.setCharacterSize(60);
			text.setPosition(sf::Vector2f(75.0f * (3.5 * i+2.6), 520.0f));
			window->draw(text);
		}
	}
}
//will save game information to a file called "savedinfo.txt"
void Game::saveGame()
{
	fstream saveStream("savedinfo.txt", ios::out | ios::binary);
	if (saveStream.is_open())
	{
		SaveGameInfo info;
		std::memset(reinterpret_cast<char*>(&info), 0, sizeof(info));
		info.name = player->getName();
		info.playerScore = player->getScore();
		info.dealerScore = dealer->getScore();
		saveStream.seekp(0,ios::beg);
		saveStream.write(reinterpret_cast<char*>(&info), sizeof(info));
		saveStream.close();
	}
}

void Game::adjustScore()
{
	//determine winner and adjust score
	if (player->getScore() >= 5 && dealer->getScore() >= 5)
	{
		if (playerFolded)
		{
			dealer->clearBet();
			dealer->placeBet(player->getBet());
		}
		if (dealerFolded)
		{
			player->clearBet();
			player->placeBet(dealer->getBet());
		}
		player->calcScore(player->findWinner(dealer)); //adjust for player
		dealer->calcScore(dealer->findWinner(player)); //adjust for dealer
	}
}

//will return true if there is a "savedinfo.txt" file false otherwise
bool Game::savedGameExists()
{
	bool exists = false;
	ifstream readFile("savedinfo.txt");
	if (readFile.is_open())
	{
		exists = true;
	}
	readFile.close();
	return exists;
}

//deal new cards for cards that are held
void Game::secondDeal(bool p, bool d)
{
	secondDealAllowed = false;
	if (p)
	{
		for (int i = 0; i < 5; i++)
		{
			if (player->cardSelected(i) == false)
			{
				player->replaceCard(i,dealer->deal());
			}
		}
	}
	if (d)
	{
		for (int i = 0; i < 5; i++)
		{
			if (dealer->cardSelected(i) == false)
			{
				dealer->replaceCard(i,dealer->deal());
			}
		}
	}
}

void Game::restartHand()
{
	static bool first = true;
	checked = false;

	player->clear();
	dealer->clear();
	player->clearBet();
	dealer->clearBet();
	dealer->shuffle();

	//draw border of loading menu
	sf::RectangleShape border(sf::Vector2f(400.0f, 150.0f));
	border.setFillColor(sf::Color::Black);
	border.setPosition(sf::Vector2f(580.0f, 400.0f));
	window->draw(border);
	if (first)
	{
		window->clear(sf::Color::Black);
		text.setPosition(sf::Vector2f(580.0f, 410.0f));
		text.setString("Preparing game...");
		first = false;
	}
	else
	{
		text.setString("Shuffling...");
		text.setPosition(sf::Vector2f(635.0f, 430.0f));
	}
	text.setColor(sf::Color::Yellow);
	text.setCharacterSize(60);
	window->draw(text);
	//
	window->display();

	for (int i = 0; i < 5; i++)
	{
		dealer->drawCard(dealer->deal()); //give dealer 5 new cards
	}
	dealer->decideHand();
	//decide.join();
	for (int i = 0; i < 5; i++)
	{
		player->drawCard(dealer->deal()); //give player 5 new cards
	}
	//if player has enough money for the initial buy in which is 5 here
	//and if dealer or other player in this case has enough for buy in.
	player->placeBet(5);
	player->setBetFlag(false);
	dealer->placeBet(5);
	dealer->setBetFlag(false);

	if (playersTurnToBet == false)
	{
		if (!dealer->makeBet())
			checked = true;
	}

	secondDealAllowed = true;
	playerFolded = false;
	dealerFolded = false;
	betCalled    = false;
	playersTurnToBet = !playersTurnToBet;
	window->clear();
}

void Game::placeBet(int amount)
{
	int oldWorth = player->getScore();
	player->placeBet(amount);
	if ((oldWorth + amount) != player->getScore())
	{
		text.setString("Not enough money.");
		text.setColor(sf::Color::Yellow);
		text.setCharacterSize(60);
		text.setPosition(sf::Vector2f(600.0f, 520.0f));
		window->draw(text);
	}
}

//default constructor //sets up game graphics
Game::Game()
{
	srand(time(0)); //seed random generator
	rand();


	player = new PokerPlayer;
	dealer = new Dealer;

	//decide = std::thread(&Dealer::decideHand, &dealer);
	secondDealAllowed = false;


	//create window for game to played in
	window = new sf::RenderWindow(sf::VideoMode(1600, 1000), "5 Card Poker");

	//set moused pressed to false
	mousePressed = false;
	//set mouse starting position to (0,0)
	mousePos = new sf::Vector2i(0,0);

	//load font
	if (!font.loadFromFile("fonts/arial.ttf"))
	{
		throw "Error loading font.\n";
	}
	//set font details
	text.setFont(font);
	text.setCharacterSize(40);
	text.setColor(sf::Color::Black);

	//set game menu to needed
	gameMenuNeeded = true;
	//set text entered to false
	textEntered   = false;
	//text box not needed yet
	textBoxNeeded = false;
	nameCharCount = 0;
	playerFolded  = false;
	dealerFolded  = false;
	betMenuNeeded = false;
	playersTurnToBet = false;
	betCalled     = true;
	dealer->setBetFlag(false);
	player->setBetFlag(false);
}

//will be used to play the game
void Game::run()
{
	bool first = true;
	while (window->isOpen())
	{
		processEvents();
		update();
		if (first)
		{
			restartHand();
			first = false;
		}
		render();
	}
}

//destructor to free allocated memory
Game::~Game()
{
	delete window;
	window = nullptr;
	delete mousePos;
	mousePos = nullptr;
	delete player;
	player = nullptr;
	delete dealer;
	dealer = nullptr;
}

