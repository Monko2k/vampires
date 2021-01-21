#include <iostream>
#include <string>
#include <utility>
#include "Level.h"
#include "Utilities.h"
#include "Utilities_S.h"

//Public functions

Level::Level(Player* player, int goblinSmellDistance, int stage)	//Level constructor
{
	level_quit = 0;
	level_complete = 0;
	m_stage = stage;
	p = player;

	//Prepare the level

	test_grid();
	spawn_player(p);
	spawn_monsters(goblinSmellDistance, stage);
	spawn_items();

	m_grid[p->get_y()][p->get_x()] = p->get_symbol(); //Draw the character

	for (int i = 0; i < m_num_monster; i++)	//Draw the monsters
	{
		int x = m_monsters[i]->get_x();
		int y = m_monsters[i]->get_y();
		char symbol = m_monsters[i]->get_symbol();
		m_grid[y][x] = symbol;
	}
	for (int i = 0; i < m_num_objects; i++)	//Draw the items
	{
		int x = m_objects[i]->get_x();
		int y = m_objects[i]->get_y();
		if(m_grid[y][x] == ' ')
			m_grid[y][x] = m_objects[i]->get_symbol();
	}
	if (m_grid[m_exit->get_y()][m_exit->get_x()] == ' ')	//Draw the exit object
		m_grid[m_exit->get_y()][m_exit->get_x()] = m_exit->get_symbol();
}

Level::~Level()
{
	for (int i = 0; i < m_monsters.size(); i++)	//Delete every monster inside the monster vector
	{
		if (m_monsters[i] != nullptr)
			delete m_monsters[i];
	}
	for (int i = 0; i < m_objects.size(); i++)	//Delete every item inside the item vector
	{
		if (m_objects[i] != nullptr)
			delete m_objects[i];
	}
	delete m_exit;	//Delete the exit
}

void Level::update()	//Update the playing field
{
	for (int i = 0; i < m_monsters.size(); i++)	//Check every monster in the monster vector to see if it is dead, deleting it if so
	{
		if (m_monsters[i]->isDead())
		{
			m_grid[m_monsters[i]->get_y()][m_monsters[i]->get_x()] = ' ';
			delete m_monsters[i];
			m_monsters.erase(m_monsters.begin() + i);
			m_num_monster--;

		}
	}
	for (int i = 0; i < m_objects.size(); i++)	//Check every item in the item vector to see if it has been picked up, removing it from the vector if so
	{
		if (m_objects[i]->onGround() == false)
		{
			m_grid[m_objects[i]->get_y()][m_objects[i]->get_y()] = ' ';
			m_objects.erase(m_objects.begin() + i);
			m_num_objects--;
		}
	}

	for (int i = 0; i < 18; i++)	//Redraw the walls of the level, ignoring all objects and actors
	{
		for (int j = 0; j < 70; j++)
		{
			if (m_grid[i][j] != ' ' && m_grid[i][j] != '#')
				m_grid[i][j] = ' ';
		}
	}

	if (p->teleport_status() == 1)	//If the player used a teleport, re-execute the spawn function
	{
		p->toggle_teleport();
		spawn_player(p);
		p->add_amr(1);
	}


	m_grid[p->get_y()][p->get_x()] = p->get_symbol(); //Draw the player

	for (int i = 0; i < m_num_monster; i++)	//Draw every monster in the monster vector
	{
		int x = m_monsters[i]->get_x();
		int y = m_monsters[i]->get_y();
		char symbol = m_monsters[i]->get_symbol();
		m_grid[y][x] = symbol;
	}
	for (int i = 0; i < m_num_objects; i++)	//Draw every item in the item vector
	{
		int x = m_objects[i]->get_x();
		int y = m_objects[i]->get_y();
		if (m_grid[y][x] == ' ')
			m_grid[y][x] = m_objects[i]->get_symbol();
	}
	
	if (m_grid[m_exit->get_y()][m_exit->get_x()] == ' ')	//Draw the exit
		m_grid[m_exit->get_y()][m_exit->get_x()] = m_exit->get_symbol();
	

}

