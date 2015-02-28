#include "game.h"

Game::Game() : knights_left(KNIGHT_CARDS_NUM), 
               progress_left(PROGRESS_CARDS_NUM),
               victory_p_left(VICTORY_P_CARDS_NUM),
               build_costs(BUILD_COSTS_CARDS_NUM) {
  for (int i = 0; i < 5; ++i)
    res_left[i] = RES_CARDS_NUM_FOREACH;
}

Hex::Hex(Hex* ul, Hex* ur, Hex* l, Hex* r, 
    Hex* dl, Hex* dr, int num, int type){
  up_left = ul;
  up_right = ur;
  left = l;
  right = r;
  down_left = dl;
  down_right = dr;
  this -> num = num;
  this -> type = type;
}

Point::Point(Hex* f, Hex* s, Hex* t){
  first = f;
  second = s;
  third = t;
}

Hex** Point::getHexes(Hex** res){
  res[0] = first;
  res[1] = second;
  res[2] = third;
  return res;
}

Line::Line(Hex* f, Hex* s){
  first = s;
  second = s;
}

Line::Line(Point* a, Point* b){
  if (a == b)
    return;
  Hex** ah = new Hex*[3];
  ah = a -> getHexes(ah);

  Hex** bh = new Hex*[3];
  bh = b -> getHexes(bh);

  for (int i = 0; i < 3; ++i){
    for (int j = 0; j < 3; ++j){
      if (ah[i] == bh[j]){
        if (NULL == first)
          first = ah[i];
        else
          second = ah[i];
      }
    }
  }
  delete[] ah;
}
