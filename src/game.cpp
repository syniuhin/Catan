#include <SFML/Graphics.hpp>

#include "easylogging++.h"

#include "game.h"

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : knights_left(KNIGHT_CARDS_NUM),
      progress_left(PROGRESS_CARDS_NUM),
      victory_p_left(VICTORY_P_CARDS_NUM),
      build_costs(BUILD_COSTS_CARDS_NUM),
      window(win),
      game_map(gm),
      players(plyrs.begin(), plyrs.end()){
    for (int i = 0; i < 5; ++i)
        res_left[i] = RES_CARDS_NUM_FOREACH;
}

void Game::gen_map(){
    game_map -> generate(window);
}

void Game::update(){
    sf::Event event;
    while (window -> pollEvent(event)) {
        switch (event.type){
            case sf::Event::Closed:
                window -> close();
                break;
            case sf::Event::MouseButtonReleased:
                click();
                break;
            default:
                break;
        }
    }
    window -> clear();
    game_map -> draw(window);
    window -> display();
}

void Game::click(){
    game_map -> click(window);
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

Player::Player(int p_id) : player_id(p_id) {}
