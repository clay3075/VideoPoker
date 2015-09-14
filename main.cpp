/*main.cpp
 *
 *@remarks This function will simply create a game object and run the game. This 
 *		   function will also catch any exceptions that may be thrown by the game.
 *         In the case than an exception be thrown the user will displayed message 
 *         of why the exception occured and the game will be shut down.
 *
 *@author Clay Reddick
 *@date	  09/11/15
 */

 #include "game.hpp"
 #include <iostream>

 int main()
 {
 	try
 	{
 		Game game;
 		game.run();
 	}
 	catch (char* exception)
 	{
 		std::cout << exception << std::endl;
 	}

 	return 0;
 }