void Level::display()
{
	for (int i = 0; i < 18; i++)	//Print every cell in the level
	{
		for (int j = 0; j < 70; j++)
		{
			std::cout << m_grid[i][j];
		}
		std::cout << std::endl;

	}
	display_stats();	//Display current statistics

	std::cout << message_buffer << endl;	//Print the message buffer, resetting it afterwards
	message_buffer = "";
}

void Level::player_move()	//Process the player turn
{
	if (p->get_sleep() > 0)	//Pass the turn if the player is asleep
	{
		p->sleep_pass();
		getCharacter();
		return;
	}

	int roll = randInt(1, 10);	//10% chance to heal per turn
	if (roll == 1)
		p->add_hp(1);

	char input = getCharacter();
	string response = p->input_handler(input);	//Run the input through the player's input handler
	if (input == 'i' || input == 'I')
		return;		//exit the function if the player's viewed the inventory through the input handler

	if (response != "")
	{
		message_buffer = response;	//add the corresponding message if the player equiped an item through the input handler
		return;	
	}
			
	

	switch (input) //Process the corresponding for each action (For moves, attempt an attack in the direction, and a move if an attack is invalid)
	{
	case 'h':
	case 'H':
		if (valid_attack('h', p) == true)
			player_attack(p, p->get_x() - 1, p->get_y());
		else if (valid_move('h', p) == true)
			p->move('h');
		return;
	case 'j':
	case 'J':
		if (valid_attack('j', p) == true)
			player_attack(p, p->get_x(), p->get_y() + 1);
		else if (valid_move('j', p) == true)
			p->move('j');
		return;
	case 'k':
	case 'K': 
		if (valid_attack('k', p) == true)
			player_attack(p, p->get_x(), p->get_y() - 1);
		else if (valid_move('k', p) == true)
			p->move('k');
		return;
	case 'l':
	case 'L':
		if (valid_attack('l', p) == true)
			player_attack(p, p->get_x() + 1, p->get_y());
		else if (valid_move('l', p) == true)
			p->move('l');
		return;
	case 'g':
	case 'G':
		message_buffer += p->pickup(getItem());	//Execute the player's pickup command with the item at the current location and add the corresponding message to the buffer
		return;
	case '>':
		if (m_stage != 4 && p->get_x() == m_exit->get_x() && p->get_y() == m_exit->get_y())	//Check if the player is standing on the level ender, ending the level if so
			level_complete = 1;
		return;
	case 'q':
	case 'Q':
		level_complete = 1;
		level_quit = 1;	//Set the status of the level to "finished/Quit" if the player presses Q
		return;
	case 'c':
	case 'C':
		p->cheat();	//Execute the player's cheat function
		return;
	default:
		return;
	}
}

void Level::monster_move()
{
	if (level_complete)	//Do nothing if the level is over
		return;
	for (int i = 0; i < m_num_monster; i++)	//Loop through every monster in the monster vector
	{
		if (m_monsters[i]->get_sleep() > 0)	//Do nothing if the monster is asleep
		{
			m_monsters[i]->sleep_pass();
			return;
		}

		Monster* current = m_monsters[i];
		Goblin* check = dynamic_cast<Goblin*>(current);	//Setup check for if the current monster is a goblin
		if (valid_attack('h', current) || valid_attack('j', current) || valid_attack('k', current) || valid_attack('l', current))	//Attempt an attack on adjacent spaces
		{
			monster_attack(current);
		}
		else if (check != nullptr)	//Check if the current monster is a goblin
		{
			int dist_x = abs(p->get_x() - check->get_x());
			int dist_y = abs(p->get_y() - check->get_y());
			if (dist_x <= check->get_smell() && dist_y <= check->get_smell())	//Only run the smell algorithm if the goblin has a rough possibility to be in range
			{
				char move = check->smell(p, m_grid);
				if (valid_move(move, check))
					check->move(move);	//attempt to move in the direction returned by the goblin's smell function
			}
		}
		else if (valid_move(current->smell(m_grid, p), current))	//attempt to move in the direction returned by the general monster's smell function
		{
			current->move(current->smell(m_grid, p));
		}
		update();	//udpate the board
	}
}

