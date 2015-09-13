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
		if (newGameSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(*mousePos)))
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
	}
	//if player enters text
	else if (textBoxNeeded && textEntered)
	{
		//if text is a printable character
		if (event.text.unicode < 128 && nameCharCount < 16)
		{
			player->setName(player->getName() + static_cast<char>(event.text.unicode)); //add text to players name
			textEntered = false;
			nameCharCount += 1;
			if (static_cast<char>(event.text.unicode) == '\n')
			{
				textBoxNeeded = false;
			}	
			if (static_cast<char>(event.text.unicode) == '\b')
			{
				
				string temp = player->getName();
				temp = temp.substr(0, temp.size() - 2);
				player->setName(temp);
				if (nameCharCount > 1)
				{
					nameCharCount -= 2;
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

//default constructor //sets up game graphics
Game::Game()
{
	player = new PokerPlayer;
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
}

