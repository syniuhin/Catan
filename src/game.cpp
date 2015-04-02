#include "game.h"

#include <SFML/Graphics.hpp>

#include "easylogging++.h"

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : window(win),
      game_map(gm),
      players(plyrs.begin(), plyrs.end()) {}

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

Village::Village(Point* loc, Player* ownr){
    resource_delta = 1;
    location = loc;
    owner = ownr;
}

City::City(Point* loc, Player* ownr){
    resource_delta = 2;
    location = loc;
    owner = ownr;
}

Player::Player(int p_id) : player_id(p_id) {}
