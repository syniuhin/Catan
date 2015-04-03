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
        int player_id_;

        int cities_;
        int sttls_;
        int roads_;
};

class Village {
    public:
        Village(Point* loc = NULL, Player* ownr = NULL);
    protected:
        Point* location_;
        Player* owner_;
        int resource_delta_;
};

class City : public Village {
    public:
        City(Point* loc = NULL, Player* ownr = NULL);
};

class Game {
    public:
        Game(Map*, std::vector<Player*>, sf::RenderWindow*);

        void GenMap();

        void Update();

        void Click();
    private:
        int res_left_[5] = {RES_CARDS_NUM_FOREACH};
        int knights_left_ = KNIGHT_CARDS_NUM;
        int progress_left_ = PROGRESS_CARDS_NUM;
        int victory_p_left_ = VICTORY_P_CARDS_NUM;
        int build_costs_ = BUILD_COSTS_CARDS_NUM;

        Map* game_map_;
        std::vector<Player*> players_;

        sf::RenderWindow* window_;
};

#endif
