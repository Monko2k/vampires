#include <iostream>
#include "Actors.h"
#include "Objects.h"
#include "Utilities.h"
#include "Utilities_S.h"

#include <queue>


//Actor Implementation

//Actor public functions

Actor::Actor(int health, Weapon* wielding, int amr, int str, int dex, char symbol, string name)	//Actor constructor
{
	m_name = name;
	m_hp = health;
	m_hp_max = health;
	current = wielding;
	m_amr = amr;
	m_str = str;
	m_dex = dex;
	m_symbol = symbol;
	m_sleep = 0;
}

Actor::~Actor()	//Actor destructor
{
	delete current;
}

string Actor::attack(Actor* target)	//Processes attacks from one actor onto another
{
	string response = "\n" + get_name() + " " + current->action() + " " + current->name() + " at " + target->get_name();	//Set up return string
	int attackerPoints = m_dex + current->get_dex();	//Calculate attack math based on spec
	int defenderPoints = target->get_dex() + target->get_amr();
	int damage = -(randInt(0, get_str() + current->get_dam() - 1));
	if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))	//Execute if the attack is considered a "hit"
	{
		target->add_hp(damage);

		Fang* fang_check = dynamic_cast<Fang*>(current);	//Setup check to see if attacker is wielding magic fangs

		if (fang_check == nullptr)	//If the attacker is not wielding fangs
		{
			if (target->isDead())	//Check if the target is dead after the attack, and appending the corresponding string to the return string
				response += " dealing a final blow.";	
			else
				response += " and hits.";
		}
		else	//If the attacker is wielding fangs
		{
			int roll = randInt(1, 5);	//Roll to decide if the target sleeps
			if (target->isDead())	//If the target dies, append the corresponding string to the return string
				response += " dealing a final blow.";	
			else if (roll == 1)	//If the target doesn't die, and the attacker rolls successfully
			{
				response += " and hits, putting " + target->get_name() + " to sleep.";	//Append the corresponding string to the return string
				target->set_sleep(randInt(2, 6));	//Set the target's sleep timer 
			}
			else
				response += " and hits.";	//If the attacker rolls unsuccessfully, append the corresponding string to the return string
		}
	}
	else	//If the target rolls unsuccessfully, thus missing
	{
		response += " and misses.";	//Append the corresponding string
	}
	return response;	//return the return string
}





//Actor getter functions

int Actor::get_x()
{
	return x_pos;
}

int Actor::get_y()
{
	return y_pos;
}

int Actor::get_hp()
{
	return m_hp;
}

int Actor::get_dex()
{
	return m_dex;
}

int Actor::get_amr()
{
	return m_amr;
}

int Actor::get_str()
{
	return m_str;
}

int Actor::get_sleep()
{
	return m_sleep;
}

int Actor::get_hp_max()
{
	return m_hp_max;
}

string Actor::get_name()
{
	return m_name;
}

Weapon* Actor::get_current()
{
	return current;
}

bool Actor::isDead()
{
	if (m_hp <= 0)
		return true;
	return false;
}

//Actor setter fuctions

void Actor::set_pos(int x, int y)
{
	x_pos = x;
	y_pos = y;
}

void Actor::add_hp(int amt)
{
	if (m_hp + amt > m_hp_max)
		m_hp = m_hp_max;
	else
		m_hp += amt;
}

void Actor::set_hp_max(int amt)
{
	m_hp_max = amt;
}

void Actor::add_hp_max(int amt)
{
	m_hp_max += amt;
}

void Actor::add_amr(int amt)
{
	m_amr += amt;
}

void Actor::add_str(int amt)
{
	m_str += amt;
}

void Actor::set_str(int amt)
{
	m_str = amt;
}

void Actor::add_dex(int amt)
{
	m_dex += amt;
}

void Actor::move(char dir)
{
	if (dir == 'k')
		y_pos--;
	if (dir == 'l')
		x_pos++;
	if (dir == 'j')
		y_pos++;
	if (dir == 'h')
		x_pos--;
}

char Actor::get_symbol()
{
	return m_symbol;
}

void Actor::set_sleep(int amt)
{
	if (m_sleep < amt)
		m_sleep = amt;
}

void Actor::setWeapon(Weapon* weapon)
{
	if (current == nullptr)
	{
		current = weapon;
		m_dex += weapon->get_dex();
	}
	else
	{
		m_dex -= current->get_dex();
		current = weapon;
		m_dex += weapon->get_dex();
	}
}

