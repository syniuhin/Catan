#ifndef Catan_game_h
#define Catan_game_h

#include "constants.h"
#include "vector"

class Hex {
  public:
    Hex(Hex*, Hex*, Hex*, Hex*, Hex*, Hex*, int, int);
  private:
    int num;
    int type;
    Hex* up_left;
    Hex* up_right;
    Hex* left;
    Hex* right;
    Hex* down_left;
    Hex* down_right;
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

class Game {
  public:
    Game();
  private:
    int res_left[5];
    int knights_left;
    int progress_left;
    int victory_p_left;
    int build_costs;

    std::vector<std::vector<Point> > map;
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
