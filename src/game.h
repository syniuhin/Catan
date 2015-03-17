#ifndef Catan_game_h
#define Catan_game_h

#include "constants.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

const int TYPE_SEA = 0;

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

    int get_type();
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

    void gen_map();
  private:
    std::map<int, Hex*> numMap;
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