void Actor::sleep_pass()	//Decrements the actor's sleep timer
{
	m_sleep--;
}

//Player implementation

//Player public functions
Player::Player()	//Player constructor
	:Actor(20, nullptr, 2, 2, 2, '@', "Player")	//Construct the player with out a weapon
{
	Weapon* starting = new S_Sword(0, 0, 0);	//Make a new weapon
	setWeapon(starting);	//Set the weapon
	m_inventory.push_back(starting);
	used_teleport = 0;
}

Player::~Player() //Player destructor
{
	for (int i = 0; i < m_inventory.size(); i++) //Delete every item in the inventory vector
	{
		if (m_inventory[i] != get_current())
			delete m_inventory[i];
	}
}

void Player::cheat()	//Buffs the player
{
	set_hp_max(50);
	add_hp(50);
	set_str(9);
}

string Player::pickup(Object* item)	//Attempts to add an item passed by the level's getItem function to the inventory, and returns a corresponding string
{
	if (item == nullptr) //Return an empty string and do nothing if a nullptr was passed
		return "";

	if (m_inventory.size() <= 25)	//If the inventory still has space
	{
		m_inventory.push_back(item);	//Add the item to the inventory
		if (item->get_symbol() == '?')	//If the item was a scroll
			return "\nYou pick up a scroll called " + item->name(); //Return the corresponding string
		else	//If the item was not a scroll
			return "\nYou pick up " + item->name();	//Return the corresponding string
	}
	return "Your knapsack is full; you can't pick that up.";	//Returns message if the the inventory was full
	
}

void Player::toggle_teleport()
{
	used_teleport = !used_teleport;
}

bool Player::teleport_status()
{
	return used_teleport;
}

void Player::useScroll(Scroll* s) //Executes a scroll's effects
{
	int id = s->get_type();	//Gets the type # of the scroll
	switch (id)	//Execute the corresponding action, given the type number
	{
	case 0:
		toggle_teleport();
		break;
	case 1:
		add_amr(randInt(1, 3));
		break;
	case 2:
		add_str(randInt(1, 3));
		break;
	case 3:
		add_hp_max(randInt(3, 8));
		break;
	case 4:
		add_dex(1);
	}
	delete s;
}

void Player::viewInventory()	//Prints the player's inventory
{
	char index = 'a';
	clearScreen();
	std::cout << "Inventory:" << endl;
	for (int i = 0; i < m_inventory.size(); i++)	//Loop through the entire inventory vector
	{
		Scroll* current_item = dynamic_cast<Scroll*>(m_inventory[i]);	//Setup check for if the current item is a scroll
		if (current_item != nullptr)	//If the current item is a scroll
			std::cout << ' ' << index << ". A scroll called " << m_inventory[i]->name() << endl; 
		else //If the current item is not a scroll
			std::cout << ' ' << index << ". " << m_inventory[i]->name() << endl;
		index++; //Increment the alphabetical index
	}

}

string Player::wield()
{
	viewInventory();
	string response;
	char selection = getCharacter();
	if (int(selection) - 96 > 0) //Checking if the input value is lowercase
	{
		int converted = (int)selection - 97; //Shifts the index of the alphabet to fit the index of the inventory
		if (converted < m_inventory.size())
		{
			Weapon* equip_item = dynamic_cast<Weapon*>(m_inventory[converted]);	//Checks if the item is a weapon
			if (equip_item != nullptr)	//If the item is a weapon
			{
				setWeapon(equip_item);
				response = "\nYou are wielding " + m_inventory[converted]->name();	//Set the return string
			}
			else	//If the item is not a weapon
			{
				response = "\nYou can't wield " + m_inventory[converted]->name();	//Set the returns tring
			}
		}
		else	//If the input was uppercase
		{
			int converted = (int)selection - 65;  //Shifts the index of the alphabet to fit the index of the inventory
			if (converted < m_inventory.size())
			{
				Weapon* equip_item = dynamic_cast<Weapon*>(m_inventory[converted]); //Checks if the item is a weapon
				if (equip_item != nullptr)	//if the item was a weapon
				{
					setWeapon(equip_item);
					response = "\nYou are wielding " + m_inventory[converted]->name();	//Set the return string
				}
				else	//If the item was not a weapon
				{
					response = "\nYou can't wield " + m_inventory[converted]->name();	//Set the return string
				}
			}
		}

	}
	return response;

}

