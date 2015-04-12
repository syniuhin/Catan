#ifndef __MAP_H__
#define __MAP_H__

#include "constants.h"
#include "util.h"

#include <stdlib.h>
#include <math.h>

#include <vector>
#include <map>
#include <set>

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
class Point;

class MapObject {
    public:
        virtual ~MapObject() {}

        sf::Vector2f get_pos() const;
        void set_pos(sf::Vector2f);

        virtual void Click() = 0;
        virtual bool OnMouse(sf::Vector2i) const = 0;
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
        ~Hex();

        void set_num(int);
        int get_num() const;

        void set_type(int);
        int get_type() const;

        std::string to_string() const;

        void Click();
        bool OnMouse(sf::Vector2i) const;

        bool CanBeAdded(Point*);
        void AddPoint(Point*);
    private:
        int num_;
        int type_;

        Point* points_[6] = {NULL};

        void AddPoint(Point*, int index);

        int GetIndexForPoint(Point*);
};

class Point : public MapObject {
    public:
        Point(Hex* first_,
                Hex* second_,
                Hex* third_);
        ~Point();

        static bool AreAdjacent(const Point, const Point);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        int get_owner_id() const;
        void set_owner_id(int);

        Hex** get_hexes(Hex**) const;
        std::set<Hex*> get_hexes();
    private:
        std::set<Hex*> hexes_;
        int owner_id_ = -1;
};

class Line : public MapObject {
    public:
        ~Line();
        /**
         * Factory method. Why? Because
         * we need to perform set_intersection
         * because of (probably poorely designed)
         * Point interface. TODO: think about
         * Point carefully and decide between
         * consistency and performance
         */
        static Line* FromPoints(Point*, Point*);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        bool CheckOwnership(int owner_id) const;

        void set_owner_id(int);
        int get_owner_id() const;

        Point** get_points(Point**) const;
    private:
        /**
         * This constructor logic sucks.
         * TODO: search how to initialize
         * private members when using
         * factory method
         */
        Line(Point*, Point*, std::set<Hex*>);

        Point* first_;
        Point* second_;

        int owner_id_ = -1;

        std::set<Hex*> hexes_;
};

class Map {
    public:
        explicit Map(sf::RenderWindow*);
        Map(Hex* root, sf::RenderWindow*);

        ~Map();

        void Generate();
        void Draw();
        void Click();

        /**
         * Adds new Village to a Map.
         * NOTE: This method called only
         * after mouse button released!
         */
        Point* AddVillage(Player*);

        /**
         * Same note here
         */
        Line* AddRoad(Player*);

        /**
         * Generates triples by dice sum:
         * <player_id, resource_id, quantity>
         */
        std::vector<Triple<int, int, int> >
                GenerateResources(int num);
    private:
        std::vector<MapObject*> map_objects_;

        std::vector<Hex*> hexes_;
        std::vector<Point*> points_;
        std::vector<Line*> lines_;

        std::vector<Hex*> hexes_by_num_[13];

        const int dims_[GRID_SIZE + 2] = {GRID_SIZE - 1,
                GRID_SIZE, GRID_SIZE + 1, GRID_SIZE + 2,
                GRID_SIZE + 1, GRID_SIZE, GRID_SIZE - 1};

        Hex* root_ = NULL;
        sf::RenderWindow* window_;

        sf::CircleShape hexagon_;
        sf::CircleShape mouse_circle_;
        sf::CircleShape point_circle_;
        sf::VertexArray line_array_;

        void Init();

        void GeneratePoints();
        /**
         * NOTE: Called STRICTLY
         * after GeneratePoints()
         */
        void GenerateLines();

        void DrawMap();
        void DrawPoints();
        void DrawLines();
        void DrawMousePointer();

        Point* AddPoint(Point*);
        Line* AddLine(Line*);

        bool TryAddPoint(Point*);
};

#endif /* defined(__MAP_H__) */
