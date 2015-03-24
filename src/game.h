#ifndef Catan_game_h
#define Catan_game_h

#include "constants.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

static const int TYPE_SEA = 0;
static const int TYPE_DESERT = 1;
static const int TYPE_HILLS = 2;
static const int TYPE_PASTURE = 3;
static const int TYPE_MOUNTAINS = 4;
static const int TYPE_FIELDS = 5;
static const int TYPE_FOREST = 6;

static const int TYPE_COUNT[] = {0, 1, 3, 4, 3, 4, 4};
static const sf::Color TYPE_COLOR[] = {
    sf::Color::Blue,
    sf::Color::White,
    sf::Color(173, 125, 36),
    sf::Color(106, 254, 129),
    sf::Color(125, 125, 125),
    sf::Color(227, 227, 48),
    sf::Color(22, 174, 0)
};

const int HEXES_NUM = 19;
const int nums_arr[] = {
    2, 3, 3,
    4, 4, 5,
    5, 6, 6,
    8, 8, 9,
    9, 10, 10,
    11, 11, 12
};

class Hex {
  public:
    Hex* up_left;
    Hex* up_right;
    Hex* left;
    Hex* right;
    Hex* down_left;
    Hex* down_right;

    Hex();
    Hex(Hex*, Hex*, Hex*, Hex*, Hex*, Hex*, int, int);

    void set_num(int);
    int get_num();

    void set_type(int);
    int get_type();

    std::string to_string();
  private:
    int num;
    int type;
};

struct Point {
  Hex* first;
  Hex* second;
  Hex* third;

  Point() {}
  Point(Hex*, Hex*, Hex*);

  Hex** getHexes(Hex**);
};

struct Line {
  Hex* first;
  Hex* second;

  Line() {}
  Line(Hex*, Hex*);
  Line(Point*, Point*);
};

class Map {
  public:
    Map();
    Map(Hex*);

    void generate();

    void draw(sf::RenderWindow*);
  private:
    std::map<int, Hex*> num_map;
    std::vector<Hex*> id_map;
    Hex* root;
};

class Game {
  public:
    Game();
  private:
    int res_left[5];
    int knights_left;
    int progress_left;
    int victory_p_left;
    int build_costs;
};

class Player {
  private:
    int cities;
    int sttls;
    int roads;
};

namespace GameFlow {
};
#endif
