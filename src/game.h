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
        Village();
        Village(Point*, Player*);
    protected:
        Point* location;
        Player* owner;
        int resource_delta;
};

class City : public Village {
    public:
        City();
        City(Point*, Player*);
};

class Game {
    public:
        Game(Map*, std::vector<Player*>, sf::RenderWindow*);

        void gen_map();

        void update();

        void click();
    private:
        int res_left[5];
        int knights_left;
        int progress_left;
        int victory_p_left;
        int build_costs;

        Map* game_map;
        std::vector<Player*> players;

        sf::RenderWindow* window;
};

namespace GameFlow {
};
#endif
