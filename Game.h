#ifndef GAME_H
#define GAME_H
#include <vector>

class Player;
class Level;


class Game
{
public:
	Game(int goblinSmellDistance);
	~Game();

	void play();
private:
	int m_current_level;
	Player* p;
	std::vector<Level*> levels;
	int m_goblinSmellDistance;
};



#endif