bool Level::level_status()
{
	return level_complete;
}

bool Level::exit_status()
{
	return level_quit;
}

//Private functions

bool Level::valid_move(char dir, Actor* a)	//Determines whether an attempted move is legal, given the actor's current surroundings
{
	update();	//Update the board


	//Check the given direction for an opening

	if (dir == 'h')
	{
		char left = m_grid[a->get_y()][a->get_x() - 1];
		if (left == ' ' || left == ')' || left == '?' || left == '>' || left == '&')
			return true;
	}
	else if (dir == 'j')
	{
		char down = m_grid[a->get_y() + 1][a->get_x()];
		if (down == ' ' || down == ')' || down == '?' || down == '>' || down == '&')
			return true;
	}
	else if (dir == 'k')
	{
		char up = m_grid[a->get_y() - 1][a->get_x()];
		if (up == ' ' || up == ')' || up == '?' || up == '>' || up == '&')
			return true;
	}
	else if (dir == 'l')
	{
		char right = m_grid[a->get_y()][a->get_x() + 1];
		if (right == ' ' || right == ')' || right == '?' || right == '>' || right == '&')
			return true;
	}
	return false;
}

bool Level::valid_attack(char dir, Actor* a)
{
	char enemies[] = { 'B', 'S', 'G', 'D' };
	int size;
	char player[] = { '@' };
	char* start;

	Player* check = dynamic_cast<Player*>(a); //Set the corresponding array of attackable enemies, given the attacker's type
	if (check != nullptr)
	{
		start = enemies;
		size = 4;
	}
	else
	{
		start = player;
		size = 1;
	}

	//For the given inputted direction, check if there is an enemy in the corresponding space

	if (dir == 'h')
	{
		for (int i = 0; i < size; i++)
		{
			if (m_grid[a->get_y()][a->get_x() - 1] == *(start + i))
				return true;
		}
	}
	else if (dir == 'j')
	{
		for (int i = 0; i < size; i++)
		{
			if (m_grid[a->get_y() + 1][a->get_x()] == *(start + i))
				return true;
		}
	}
	else if (dir == 'k')
	{
		for (int i = 0; i < size; i++)
		{
			if (m_grid[a->get_y() - 1][a->get_x()] == *(start + i))
				return true;
		}
	}
	else if (dir == 'l')
	{
		for (int i = 0; i < size; i++)
		{
			if (m_grid[a->get_y()][a->get_x() + 1] == *(start + i))
				return true;
		}
	}
	return false;	//Return false if there is no enemy or if the input is invalid
}

void Level::player_attack(Player* p, int x, int y)
{
	Monster* target = nullptr;
	for (int i = 0; i < m_num_monster; i++)	//Identify which monster in the monster vector is being attacked, given the passed coordinates (Guaranteed to be one, as this function is only called after valid_attack)
	{
		if (m_monsters[i]->get_x() == x && m_monsters[i]->get_y() == y)
			target = m_monsters[i];
	}
	message_buffer += p->attack(target);	//Add the corresponding attack message to the message buffer and execute the attack
	if (target->isDead() == true)	//Check if the target is dead
	{
		int death_x = target->get_x();
		int death_y = target->get_y();
		for (int i = 0; i < m_num_objects; i++)	//Check the item vector to see if the monster was standing on an item
		{
			if (m_objects[i]->get_x() == death_x && m_objects[i]->get_y() == death_y)
				return;	//Exit the function if there was an item in the same spot
		}
		Object* dropped = target->onDeathDrop(); //Get the dropped item by executing the monster's onDeathDrop function
		if (dropped != nullptr)	//If a valid item was dropped
		{
			m_objects.push_back(dropped);	//Add the item to the item vector
			m_num_objects++;
		}
	}

	return;
		
	
}

void Level::monster_attack(Monster* m)
{
	message_buffer += m->attack(p);	//Attack the player
}

