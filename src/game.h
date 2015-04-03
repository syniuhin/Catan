#ifndef __GAME_H__
#define __GAME_H__

#include "constants.h"
#include "map.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

class Player {
    public:
        Player(int);

        int get_id();
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
//        friend class Point;

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

        void SetUp();
        void Update();

    private:
        int res_left_[5] = {RES_CARDS_NUM_FOREACH};
        int knights_left_ = KNIGHT_CARDS_NUM;
        int progress_left_ = PROGRESS_CARDS_NUM;
        int victory_p_left_ = VICTORY_P_CARDS_NUM;
        int build_costs_ = BUILD_COSTS_CARDS_NUM;

        Map* game_map_;
        std::vector<Player*> players_;
        std::vector<Village*> villages_;

        sf::RenderWindow* window_;

        void Click();
};

#endif
