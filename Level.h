#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "Actors.h"
#include "Objects.h"



class Level
{
public:
    Level(Player* player, int goblinSmellDistance, int stage);
    ~Level();
    void display();
    void update();
    void player_move();
    void monster_move();
    
    bool level_status();
    bool exit_status();
    

private:
    char m_grid[18][70];
    std::vector<Monster*> m_monsters;
    std::vector<Object*> m_objects;

    Object* m_exit;
    int m_num_objects;
    int m_num_monster;
    int m_stage;
    Player* p;
    string message_buffer;

    bool level_quit;
    bool level_complete;

    Object* getItem();
    void player_attack(Player* p, int x, int y);
    void monster_attack(Monster* m);
    bool valid_move(char dir, Actor* a);
    bool valid_attack(char dir, Actor* a);
    void spawn_monsters(int goblinSmellDistance, int stage);
    void spawn_items();
    void spawn_player(Player* p);
    void display_stats();
    void generate_grid();
    void test_grid();
    void grid_fill(int sr, int sc);
 

};




#endif