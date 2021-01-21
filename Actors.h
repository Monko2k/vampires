// Actors.h

#ifndef ACTORS_H
#define ACTORS_H
#include "Objects.h"
#include <vector>
#include <queue>

// You may add data members and other member functions to this class.
class Level;
class Weapon;
class Coord;

class Actor
{
public:
	Actor(int health, Weapon* wielding, int amr, int str, int dex, char symbol, string name);
	virtual ~Actor();
	void move(char dir); 
	string attack(Actor* target);
	void setWeapon(Weapon* weapon);
	void add_hp(int amt);
	void set_pos(int x, int y);
	void add_hp_max(int amt);
	void set_hp_max(int amt);
	void set_str(int amt);
	void add_amr(int amt);
	void add_str(int amt);
	void add_dex(int amt);
	void set_sleep(int amt);
	void sleep_pass();

	bool isDead();
	int get_sleep();
	int get_x();
	int get_y();
	char get_symbol();
	int get_hp_max();
	int get_hp();
	int get_amr();
	int get_str();
	int get_dex();
	string get_name();
	Weapon* get_current();
private:
	string m_name;
	int m_sleep;
	int x_pos;
	int y_pos;
	int m_hp;
	Weapon* current;
	int m_amr;
	int m_str;
	int m_dex;
	int m_hp_max;
	int sleeptime;
	char m_symbol;

	
};



class Player : public Actor
{
public:
	Player();
	~Player();
	string wield();
	string pickup(Object* item);
	string read();
	bool teleport_status();
	void toggle_teleport();
	void viewInventory();
	void cheat();
	void useScroll(Scroll* s);
	string input_handler(char input);

private:
	bool used_teleport;
	std::vector<Object*> m_inventory;

};

class Monster : public Actor
{
public:
	Monster(int health, Weapon* wielding, int amr, int str, int dex, int smell, char symbol, string name);
	virtual ~Monster();
	virtual Object* onDeathDrop() = 0;
	virtual char smell(char grid[18][70], Player* p);
	int get_smell();
private:
	int m_smell_distance;
};

class Bogeyman : public Monster
{
public:
	Bogeyman(int x, int y);
	~Bogeyman();
	virtual Object* onDeathDrop();
};

class Snakewoman : public Monster
{
public:
	Snakewoman(int x, int y);
	~Snakewoman();
	virtual Object* onDeathDrop();
};
class Dragon : public Monster
{
public:
	Dragon(int x, int y);
	~Dragon();
	virtual Object* onDeathDrop();
};

class Goblin : public Monster
{
public:
	Goblin(int x, int y, int dist);
	~Goblin();
	virtual Object* onDeathDrop();
	virtual char smell(Player* p, char maze[18][70]);
	bool pathExists(char maze[18][70], int er, int ec, queue<Coord> coordQueue, int max_step);
};



#endif // ACTORS_H