string Player::read()
{
	viewInventory();
	string response;
	char selection = getCharacter();
	if (int(selection) - 96 > 0) //Checking if the input value is lowercase
	{
		int converted = (int)selection - 97; //Shifts the index of the alphabet to fit the index of the inventory
		if (converted < m_inventory.size())
		{
			Scroll* equip_item = dynamic_cast<Scroll*>(m_inventory[converted]); //Checks if the item is a scroll
			if (equip_item != nullptr)
			{
				response = "\nYou read the scroll called " + m_inventory[converted]->name();	//Set the return string
				response += "\n" + equip_item->action();
				m_inventory.erase(m_inventory.begin() + converted);
				useScroll(equip_item);	//Pass the scroll to the useScroll function to consume it
			}
			else
			{
				response = "\nYou can't read a " + m_inventory[converted]->name();	//Set the return string
			}
		}
		else	//If the input was uppercase
		{
			int converted = (int)selection - 65; //Shifts the index of the alphabet to fit the index of the inventory
			if (converted < m_inventory.size())
			{
				Scroll* equip_item = dynamic_cast<Scroll*>(m_inventory[converted]); //Checks if the item is a scroll
				if (equip_item != nullptr)
				{
					response = "\nYou read the scroll called " + m_inventory[converted]->name(); //Set the return string
					response += "\n" + equip_item->action();
					m_inventory.erase(m_inventory.begin() + converted);
					useScroll(equip_item); //Pass the scroll to the useScroll function to consume it
				}
				else
				{
					response = "\nYou can't read a " + m_inventory[converted]->name();	//Set the return string
				}
			}
		}

	}
	return response;

}

string Player::input_handler(char input) //Called upon by the level player_move function to handle inventory management
{
	switch (input) //Perform the action corresponding to each keyboard input
	{
	case 'i':
	case 'I':
	{
		viewInventory();
		getCharacter();
		clearScreen();
		return "viewed_inv";
	}
	case 'w':
	case 'W':
	{
		return wield();;
	}
	case 'r':
	case 'R':
	{
		return read();
	}
	default:
		return "";

	}

}

//Monster implementation

Monster::Monster(int health, Weapon* wielding, int amr, int str, int dex, int smell, char symbol, string name) //Monster constructor
	:Actor(health, wielding, amr, str, dex, symbol, name)
{
	m_smell_distance = smell;
}

Monster::~Monster()	//Monster destructor
{}

int Monster::get_smell()
{
	return m_smell_distance;
}

char Monster::smell(char grid[18][70], Player* p)	//Returns a direction corresponding to an input if a player is detected
{
	int t_x = p->get_x();
	int t_y = p->get_y();
	int s_x = get_x();
	int s_y = get_y();

	bool detected = 0;
	for (int i = 0; i <= m_smell_distance; i++)	//Loop from maximum smell distance to zero
	{
		int dist = i;
		for (int j = -dist; j <= 0; j++)	//check the northwest line of spaces i spaces away from the monster
		{
			if (s_x + j == t_x && s_y + (dist + j) == t_y)
				detected = 1;
		}	
		for (int j = 0; j <= dist; j++)		//check the northeast line of spaces i spaces away from the monster
		{
			if (s_x + j == t_x && s_y + (dist - j) == t_y)
				detected = 1;
		}
		for (int j = -dist; j <= 0; j++)	//check the southwest line of spaces i spaces away from the monster
		{
			if (s_x + j == t_x && s_y - (dist + j) == t_y)
				detected = 1;
		}
		for (int j = 0; j <= dist; j++)		//check the southeast line of spaces i spaces away from the monster
		{
			if (s_x + j == t_x && s_y - (dist - j) == t_y)
				detected = 1;
		}
		if (detected == 1) break;
	}
		
	if (detected == 1)	//If the player was detected
	{
		char north = grid[s_y - 1][s_x];
		char east = grid[s_y][s_x + 1];
		char south = grid[s_y + 1][s_x];
		char west = grid[s_y][s_x - 1];

		bool validnorth = (north == ' ' || north == '?' || north == ')' || north == '>' || north == '&' || north == '@');
		bool valideast = (east == ' ' || east == '?' || east == ')' || east == '>' || east == '&' || east == '@');
		bool validsouth = (south == ' ' || south == '?' || south == ')' || south == '>' || south == '&' || south == '@');
		bool validwest = (west == ' ' || west == '?' || west == ')' || west == '>' || west == '&' || west == '@');

		if (p->get_x() - get_x() > 0 && valideast )	//If moving east is a possible/logical movement
			return 'l';
		else if (p->get_x() - get_x() < 0 && validwest)	//If moving west is a possible/logical movement
			return 'h';
		else if (p->get_y() - get_y() > 0 && validsouth)	//If moving south is a possible/logical movement
			return 'j';
		else if (p->get_y() - get_y() < 0 && validnorth)	//If moving north is a possible/logical movement
			return 'k';
	}

	return 'x';	//Return 'x' (invalid input to the level's attempt_move function) if no player was detected or no reasonable move could be made

}

