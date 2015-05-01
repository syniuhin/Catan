#include "game.h"

#include "util.h"

#include <SFML/Graphics.hpp>

#include "easylogging++.h"

const sf::Vector2f ACTION_PANEL_POS = sf::Vector2f(10, 600);

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : window_(win),
      game_map_(gm),
      players_(plyrs.begin(), plyrs.end()),
      curr_player_ind_(0) {}

void Game::GenMap(){
    game_map_ -> Generate();
}

void Game::SetUp() {
    exit_cond_ = [] () {
        return false;
    };

    Button* p_new_village_btn =
        Button::CreateInstance(ACTION_PANEL_POS +
            sf::Vector2f(45, 10), sf::Vector2f(30, 30))
                -> SetColors(sf::Color(196, 53, 52, 196),
                             sf::Color(196, 53, 52, 255))
                -> AddCallback(
                        [this] () {
                            game_map_ -> AddVillage();
                            LOG(INFO) << "Add village "
                                "button clicked";
                        });
    game_map_ -> AddButton(p_new_village_btn);

    Button* p_dice_btn =
        Button::CreateInstance(ACTION_PANEL_POS +
            sf::Vector2f(10, 10), sf::Vector2f(30, 30))
                -> SetColors(sf::Color(53, 196, 72, 196),
                             sf::Color(53, 196, 72, 255))
                -> AddCallback(
                        [this] () {
                            curr_player_ind_ =
                                    (curr_player_ind_ + 1) % 4;
                            PerformTurn(players_[curr_player_ind_]);
                        });
    game_map_ -> AddButton(p_dice_btn);

//    game_map_ -> SetNextTurnCallback(
//            [this] () {
//                curr_player_ind_ =
//                        (curr_player_ind_ + 1) % 4;
//                PerformTurn(players_[curr_player_ind_]);
//            });
//
//    game_map_ -> SetAddVillageCallback(
//            [this] () {
//                exit_cond_ = [&] () {
//                    game_map_ -> AddVillage();
//                    return false;
//                };
//            });

    for (size_t i = 0, k = 0; k < 2 * players_.size(); ++k){
        Player* curr = players_[i];
        Point* village_added = NULL;
        Line* road_added = NULL;
        sf::Event event;
        while (window_ -> isOpen() && (!village_added ||
                    !road_added)){
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
                        game_map_ ->
                            Click(players_[curr_player_ind_]);
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

void Game::Update() {
    sf::Event e;
    while (window_ -> isOpen() && !exit_cond_()) {
        while (window_ -> pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed:
                    window_ -> close();
                    break;
                case sf::Event::MouseButtonReleased:
                    game_map_ ->
                        Click(players_[curr_player_ind_]);
                    break;
                default:
                    break;
            }
        }
        window_ -> clear();
        game_map_ -> Draw();
        window_ -> display();
    }
}

void Game::PerformTurn(Player* curr) {
    //TODO: Complete when added all logic
    const int num = ThrowDice();
    game_map_ -> ShowNotification(std::string("Dice showed ")
            .append(std::to_string(num)));
    if (num != 7) {
        std::vector<Triple<int, int, int> > generated_resources =
                game_map_ -> GenerateResources(num);
        for (size_t i = 0; i < generated_resources.size(); ++i) {
            Triple<int, int, int> triple = generated_resources[i];
            LOG(INFO) << "PlayerID: " << std::to_string(triple.first) <<
                    " TerrainID: " << std::to_string(triple.second) <<
                    " Count: " << std::to_string(triple.third);
            players_[triple.first] ->
                AddResource(TERRAIN_RES[triple.second], triple.third);
        }
    } else {
        //TODO: activate robber
    }

    //Res output
    for (size_t i = 0; i < players_.size(); ++i)
        LOG(INFO) << players_[i] -> to_string();
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

void Player::AddResource(int res_id, int count) {
    resources_[res_id] += count;
}

std::string Player::to_string() {
    std::string res;
    res.append("{");
    for (int i = 0; i < 5; ++i)
        res.append(std::to_string(resources_[i])).append((i < 4)
                ? ", "
                : "} ");
    res.append("id = ").append(std::to_string(player_id_));
    return res;
}
