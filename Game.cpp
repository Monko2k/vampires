#include <iostream>
#include "Game.h"
#include "Level.h"
#include "Actors.h"
#include "Utilities.h"


Game::Game(int goblinSmellDistance)   //Game constructor
{
	p = new Player; //Make a new player
	m_current_level = 0;    
    m_goblinSmellDistance = goblinSmellDistance;
    Level* newLevel = new Level(p, goblinSmellDistance, 0); //Make the first level
    levels.push_back(newLevel);
}

Game::~Game()
{
	for (int i = 0; i < m_current_level; i++)   //Delete every level in the level vector
	{
		delete levels[i];
	}
	delete p;   //Delete the player
}

void Game::play()
{
    while (m_current_level <= 4)    //Run while the current level is 4 or less
    {
        clearScreen();
        levels[m_current_level]->display(); 
        while (levels[m_current_level]->level_status() == 0) //Run while the level is not marked as completed
        {
            if (p->isDead())    //Exit the game if the player dies
            {
                std::cout << "Press q to exit game." << endl;
                while (getCharacter() != 'q' && getCharacter() != 'Q')
                {
                }
                return;
            }

            levels[m_current_level]->player_move(); //Process the player move
            if (levels[m_current_level]->exit_status() == true) //Exit if the player's move was a "Q"
                return;
            else if (m_current_level == 4 && levels[m_current_level]->level_status() == 1)  //Check if the player completed the game 
            {
                std::cout << endl << "Press q to exit game." << endl;
                while (getCharacter() != 'q' && getCharacter() != 'Q')
                {
                }
                return;
            }
            levels[m_current_level]->update();  //Update the board after the player's move
            levels[m_current_level]->monster_move();    //Process the monster's move
            clearScreen();  
            levels[m_current_level]->display(); //Redraw the board

        }

        m_current_level++;  //Increment the current level variable if the level is completed
        if (m_current_level <= 4)   //Add a new level to the level vector
        {
            Level* newLevel = new Level(p, m_goblinSmellDistance, m_current_level);
            levels.push_back(newLevel);
        }
    }


}