//Bogeyman implementation

Bogeyman::Bogeyman(int x, int y)	//Bogeyman constructor
	:Monster(randInt(5,10), nullptr, randInt(2,3), randInt(2,3), 2, 5, 'B', "the Bogeyman")	//Construct without a weapon
{
	Weapon* starting = new S_Sword(0, 0, 0);	//Make a new weapon
	setWeapon(starting);	//Equip the weapon
	set_pos(x,y);
}

Bogeyman::~Bogeyman()	//Bogeyman destructor
{
}

Object* Bogeyman::onDeathDrop()	//Returns the object dropped upon death
{
	int roll = randInt(1, 10);	
	if (roll == 1)	//If rolled successfully
	{
		Axe* drop = new Axe(get_x(), get_y(), 1); //Create an axe at the current location
		return drop;	//return the axe
	}
	return nullptr;		//if rolled unsuccessfully, return nothing (nullptr)
}

//Snakewoman implementation

Snakewoman::Snakewoman(int x, int y)	//Snakewoman constructor
	:Monster(randInt(3, 6), nullptr, 2, 3, 3, 3, 'S', "the Snakewoman")	//Construct without a weapon
{
	Weapon* starting = new Fang(0, 0, 0);	//Make a new weapon
	setWeapon(starting);	//Equip the weapon
	set_pos(x, y);
}

Snakewoman::~Snakewoman()	//Snakewoman destructor
{
}

Object* Snakewoman::onDeathDrop()	//Returns the object dropped upon death
{
	int roll = randInt(1, 3);
	if (roll == 1)	//If rolled successfully
	{
		Fang* drop = new Fang(get_x(), get_y(), 1); //Create magic fangs at the current location
		return drop;	//Return the fangs
	}
	return nullptr;	//if rolled unsuccessfully, return nothing (nullptr)

}

//Dragon implementation

Dragon::Dragon(int x, int y)	//Dragon constructor
	:Monster(randInt(20, 25), nullptr, 4, 4, 4, 0, 'D', "the Dragon")	//Construct without a weapon
{
	Weapon* starting = new L_Sword(0, 0, 0);	//Make a new weapon
	setWeapon(starting);	//Equip the weapon
	set_pos(x, y);
}

Dragon::~Dragon()	//Dragon destructor
{
}

Object* Dragon::onDeathDrop()	//Returns the object dropped upon death	
{
	int roll = randInt(1, 5);
	switch (roll)	//Drops creates and drops a scroll at the current position, depending on what was rolled
	{
	case 1:
	{
		Teleport* drop = new Teleport(get_x(), get_y(), 1);
		return drop;
	}
	break;
	case 2:
	{
		Armor* drop = new Armor(get_x(), get_y(), 1);
		return drop;
	}
	break;
	case 3:
	{
		Strength* drop = new Strength(get_x(), get_y(), 1);
		return drop;
	}
	break;
	case 4:
	{
		Health* drop = new Health(get_x(), get_y(), 1);
		return drop;
	}
	break;
	case 5:
	{
		Dexterity* drop = new Dexterity(get_x(), get_y(), 1);
		return drop;
	}
	break;
	}
}


//Goblin implementation

Goblin::Goblin(int x, int y, int dist)	//Goblin constructor
	:Monster(randInt(15, 20), nullptr, 3, 1, 1, dist, 'G', "the Goblin") //Construct without a weapon
{
	Weapon* starting = new S_Sword(0, 0, 0);	//Create a new weapon
	setWeapon(starting);	//Equip the weapon
	set_pos(x, y);
}

Goblin::~Goblin()	//Goblin destructor
{
}

Object* Goblin::onDeathDrop()	//Returns the object dropped upon death
{
	int roll = randInt(1,6);	//Execute drop depending on roll
	if (roll == 1)	
	{
		Fang* drop = new Fang(get_x(), get_y(), 1);	//Create and drop a fang at the current location
		return drop;
	}
	else if (roll == 2)	
	{
		Axe* drop = new Axe(get_x(), get_y(), 1); //Create and drop an axe at the current location
		return drop;
	}
	return nullptr;	//If rolled unsuccessfully, return nothing (nullptr)

}

