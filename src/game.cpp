#include "game.h"

#include "util.h"
#include "constants.h"
#include "easylogging++.h"

#include <SFML/Graphics.hpp>

const int MOUSE_POINTER_SIZE = 10;
const int MOUSE_POINTER_PRECISION = 17;

Game::Game(Map* gm, std::vector<Player*> plyrs,
        sf::RenderWindow* win)
    : window_(win),
      game_map_(gm),
      mouse_circle_(sf::CircleShape(MOUSE_POINTER_SIZE,
              MOUSE_POINTER_PRECISION)),
      trade_win_(TradeWindow::CreateInstance(0)),
      players_(plyrs.begin(), plyrs.end()),
      curr_player_ind_(0) {}

void Game::GenMap(){
    game_map_ -> Generate();
}

void Game::SetUp() {
    on_click_ = [](){};

    Button* p_new_village_btn =
        Button::CreateInstance(ACTION_PANEL_POS +
            sf::Vector2f(45, 10), sf::Vector2f(30, 30))
                -> SetColors(sf::Color(196, 53, 52, 196),
                             sf::Color(196, 53, 52, 255))
                -> AddCallback(
                        [this] () {
                            Player* curr = players_[curr_player_ind_];
                            if (curr -> TryBuildVillage()) {
                                on_click_ = [&]() {
                                    game_map_ -> AddVillage();
                                };
                                game_map_ -> ShowNotification("Add a village");
                            } else {
                                game_map_ -> ShowNotification("Can't add village");
                            }
                            LOG(INFO) << "Add village "
                                "button clicked";
                        });
    game_map_ -> AddButton(p_new_village_btn);

    Button* p_new_road_btn =
        Button::CreateInstance(ACTION_PANEL_POS +
            sf::Vector2f(80, 10), sf::Vector2f(30, 30))
                -> SetColors(sf::Color(96, 153, 52, 196),
                             sf::Color(96, 153, 52, 255))
                -> AddCallback(
                        [this] () {
                            Player* curr = players_[curr_player_ind_];
                            if (curr -> TryBuildRoad()) {
                                on_click_ = [&]() {
                                    game_map_ ->
                                        AddRoad(players_[curr_player_ind_]);
                                };
                                game_map_ -> ShowNotification("Add a road");
                            } else {
                                game_map_ -> ShowNotification("Can't add a road");
                            }
                            LOG(INFO) << "Add road "
                                "button clicked";
                        });
    game_map_ -> AddButton(p_new_road_btn);

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
                            on_click_ = [](){};
                        });
    game_map_ -> AddButton(p_dice_btn);

    Button* p_trade_button =
        Button::CreateInstance(ACTION_PANEL_POS +
            sf::Vector2f(115, 10), sf::Vector2f(30, 30))
                -> SetColors(sf::Color(100, 73, 66, 196),
                             sf::Color(100, 73, 66, 255))
                -> AddCallback(
                        [this] () {
                            visual_mode_ = 1;
                            trade_win_ -> set_seller(curr_player_ind_);
                            on_escape_ = [&] () {
                                visual_mode_ = 0;
                            };
                        });
    game_map_ -> AddButton(p_trade_button);

    game_map_ -> DisplayPlayersInfo(players_);
    mouse_circle_.setFillColor(sf::Color(200, 200, 200, 250));
    mouse_circle_.setOrigin(.5 * sqrt(2.0) * MOUSE_POINTER_SIZE,
            .5 * sqrt(2.0) * MOUSE_POINTER_SIZE);

    RandomSetUp();
    PerformTurn(players_[0]);
    LOG(INFO) << "Game was set up successfully";
}

void Game::RandomSetUp() {
    for (size_t i = 0; i < players_.size(); ++i) {
        game_map_ -> AddRandomVillageRoad(players_[i]);
        game_map_ -> AddRandomVillageRoad(players_[i]);
    }
}

void Game::ManualSetUp() {
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
}

void Game::Update() {
    sf::Event e;
    int eventType = -1;
    while (window_ -> isOpen()) {
        while (window_ -> pollEvent(e)) {
            eventType = e.type;
            switch (eventType) {
                case sf::Event::Closed:
                    window_ -> close();
                    break;
                case sf::Event::MouseButtonReleased:
                    if (0 == visual_mode_) {
                        game_map_ ->
                            Click(players_[curr_player_ind_]);
                        on_click_();
                    } else if (1 == visual_mode_) {
                        trade_win_ -> Click(window_);
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Escape)
                        on_escape_();
                default:
                    break;
            }
        }
        window_ -> clear();
        game_map_ -> Draw();
        if (1 == visual_mode_)
            trade_win_ -> Draw(window_);
        DrawMousePointer();
        window_ -> display();
    }
}

void Game::PerformTurn(Player* curr) {
    //TODO: Complete when added all logic
    const int num = ThrowDice();
    game_map_ -> ShowNotification(std::to_string(curr -> get_id()) +
                " make a turn, dice " +
                std::to_string(num));
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

void Game::DrawMousePointer() const {
    sf::Vector2i point = sf::Mouse::getPosition(*window_);
    mouse_circle_.setPosition((float) point.x, (float) point.y);
    window_ -> draw(mouse_circle_);
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

Player::Player(int p_id)
    : player_id_(p_id),
      cities_(0),
      villages_(0),
      roads_(0) {}

int Player::get_id() {
    return player_id_;
}

int* Player::get_resources() {
    return resources_;
}

void Player::AddResource(int res_id, int count) {
    resources_[res_id] += count;
}

bool Player::TryBuildVillage() {
    bool res_enough = true;
    for (int i = 0; i < 5 && res_enough; ++i)
        res_enough = resources_[i] >= VILLAGE_COST[i];
    if (res_enough) {
        for (int i = 0; i < 5; ++i)
            resources_[i] -= VILLAGE_COST[i];
        villages_++;
        return true;
    }
    return false;
}

bool Player::TryBuildRoad() {
    bool res_enough = true;
    for (int i = 0; i < 5 && res_enough; ++i)
        res_enough = resources_[i] >= ROAD_COST[i];
    if (res_enough) {
        for (int i = 0; i < 5; ++i)
            resources_[i] -= ROAD_COST[i];
        roads_++;
        return true;
    }
    return false;
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
