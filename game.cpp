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
 	using std::ofstream;
 	using std::ifstream;
#include <string>
 	using std::string;
#include <iostream>
#include <ctime>

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
		//if player presses new game button from start menu
		if (newGameSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)) && gameMenuNeeded)
		{
			gameMenuNeeded = false;
			textBoxNeeded = true;
		}

		//if player presses resume button from start menu
		if (resumeSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			gameMenuNeeded = false; //close menu option
			if (savedGameExists())
			{
				initializePlayer();
			}
		}

		//if player presses check button 
		if (checkSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
		}

		//if player presses fold button 
		if (foldSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			secondDealAllowed = false; //hand is over so do not allow another deal
		}

		//if player presses call button 
		if (callSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
		}

		//if player presses raise button 
		if (raiseSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
		}

		//if player presses deal button 
		if (dealSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			if (secondDealAllowed) //if its first round allow another round to be dealt
				secondDeal();
			else				   //if hand is over restart hand
				restartHand();
		}

		//player presses card 1
		if (cardSprites[CARD1].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			player->selectCard(CARD1);
		}

		//player presses card 2
		if (cardSprites[CARD2].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			player->selectCard(CARD2);
		}

		//player presses card 3
		if (cardSprites[CARD3].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			player->selectCard(CARD3);
		}

		//player presses card 4
		if (cardSprites[CARD4].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
		{
			player->selectCard(CARD4);
		}

		//player presses card 5
		if (cardSprites[CARD5].getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
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
		text.setCharacterSize(40);
		text.setColor(sf::Color::Black);
		drawButtons();
		drawScore();
		drawCards();
		drawHold();
		drawNewHandInstructions();
		drawWinnerAndHandSpecifications();
	}

	//show window to screen
	window->display();
}

//will read data from "savedinfo.txt" if it exist into player on game
void Game::initializePlayer()
{

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
	text.setString("Score: " + std::to_string(player->getScore()));
	text.setPosition(sf::Vector2f(10.0f, 900.0f));
	text.setCharacterSize(70);
	text.setColor(sf::Color::Yellow);
	window->draw(text);
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
		if (player->findWinner(dealer))
		{
			text.setString(player->getName() + " Won");
		}
		else if (dealer->findWinner(player)) //if other player won
		{
			text.setString(dealer->getName() + " Won");
			//std::cout << "Dealer\n";
		}
		else
		{
			text.setString("Tie");
		}
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
void Game::secondDeal()
{
	secondDealAllowed = false;
	for (int i = 0; i < 5; i++)
	{
		if (player->cardSelected(i) == false)
		{
			player->replaceCard(i,dealer->deal());
		}
	}
}

void Game::restartHand()
{
	player->clear();
	dealer->clear();
	dealer->shuffle();
	for (int i = 0; i < 5; i++)
	{
		player->drawCard(dealer->deal()); //give player 5 new cards
		dealer->drawCard(dealer->deal()); //give dealer 5 new cards
	}

	secondDealAllowed = true;
}

//default constructor //sets up game graphics
Game::Game()
{
	srand(time(0)); //seed random generator
	rand();

	player = new PokerPlayer;
	dealer = new Dealer;

	secondDealAllowed = false;

	restartHand();


	//create window for game to played in
	window = new sf::RenderWindow(sf::VideoMode(1600, 1000), "Jacks or Better");

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
	textEntered = false;
	//text box not needed yet
	textBoxNeeded = false;
	nameCharCount = 0;
}

//will be used to play the game
void Game::run()
{
	while (window->isOpen())
	{
		processEvents();
		update();
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

