#ifndef Catan_constants_h
#define Catan_constants_h

#include <string.h>
#include <string>

const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1200;

const std::string APP_NAME = "Catan";

/**
 * Game rules constants.
 */
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
static const int STTL_COST[] = {1, 0, 1, 1, 1};
static const int CITY_COST[] = {0, 0, 3, 2, 0};
static const int CARD_COST[] = {0, 1, 1, 1, 0};

#endif
