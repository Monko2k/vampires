#ifndef OBJECTS_H
#define OBJECTS_H
#include <string>

using namespace std;

class Object
{
public:
	Object(string action, string name, char symbol, int x, int y, bool onground);
	virtual ~Object();
	void toggleStatus();
	bool onGround();
	int get_x();
	int get_y();
	char get_symbol();
	string action();
	string name();
	
private:
	string m_action;
	string m_name;
	int x_pos;
	int y_pos;
	bool m_ground;
	char m_symbol;
};


class Weapon : public Object
{
public:
	Weapon();
	Weapon(string action, string name, int dex, int dam, int x, int y, bool onground);
	virtual ~Weapon();
	int get_dex();
	int get_dam();
private:
	int dexterity;
	int damage;
};

class Mace : public Weapon
{
public:
	Mace(int x, int y, bool onground);
	~Mace();
};

class S_Sword : public Weapon
{
public:
	S_Sword(int x, int y, bool onground);
	~S_Sword();
};

class L_Sword : public Weapon
{
public:
	L_Sword(int x, int y, bool onground);
	~L_Sword();
};

class Axe : public Weapon
{
public:
	Axe(int x, int y, bool onground);
	~Axe();
};

class Fang : public Weapon
{
public:
	Fang(int x, int y, bool onground);
	~Fang();
};

class Scroll : public Object
{
public:
	Scroll(string action, string name, int x, int y, bool onground, int type);
	virtual ~Scroll();
	int get_type();
private:
	int m_type;
	
};

class Teleport : public Scroll
{
public:
	Teleport(int x, int y, bool onground);
	~Teleport();
};

class Armor : public Scroll
{
public:
	Armor(int x, int y, bool onground);
	~Armor();
};

class Strength : public Scroll
{
public:
	Strength(int x, int y, bool onground);
	~Strength();
};


class Health : public Scroll
{
public:
	Health(int x, int y, bool onground);
	~Health();
};

class Dexterity : public Scroll
{
public:
	Dexterity(int x, int y, bool onground);
	~Dexterity();
};
#endif

