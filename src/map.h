//
//  map.h
//  Catan
//
//  Created by Illia Syniuhin on 2/18/15.
//  Copyright (c) 2015 Illia Syniuhin. All rights reserved.
//

#ifndef __Catan__map__
#define __Catan__map__

#include "constants.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

//#include "game.h"
/**
 * Types of the land.
 */
static const int TYPE_SEA = 0;
static const int TYPE_DESERT = 1;
static const int TYPE_HILLS = 2;
static const int TYPE_PASTURE = 3;
static const int TYPE_MOUNTAINS = 4;
static const int TYPE_FIELDS = 5;
static const int TYPE_FOREST = 6;

static const int TYPE_COUNT[] = {0, 1, 3, 4, 3, 4, 4};
static const sf::Color TYPE_COLOR[] = {
    sf::Color(10, 10, 255, 180),
    sf::Color(255, 255, 255, 180),
    sf::Color(173, 125, 36, 180),
    sf::Color(106, 254, 129, 180),
    sf::Color(125, 125, 125, 180),
    sf::Color(227, 227, 48, 180),
    sf::Color(22, 174, 0, 180)
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

class MapObject {
    public:
        MapObject();

        sf::Vector2f* get_pos();
        void set_pos(sf::Vector2f*);

        virtual void click() = 0;
        virtual bool on_mouse(sf::Vector2i) = 0;
    protected:
        sf::Vector2f* pos;
};

class Hex : public MapObject {
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

        void click();
        bool on_mouse(sf::Vector2i);
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

        void generate(sf::RenderWindow*);

        void draw(sf::RenderWindow*);

        void click(sf::RenderWindow*);
    private:
        std::map<int, Hex*> num_map;
        std::vector<Hex*> id_map;

        std::vector<MapObject*> map_objects;
        Hex* root;
};

#endif /* defined(__Catan__map__) */