Object* Level::getItem()	//Gets the item at the current player location
{
	if (m_stage == 4 && p->get_x() == m_exit->get_x() && p->get_y() == m_exit->get_y())	//Only check for the golden idol if the current level is 4
	{
		level_complete = 1;	//End the game if the golden idol is picked up
		std::cout << "You pick up the golden idol" << endl << "Congratulations, you won!";
		return nullptr;
	}

	Object* onItem = nullptr;	//Check every item in the item vector to see if it is in the same location as the player, returning it if so
	for (int i = 0; i < m_num_objects; i++)
	{
		if (m_objects[i]->get_x() == p->get_x() && m_objects[i]->get_y() == p->get_y())
		{
			onItem = m_objects[i];
			m_objects[i]->toggleStatus();
		}

	}
	return onItem;
}

void Level::spawn_monsters(int goblinSmellDistance, int stage)	//Spawn initial monsters upon level creation
{
	int monster_gen = randInt(2, 5 * (stage + 1) + 1);	//Decide how many monsters to spawn, given the current level

	for (int i = 0; i < monster_gen; i++)	//Loop until enough monsters are generated
	{
		int x;	//Randomly generate a spawn location
		int y;
		do
		{
			x = randInt(1, 68);
			y = randInt(1, 16);
			for (int i = 0; i < m_num_monster; i++)	//Check if the current randomized spawn location is already occupied by another monster
			{
				if (m_monsters[i]->get_x() == x && m_monsters[i]->get_y() == y)	
				{
					x = randInt(1, 68);
					y = randInt(1, 16);
				}
			}
		} while (m_grid[y][x] != ' ');	//Rerun if the spawn location is not empty

		int gen_id;
		if (stage >= 3)	//Randomly select which type of monster to spawn, given the current level 
			gen_id = randInt(0, 3);
		else if (stage >= 2)
			gen_id = randInt(0, 2);
		else
			gen_id = randInt(0, 1);
		switch (gen_id)	//Spawn the corresponding monster 
		{
		case 0:
		{
			Goblin* g = new Goblin(x, y, goblinSmellDistance);
			m_monsters.push_back(g);
			m_num_monster++;
		}
			break;
		case 1:
		{
			Snakewoman* s = new Snakewoman(x, y);
			m_monsters.push_back(s);
			m_num_monster++;
		}
			break;
		case 2:
		{
			Bogeyman* b = new Bogeyman(x, y);
			m_monsters.push_back(b);
			m_num_monster++;
		}
			break;
		case 3:
		{
			Dragon* d = new Dragon(x, y);
			m_monsters.push_back(d);
			m_num_monster++;
		}
			break;
		}
	}

}

void Level::spawn_items()
{
	int item_gen = randInt(2, 3);	//Decide how many items to spawn
	for (int i = 0; i < item_gen; i++)	//Loop until enough items are spawned
	{
		int x;	
		int y;
		do
		{
			x = randInt(1, 68); //Randomly generate a starting location
			y = randInt(1, 16);
			for (int j = 0; j < m_num_objects; j++)	//Check the coordinates of every existing item to see if the randomly generated location overlaps, regenerating if so
			{
				if (m_objects[j]->get_x() == x && m_objects[j]->get_y() == y)
				{
					x = randInt(1, 68);
					y = randInt(1, 16);
				}
			}
		} while (m_grid[y][x] == '#');	//Rerun if the random location is a wall

		int gen_id = randInt(0, 6);	//Randomly decide which item to generate
		switch (gen_id)	//Generate the corresponding item
		{
		case 0:
		{
			S_Sword* s = new S_Sword(x, y, 1);
			m_objects.push_back(s);
			m_num_objects++;
		}
		break;
		case 1:
		{
			L_Sword* l = new L_Sword(x, y, 1);
			m_objects.push_back(l);
			m_num_objects++;
		}
		break;
		case 2:
		{
			Mace* m = new Mace(x, y, 1);
			m_objects.push_back(m);
			m_num_objects++;
		}
		break;
		case 3:
		{
			Armor* ar = new Armor(x, y, 1);
			m_objects.push_back(ar);
			m_num_objects++;
		}
		break;
		case 4:
		{
			Strength* st = new Strength(x, y, 1);
			m_objects.push_back(st);
			m_num_objects++;
		}
		break;
		case 5:
		{
			Health* h = new Health(x, y, 1);
			m_objects.push_back(h);
			m_num_objects++;
		}
		break;
		case 6:
		{
			Dexterity* d = new Dexterity(x, y, 1);
			m_objects.push_back(d);
			m_num_objects++;
		}
		break;
		}

	}

	//After all the items are generated, spawn the ending object (> or &)

	int exit_x;
	int exit_y;
	do
	{
		exit_x = randInt(1, 68);	//Randomly generate a spawning position
		exit_y = randInt(1, 16);

		for (int i = 0; i < m_num_objects; i++) //Check the coordinates of every existing item to see if the randomly generated location overlaps, regenerating if so
		{
			if (m_objects[i]->get_x() == exit_x && m_objects[i]->get_y() == exit_y)
			{
				exit_x = randInt(1, 68);
				exit_y = randInt(1, 16);
			}
		}
	} while (m_grid[exit_y][exit_x] == '#');	//Rerun if the spawn location is a wall

	if (m_stage == 4)
		m_exit = new Object("Congratulations, you won!", "idol", '&', exit_x, exit_y, 1); //Construct an object with the features of the exit idol if the current level is 4
	else
		m_exit = new Object("", "stair", '>', exit_x, exit_y, 1); //Construct an object with the features of a stair if the current level is not 4
	
}

