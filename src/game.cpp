#include "game.h"

#include "util.h"

#include <SFML/Graphics.hpp>

#include "easylogging++.h"

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : window_(win),
      game_map_(gm),
      players_(plyrs.begin(), plyrs.end()) {}

void Game::GenMap(){
    game_map_ -> Generate();
}

void Game::SetUp() {
    for (size_t i = 0, k = 0; k < 2 * players_.size(); ++k){
        Player* curr = players_[i];
        Point* village_added = NULL;
        Line* road_added = NULL;
        while (window_ -> isOpen() && (!village_added ||
                    !road_added)){
            sf::Event event;
            game_map_ -> ShowNotification((village_added)
                    ? std::to_string(curr -> get_id() + 1)
                            .append(" player, add a road")
                    : std::to_string(curr -> get_id() + 1)
                            .append(" player, add a village"));
            while (window_ -> pollEvent(event)) {
                switch (event.type){
                    case sf::Event::Closed:
                        window_ -> close();
                        break;
                    case sf::Event::MouseButtonReleased:
                        if (!village_added) {
                            village_added = game_map_ ->
                                AddVillage(curr);
                            if (village_added)
                                villages_.push_back(new Village(
                                            village_added, curr));
                        } else {
                            road_added = game_map_ -> AddRoad(curr);
                        }
                        break;
                    default:
                        break;
                }
            }
            window_ -> clear();
            game_map_ -> Draw();
            window_ -> display();
        }
        if (k < players_.size() - 1)
            i++;
        else if (k >= players_.size())
            i--;
    }
    LOG(INFO) << "Game was set up successfully";
    game_map_ -> ShowNotification("Game was set up successfully",
            30);
}

void Game::Update(){
    sf::Event event;
    while (window_ -> pollEvent(event)) {
        switch (event.type){
            case sf::Event::Closed:
                window_ -> close();
                break;
            case sf::Event::MouseButtonReleased:
                game_map_ -> Click();
                break;
            default:
                break;
        }
    }
    window_ -> clear();
    game_map_ -> Draw();
    window_ -> display();
}

void Game::PerformTurn(Player* player) {
    //TODO: Complete when added all logic
    int num = ThrowDice();
    std::vector<Triple<int, int, int> > generated_resources =
            game_map_ -> GenerateResources(num);
}

int Game::ThrowDice() {
    return (rand() % 6 + 1) +
        (rand() % 6 + 1);
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

Player::Player(int p_id) {
    this -> player_id_ = p_id;
}

int Player::get_id() {
    return player_id_;
}
