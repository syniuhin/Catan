#ifndef Catan_constants_h
#define Catan_constants_h

#include <SFML/Graphics.hpp>

#include <string.h>
#include <string>

const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1200;

const int TRADE_WINDOW_HEIGHT = 500;
const int TRADE_WINDOW_WIDTH = 700;

const std::string APP_NAME = "Catan";

/**
 * Game rules constants.
 */
static const int GRID_SIZE = 5;

static const int RES_CARDS_NUM_FOREACH = 19;
static const int KNIGHT_CARDS_NUM = 14;
static const int PROGRESS_CARDS_NUM = 6;
static const int VICTORY_P_CARDS_NUM = 5;
static const int BUILD_COSTS_CARDS_NUM = 4;

static const int CITIES_NUM_FOREACH = 4;
static const int STTL_NUM_FOREACH = 5;
static const int ROADS_NUM_FOREACH = 15;

static const int BRICKS_ID = 0;
static const int WOOL_ID = 1;
static const int ORE_ID = 2;
static const int GRAIN_ID = 3;
static const int LUMBER_ID = 4;

/**
 * Buildings' costs
 */
static const int ROAD_COST[] = {1, 0, 0, 0, 1};
static const int VILLAGE_COST[] = {1, 0, 1, 1, 1};
static const int CITY_COST[] = {0, 0, 3, 2, 0};
static const int CARD_COST[] = {0, 1, 1, 1, 0};

/**
 * Common constants
 */
static const double EPS = 0.0000005;

static const sf::Vector2f ACTION_PANEL_SIZE = sf::Vector2f(210, 50);
static const sf::Vector2f ACTION_PANEL_POS =
        sf::Vector2f(SCREEN_WIDTH - ACTION_PANEL_SIZE.x - 20,
                SCREEN_HEIGHT - ACTION_PANEL_SIZE.y - 40);

static const sf::Vector2f PLAYER_CARD_SIZE = sf::Vector2f(300, 80);

static const sf::Vector2f PLAYER_PANEL_SIZE =
        sf::Vector2f(PLAYER_CARD_SIZE.x + 20,
            4 * (PLAYER_CARD_SIZE.y + 20));
static const sf::Vector2f PLAYER_PANEL_POS =
        sf::Vector2f(10, .5 * (SCREEN_HEIGHT - PLAYER_PANEL_SIZE.y));

static std::vector<std::string> PLAYER_NAMES = {
            "Candamir",
            "Hildegard",
            "Jean",
            "Louis"
        };
#endif