void Level::spawn_player(Player* p)
{
	int x;	//Randomly generate a spawn location
	int y;
	do
	{
		x = randInt(1, 68);
		y = randInt(1, 16);
		for (int i = 0; i < m_num_monster; i++)	//Check if the current randomized spawn location is already occupied by another actor
		{
			if (m_monsters[i]->get_x() == x && m_monsters[i]->get_y() == y)
			{
				x = randInt(1, 68);
				y = randInt(1, 16);
			}
		}
	} while (m_grid[y][x] != ' ' && m_grid[y][x] != '>' && m_grid[y][x] != ')' && m_grid[y][x] != '?' && m_grid[y][x] != '&');	//Rerun if the spawn location is not empty is not a valid space
	p->set_pos(x, y);	//Set the player's position to the generated location
}

void Level::display_stats()	//Gets the player's current statistics and displays them
{
	string stats;	
	stats += "Dungeon Level: ";
	stats += to_string(m_stage);
	stats += ", Hit points: ";
	stats += to_string(p->get_hp());
	stats += ", Armor: ";
	stats += to_string(p->get_amr());
	stats += ", Strength: ";
	stats += to_string(p->get_str());
	stats += ", Dexterity: ";
	stats += to_string(p->get_dex());
	std::cout << stats << endl;
}

