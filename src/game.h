#ifndef Catan_game_h
#define Catan_game_h

#include "constants.h"
#include "map.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

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

class Village {
    public:
        Village();
        Village(Point*, Player*);
    protected:
        Point* location;
        Player* owner;
        int resource_delta;
};

class City : public Village {
    public:
        City();
        City(Point*, Player*);
};

namespace GameFlow {
};
#endif
