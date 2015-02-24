//
//  map.cpp
//  Catan
//
//  Created by Illia Syniuhin on 2/18/15.
//  Copyright (c) 2015 Illia Syniuhin. All rights reserved.
//

#include "map.h"

const int GRID_SIZE = 5;
const int HEX_SIZE = 60;
const int HEX_OUTLINE_SIZE = 5;

const double FACTOR_W = 2;
const double FACTOR_H = 1.7320508075688773;

void draw_map(sf::RenderWindow* window){
    sf::CircleShape hexagon(HEX_SIZE, 6);
    hexagon.rotate(90);
    hexagon.setOutlineThickness(HEX_OUTLINE_SIZE);
    hexagon.setOutlineColor(sf::Color::Black);

    int horizontal_offset = (window -> getSize().x -
                             8 * (HEX_SIZE + HEX_OUTLINE_SIZE)) / 2;
    for (int i = 0; i <= GRID_SIZE / 2; ++i){
        int side = 3 + i;
        int vertical_offset = (window -> getSize().y -
                               side * (HEX_SIZE * FACTOR_H + HEX_OUTLINE_SIZE * 2)) / 2;
        for (int j = 0; j < side; ++side){
            
        }
    }
}