void Level::generate_grid()	//Generates a playing field
{
	for (int i = 0; i < 18; i++)	//Generate a completely filled grid
	{
		for (int j = 0; j < 70; j++)
		{
			m_grid[i][j] = '#';
		}
	}

	//Room generation

	vector<room> rooms;
	int numRooms = randInt(4, 7);	//Randomly generate the number of rooms
	while (rooms.size() < numRooms)	//Loop until we have generated enough rooms
	{
		bool passed = 0;	//Mark the default state of the overlap test as failure
		room Newroom;	//Generate a new room
		while (passed != 1)	//Perform a check to see if the space that would be occupied by the room already has a room/part of a room (empty space)
		{
			bool success = 1;	//Room is currently passing the overlap test until an overlap is found
			for (int i = Newroom.top() - 1; i < Newroom.bottom() + 1; i++) //Loop through the entire space of the new room
			{
				for (int j = Newroom.left() - 1; j < Newroom.right() + 1; j++)
				{
					if (m_grid[i][j] == ' ')
						success = 0;	//Mark the room as a failure if an empty space is encountered
				}
			}
			if (success == 1)	//If no overlaps were found, the test overlap test was passed
				passed = 1;
			else
				Newroom = room();	//If the overlap test failed, generate a new room
		}

		rooms.push_back(Newroom);	//Add the new room

		for (int i = Newroom.top(); i < Newroom.bottom(); i++)	//Fill the space occupied by the room with empty space
		{
			for (int j = Newroom.left(); j < Newroom.right(); j++)
			{
				m_grid[i][j] = ' ';

			}
		}
	}

	for (int i = 0; i < rooms.size(); i++)	//Generating corridors between each room
	{	
		int r_tun;	//tunneling to the right
		for (int j = 0; j < rooms.size(); j++) //Loop through the room vector
		{
			if (j != i)	//if the current room is not the checked room
			{
				bool vertical = (rooms[j].left() > rooms[i].right());	//Check that the rooms are vertically aligned
				bool horizontal_a = (rooms[j].top() <= rooms[i].top() && rooms[j].bottom() >= rooms[i].bottom());	//Check that the rooms overlap horizontally, in various different ways 
				bool horizontal_b = (rooms[j].top() >= rooms[i].top() && rooms[j].bottom() <= rooms[i].bottom());	
				bool horizontal_c = (rooms[j].top() < rooms[i].top() && rooms[j].bottom() > rooms[i].top());
				bool horizontal_d = (rooms[j].top() < rooms[i].bottom() && rooms[j].bottom() > rooms[i].bottom());
				bool dist = (rooms[j].left() - rooms[i].right() < 15);
				if (vertical && horizontal_a && dist) //If the room found is to the right, has horizontal overlap a, and is within range
				{
					r_tun = randInt(rooms[i].top() + 1, rooms[i].bottom() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].right(); k <= rooms[j].left(); k++)	//"tunnel" until the room is reached
					{
						m_grid[r_tun][k] = ' ';	
					}
					break;
				}
				else if (vertical && horizontal_b && dist)	//If the room found is to the right, has horizontal overlap b, and is within range
				{
					r_tun = randInt(rooms[j].top() + 1, rooms[j].bottom() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].right(); k <= rooms[j].left(); k++)	//"tunnel" until the room is reached
					{	
						m_grid[r_tun][k] = ' ';
					}
					break;
				}
				else if (vertical && horizontal_c && dist)	//If the room found is to the right, has horizontal overlap c, and is within range
				{
					r_tun = randInt(rooms[i].top() + 1, rooms[j].bottom() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].right(); k <= rooms[j].left(); k++)	//"tunnel" until the room is reached
					{
						m_grid[r_tun][k] = ' ';
					}
					break;
				}
				else if (vertical && horizontal_d && dist)	//If the room found is to the right, has horizontal overlap d, and is within range
				{
					r_tun = randInt(rooms[j].top() + 1, rooms[i].bottom() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].right(); k <= rooms[j].left(); k++)	//"tunnel" until the room is reached
					{
						m_grid[r_tun][k] = ' ';
					}
					break;
				}
			}
		}


		int d_tun;	//tunneling downwards
		for (int j = 0; j < rooms.size(); j++)	//Loop through the room vector
		{
			if (j != i)	//if the current room is not the checked room
			{
				bool horizontal = (rooms[j].top() > rooms[i].bottom());	//Check that the rooms are horizontally aligned
				bool vertical_a = (rooms[j].left() <= rooms[i].left() && rooms[j].right() >= rooms[i].right());	//Check that the rooms overlap vertically, in various ways 
				bool vertical_b = (rooms[j].left() > rooms[i].left() && rooms[j].right() < rooms[i].right());
				bool vertical_c = (rooms[j].left() > rooms[i].left() && rooms[j].left() < rooms[i].right());
				bool vertical_d = (rooms[j].right() > rooms[i].left() && rooms[j].right() < rooms[i].right());
				bool dist = (rooms[j].top() - rooms[i].bottom() < 6);
				if (horizontal && vertical_a && dist)	//If the room found is underneath the current room, has vertical overlap a, and is within range
				{
					d_tun = randInt(rooms[i].left() + 1, rooms[i].right() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].bottom(); k <= rooms[j].top(); k++)	//"tunnel" until the room is reached
					{
						m_grid[k][d_tun] = ' ';
					}
					break;
				}
				else if (horizontal && vertical_b && dist)	//If the room found is underneath the current room, has vertical overlap a, and is within range
				{
					d_tun = randInt(rooms[j].left() + 1, rooms[j].right() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].bottom(); k <= rooms[j].top(); k++)	//"tunnel" until the room is reached
					{
						m_grid[k][d_tun] = ' ';
					}
					break;
				}
				else if (horizontal && vertical_c && dist)	//If the room found is underneath the current room, has vertical overlap a, and is within range
				{
					d_tun = randInt(rooms[j].left() + 1, rooms[i].right() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].bottom(); k <= rooms[j].top(); k++)	//"tunnel" until the room is reached
					{
						m_grid[k][d_tun] = ' ';
					}
					break;
				}
				else if (horizontal && vertical_d && dist)	//If the room found is underneath the current room, has vertical overlap a, and is within range
				{
					d_tun = randInt(rooms[i].left() + 1, rooms[j].right() - 1);	//generate a tunneling spot within the overlap
					for (int k = rooms[i].bottom(); k <= rooms[j].top(); k++)	//"tunnel" until the room is reached
					{
						m_grid[k][d_tun] = ' ';
					}
					break;
				}
			}
		}
	}
}

