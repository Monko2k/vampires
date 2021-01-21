#include "Objects.h"

//Object implementation

//Object public functions

Object::Object(string action, string name, char symbol, int x, int y, bool onground)	//Object constructor
{
	m_ground = onground;
	x_pos = x;
	y_pos = y;
	m_name = name;
	m_symbol = symbol;
	m_action = action;
}

Object::~Object() //Object destructor
{}

void Object::toggleStatus()
{
	m_ground = !m_ground;
}

//Object getter functions

string Object::name()
{
	return m_name;
}

int Object::get_x()
{
	return x_pos;
}

int Object::get_y()
{
	return y_pos;
}

char Object::get_symbol()
{
	return m_symbol;
}

bool Object::onGround()
{
	return m_ground;
}

string Object::action()
{
	return m_action;
}


//Weapon implementation

Weapon::Weapon(string action, string name, int dex, int dam, int x, int y, bool onground)	//Weapon constructor
	:Object(action, name, ')', x, y, onground)
{
	dexterity = dex;
	damage = dam;
}

Weapon::~Weapon() //Weapon destructor
{}

int Weapon::get_dex()
{
	return dexterity;
}

int Weapon::get_dam()
{
	return damage;
}

//Mace implementation

Mace::Mace(int x, int y, bool onground)	//Mace constructor
	:Weapon("swings", "mace", 0, 2, x, y, onground)
{}

Mace::~Mace()	//Mace destructor
{}

//Short Sword implementation

S_Sword::S_Sword(int x, int y, bool onground)	//Short sword constructor
	:Weapon("slashes", "short sword", 0, 2, x, y, onground)
{}

S_Sword::~S_Sword()	//Short sword destructor
{}

//Long Sword implementation

L_Sword::L_Sword(int x, int y, bool onground)	//Long sword constructor
	:Weapon("swings", "long sword", 2, 4, x, y, onground)
{}

L_Sword::~L_Sword()	//Long sword destructor
{}

//Magic Axe implementation

Axe::Axe(int x, int y, bool onground)	//Magic axe constructor
	:Weapon("chops", "magic axe", 5, 5, x, y, onground)
{}

Axe::~Axe()	//Magic axe destructor
{}

//Magic Fangs implementation

Fang::Fang(int x, int y, bool onground)	//Magic fang constructor
	:Weapon("strikes", "magic fangs of sleep", 3, 2, x, y, onground)
{}

Fang::~Fang()	//Magic fang destructor
{}

//Scroll implementation

Scroll::Scroll(string action, string name, int x, int y, bool onground, int type)	//Scroll constructor
	:Object(action, name, '?', x, y, onground)
{
	m_type = type;
}

Scroll::~Scroll()	//Scroll destructor
{}

int Scroll::get_type()
{
	return m_type;
}
//Scroll of Teleportation implementation

Teleport::Teleport(int x, int y, bool onground)	//Scroll of Teleportation constructor
	:Scroll("You feel your body wrenched in space and time.", "scroll of teleportation", x, y, onground, 0)
{}

Teleport::~Teleport()	//Scroll of Teleportation destructor
{}

//Scroll of Enhance Armor implementation

Armor::Armor(int x, int y, bool onground)	//Scroll of Enhance Armor constructor
	:Scroll("Your armor glows blue.", "scroll of enhance armor", x, y, onground, 1)
{}

Armor::~Armor()	//Scroll of Enhance Armor destructor
{}

//Scroll of Strength implementation

Strength::Strength(int x, int y, bool onground)	//Scroll of Strength constructor
	:Scroll("Your muscles bulge.", "scroll of strength", x, y, onground, 2)
{}

Strength::~Strength()	//Scroll of Strength destructor
{}

//Scroll of Enhance Health implementation

Health::Health(int x, int y, bool onground)	//Scroll of Enhance Health constructor
	:Scroll("You feel your heart beating stronger.", "scroll of enhance health", x, y, onground, 3)
{}

Health::~Health()	//Scroll of Enhance Health destructor
{}

//Scroll of Enhance Dexterity implementation

Dexterity::Dexterity(int x, int y, bool onground)	//Scroll of Enhance Dexterity constructor
	: Scroll("You feel like less of a klutz.", "scroll of enhanced dexterity", x, y, onground, 4)
{}

Dexterity::~Dexterity()	//Scroll of Enhance Dexterity destructor
{}