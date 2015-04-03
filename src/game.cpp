#include "game.h"

#include <SFML/Graphics.hpp>

#include "easylogging++.h"

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : window_(win),
      game_map_(gm),
      players_(plyrs.begin(), plyrs.end()) {}

void Game::GenMap(){
    game_map_ -> Generate(window_);
}

void Game::Update(){
    sf::Event event;
    while (window_ -> pollEvent(event)) {
        switch (event.type){
            case sf::Event::Closed:
                window_ -> close();
                break;
            case sf::Event::MouseButtonReleased:
                Click();
                break;
            default:
                break;
        }
    }
    window_ -> clear();
    game_map_ -> Draw(window_);
    window_ -> display();
}

void Game::Click(){
    game_map_ -> Click(window_);
}

Village::Village(Point* loc, Player* ownr){
    resource_delta_ = 1;
    location_ = loc;
    owner_ = ownr;
}

City::City(Point* loc, Player* ownr){
    resource_delta_ = 2;
    location_ = loc;
    owner_ = ownr;
}

Player::Player(int p_id) : player_id_(p_id) {}