void Level::test_grid()
{

	while (1 == 1)	//Run until the grid passes the check
	{
		generate_grid();

		int start_x = 0;
		int start_y = 0;
		bool detected = 0;

		for (int i = 0; i < 18; i++)	//Find an empty space from which to probe the grid from
		{
			for (int j = 0; j < 70; j++)
			{
				if (m_grid[i][j] == ' ')
				{
					start_x = j;
					start_y = i;
					break;
				}
			}
			if (start_x != 0)
				break;
		}

		grid_fill(start_y, start_x);	//Fill the grid, starting from the initial space

		for (int i = 0; i < 18; i++)	//Check each space on the grid, flagging the grid if the there are any unmarked empty spaces
		{
			for (int j = 0; j < 70; j++)
			{
				if (m_grid[i][j] == ' ')
				{
					detected = 1; //Trigger the flag and exit the loop if an empty space is detected
					break;	
				}
				else if (m_grid[i][j] == 'O')	//Erase the "paint"
				{
					m_grid[i][j] = ' ';
				}
			}
			if (detected == 1)	
				break;	//Exit the loop if the flag is triggerred
		}

		if (detected == 0)	//If the flag wasn't triggered, exit the loop
			break;
	}

	
}

void Level::grid_fill(int sr, int sc)
{
	//Adapted from mazeq from HW2
	//Similar behavior, but this version simply marks every traversable coordinate, instead of looking for a path

	queue<Coord> coordQueue;

	Coord startingPos(sr, sc);
	coordQueue.push(startingPos);
	m_grid[sr][sc] = 'O';

	while (!coordQueue.empty())	//Run until there are no more possible moves
	{
		Coord currentPos = coordQueue.front();
		coordQueue.pop();

		if (m_grid[currentPos.r() + 1][currentPos.c()] == ' ')
		{
			coordQueue.push(Coord(currentPos.r() + 1, currentPos.c()));
			m_grid[currentPos.r() + 1][currentPos.c()] = 'O';
		}
		if (m_grid[currentPos.r()][currentPos.c() - 1] == ' ')
		{
			coordQueue.push(Coord(currentPos.r(), currentPos.c() - 1));
			m_grid[currentPos.r()][currentPos.c() - 1] = 'O';
		}
		if (m_grid[currentPos.r() - 1][currentPos.c()] == ' ')
		{
			coordQueue.push(Coord(currentPos.r() - 1, currentPos.c()));
			m_grid[currentPos.r() - 1][currentPos.c()] = 'O';
		}
		if (m_grid[currentPos.r()][currentPos.c() + 1] == ' ')
		{
			coordQueue.push(Coord(currentPos.r(), currentPos.c() + 1));
			m_grid[currentPos.r()][currentPos.c() + 1] = 'O';
		}
	}

}