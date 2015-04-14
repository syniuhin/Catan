#ifndef __GAME_H__
#define __GAME_H__

#include "constants.h"
#include "map.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

const int TERRAIN_RES[7] = {-1, -1, 0, 1, 2, 3, 4};
class Player {
    public:
        Player(int);

        int get_id();

        void AddResource(int res_id, int count);

        std::string to_string();
    private:
        int player_id_;

        int cities_;
        int sttls_;
        int roads_;

        int resources_[5] = {0};
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

        void PerformTurn(Player*);
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

        int ThrowDice();
};

#endif
