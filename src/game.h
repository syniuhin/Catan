#ifndef Catan_game_h
#define Catan_game_h

#include "constants.h"
#include "map.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

class Player {
    public:
        Player(int);
    private:
        int player_id;

        int cities;
        int sttls;
        int roads;
};

class Village {
    public:
        Village(Point* loc = NULL, Player* ownr = NULL);
    protected:
        Point* location;
        Player* owner;
        int resource_delta;
};

class City : public Village {
    public:
        City(Point* loc = NULL, Player* ownr = NULL);
};

class Game {
    public:
        Game(Map*, std::vector<Player*>, sf::RenderWindow*);

        void gen_map();

        void update();

        void click();
    private:
        int res_left[5] = {RES_CARDS_NUM_FOREACH};
        int knights_left = KNIGHT_CARDS_NUM;
        int progress_left = PROGRESS_CARDS_NUM;
        int victory_p_left = VICTORY_P_CARDS_NUM;
        int build_costs = BUILD_COSTS_CARDS_NUM;

        Map* game_map;
        std::vector<Player*> players;

        sf::RenderWindow* window;
};

#endif
