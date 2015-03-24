#include <SFML/Graphics.hpp>

#include "easylogging++.h"

#include "game.h"

Game::Game() : knights_left(KNIGHT_CARDS_NUM),
               progress_left(PROGRESS_CARDS_NUM),
               victory_p_left(VICTORY_P_CARDS_NUM),
               build_costs(BUILD_COSTS_CARDS_NUM) {
  for (int i = 0; i < 5; ++i)
    res_left[i] = RES_CARDS_NUM_FOREACH;
}

Village::Village(){
    resource_delta = 1;
    location = NULL;
    owner = NULL;
}

Village::Village(Point* loc, Player* ownr){
    resource_delta = 1;
    location = loc;
    owner = ownr;
}

City::City(){
    resource_delta = 2;
    location = NULL;
    owner = NULL;
}

City::City(Point* loc, Player* ownr){
    resource_delta = 2;
    location = loc;
    owner = ownr;
}