char Goblin::smell(Player* p, char maze[18][70])	//Returns the optimal direction of movement for a goblin
{
	
	int t_x = p->get_x();
	int t_y = p->get_y();
	int s_x = get_x();
	int s_y = get_y();

	queue<Coord> coordQueue;

	Coord startingPos(s_y, s_x, 0);
	coordQueue.push(startingPos);
	maze[s_y][s_x] = 'X';

	bool exist = pathExists(maze, t_y, t_x, coordQueue, get_smell());	//check if a path exists

	if (exist)
	{
		char north = maze[s_y - 1][s_x];
		char east = maze[s_y][s_x + 1];
		char south = maze[s_y + 1][s_x];
		char west = maze[s_y][s_x - 1];

		bool validnorth = (north == ' ' || north == '?' || north == ')' || north == '>' || north == '&' || north == '@' || north == 'X');
		bool valideast = (east == ' ' || east == '?' || east == ')' || east == '>' || east == '&' || east == '@' || east == 'X');
		bool validsouth = (south == ' ' || south == '?' || south == ')' || south == '>' || south == '&' || south == '@' || south == 'X');
		bool validwest = (west == ' ' || west == '?' || west == ')' || west == '>' || west == '&' || west == '@' || west == 'X');
		if (p->get_x() - get_x() > 0 && valideast)	//If moving east is a possible/logical movement
		{

			return 'l';

		}
		else if (p->get_x() - get_x() < 0 && validwest)	//If moving west is a possible/logical movement
		{

			return 'h';

		}
		else if (p->get_y() - get_y() > 0 && validsouth)	//If moving south is a possible/logical movement
		{

			return 'j';

		}
		else if (p->get_y() - get_y() < 0 && validnorth)	//If moving north is a possible/logical movement
		{


			return 'k'; 
		}
	}
	return 'x';




}


bool Goblin::pathExists(char path_maze[18][70], int er, int ec, queue<Coord> coordQueue, int max_step)
{
	if (coordQueue.size() == 0)	//If the queue is empty, return false
		return false;

	Coord currentPos = coordQueue.front();

	if (currentPos.r() == er && currentPos.c() == ec)	//If the player was reached, return true
		return true;
	coordQueue.pop();

	path_maze[currentPos.r()][currentPos.c()] = 'X';

	char up = path_maze[currentPos.r() - 1][currentPos.c()];
	char down = path_maze[currentPos.r() + 1][currentPos.c()];
	char left = path_maze[currentPos.r()][currentPos.c() - 1];
	char right = path_maze[currentPos.r()][currentPos.c() + 1];


	if (down == ' ' || down == '?' || down == ')' || down == '>' || down == '&' || down == '@')	//If move is possible, pass next space into queue
	{
		if (currentPos.s() < max_step)
		{
			path_maze[currentPos.r() + 1][currentPos.c()] = 'X';

			coordQueue.push(Coord(currentPos.r() + 1, currentPos.c(), currentPos.s() + 1));

		}

	}
	if (left == ' ' || left == '?' || left == ')' || left == '>' || left == '&' || left == '@')//If move is possible, pass next space into queue
	{
		if (currentPos.s() < max_step)
		{
			path_maze[currentPos.r()][currentPos.c() - 1] = 'X';
			coordQueue.push(Coord(currentPos.r(), currentPos.c() - 1, currentPos.s() + 1));

		}

	}
	if (up == ' ' || up == '?' || up == ')' || up == '>' || up == '&' || up == '@')
	{
		if (currentPos.s() < max_step)
		{
			path_maze[currentPos.r() - 1][currentPos.c()] = 'X';
			coordQueue.push(Coord(currentPos.r() - 1, currentPos.c(), currentPos.s() + 1));//If move is possible, pass next space into queue
		}
	}
	if (right == ' ' || right == '?' || right == ')' || right == '>' || right == '&' || right == '@')
	{
		if (currentPos.s() < max_step)
		{
			path_maze[currentPos.r()][currentPos.c() + 1] = 'X';
			coordQueue.push(Coord(currentPos.r(), currentPos.c() + 1, currentPos.s() + 1));//If move is possible, pass next space into queue

		}
	}

	return pathExists(path_maze, er, ec, coordQueue, max_step); //return result of operation given queue
}