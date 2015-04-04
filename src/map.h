#ifndef __MAP_H__
#define __MAP_H__

#include "constants.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

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
const int NUMS_ARR[] = {
    2, 3, 3,
    4, 4, 5,
    5, 6, 6,
    8, 8, 9,
    9, 10, 10,
    11, 11, 12
};

/**
 * Forward declarations of game.h classes
 */
class Player;

class MapObject {
    public:
        sf::Vector2f get_pos();
        void set_pos(sf::Vector2f);

        virtual void Click() = 0;
        virtual bool OnMouse(sf::Vector2i) = 0;
    protected:
        sf::Vector2f pos_;
};

class Hex : public MapObject {
    public:
        Hex* up_left;
        Hex* up_right;
        Hex* left;
        Hex* right;
        Hex* down_left;
        Hex* down_right;

        Hex(Hex* up_left = NULL,
                Hex* up_right = NULL,
                Hex* left = NULL,
                Hex* right = NULL,
                Hex* down_left = NULL,
                Hex* down_right = NULL,
                int num = -1,
                int type = -1);

        void set_num(int);
        int get_num();

        void set_type(int);
        int get_type();

        std::string to_string();

        void Click();
        bool OnMouse(sf::Vector2i);
    private:
        int num_;
        int type_;
};

class Point : public MapObject {
    public:
        Point(Hex* first_,
                Hex* second_,
                Hex* third_);

        Hex** GetHexes(Hex**);

        void Click();
        bool OnMouse(sf::Vector2i);

        int get_owner_id();
        void set_owner_id(int);
    private:
        Hex* first_ = NULL;
        Hex* second_ = NULL;
        Hex* third_ = NULL;

        int owner_id_ = -1;
};

class Line : public MapObject {
    public:
        Line(Hex* first_ = NULL, Hex* second_ = NULL);

        /**
         * Factory method
         */
        static Line* FromPoints(Point*, Point*);

        void Click();
        bool OnMouse(sf::Vector2i);

        void set_owner_id(int);
    private:
        Hex* first_;
        Hex* second_;

        int owner_id_ = -1;
};

class Map {
    public:
        explicit Map(sf::RenderWindow*);
        Map(Hex* root, sf::RenderWindow*);

        ~Map() {};

        void Generate();
        void Draw();
        void Click();

        /**
         * Adds new Village to a Map.
         * NOTE: This method called only
         * after * mouse button released!
         */
        Point* AddVillage(Player*);
    private:
        std::vector<MapObject*> map_objects_;

        std::vector<Hex*> hexes_;
        std::vector<Point*> points_;
        std::vector<Line*> lines_;

        static const int dims_[GRID_SIZE + 2] = {GRID_SIZE - 1,
                GRID_SIZE, GRID_SIZE + 1, GRID_SIZE + 2,
                GRID_SIZE + 1, GRID_SIZE, GRID_SIZE - 1};

        Hex* root_ = NULL;
        sf::RenderWindow* window_;

        sf::CircleShape hexagon_;
        sf::CircleShape mouse_circle_;
        sf::CircleShape point_circle_;

        void Init();

        void GeneratePoints();

        void DrawMap();
        void DrawPoints();
        void DrawLines();
        void DrawMousePointer();

        Point* AddPoint(Hex* up_left, Hex* up_right, Hex* down);
        Line* AddLine(Hex* first_, Hex* second_);

        bool CheckNeighbors(Point*);
};

#endif /* defined(__MAP_H__) */
