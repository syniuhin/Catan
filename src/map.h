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

class Map;

class OnClickListener {
    public:
        Map* pmap_; //this sucks.

        explicit OnClickListener(Map*);
        virtual ~OnClickListener();

        virtual void OnClick() = 0;
};

class MapObject {
    public:
        virtual ~MapObject();

        sf::Vector2f get_pos() const;
        void set_pos(sf::Vector2f);

        void set_on_click_listener(OnClickListener*);

        virtual void Click();
        virtual bool OnMouse(sf::Vector2i) const = 0;
        //TODO: make it pure virtual, using tr1::shared_ptr
        virtual void Draw(sf::RenderWindow*);
    protected:
        MapObject();
        sf::Vector2f pos_;

        OnClickListener* on_click_listener_;
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

        std::vector<int> GetBoundPlayersIds() const;
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

class NotificationArea : public MapObject {
    public:
        static NotificationArea*
            CreateInstance(std::string font_path);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        void MakeVisible();
        void MakeVisible(int duration);
        void MakeInvisible();
        void Draw(sf::RenderWindow*);

        void SetContent(std::string text);

        void Update();
    private:
        NotificationArea();

        sf::Text notification_text_;
        sf::Font notification_text_font_;

        bool visible_ = false;
        int duration_ = 0;

        const int INF_DURATION = -1;
};

class DiceButton : public MapObject {
    public:
        static DiceButton* CreateInstance();
        void Click();
        bool OnMouse(sf::Vector2i) const;

        void Draw(sf::RenderWindow*);
    private:
        const sf::Color color_idle_ =
            sf::Color(212, 193, 131, 100);
        const sf::Color color_focused_ =
            sf::Color(212, 193, 131, 200);
        const sf::Color color_selected_ =
            sf::Color(131, 150, 212, 255);
        sf::CircleShape dice_circle_;

        DiceButton();
};

class Button : public MapObject {
    public:
        static Button* CreateInstance(sf::Vector2f pos,
                                      sf::Vector2f b_size);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        void Draw(sf::RenderWindow*);

        /**
         * Builder method(s)
         */
        Button* AddCallback(std::function<void()> cb);
        Button* SetColor(sf::Color);
    private:
        std::vector<std::function<void()> > callbacks_;
        sf::RectangleShape shape_;

        Button(sf::Vector2f b_size);
};

class ActionPanel : public MapObject {
    public:
        static ActionPanel* CreateInstance();

        void Click();
        bool OnMouse(sf::Vector2i) const;

        void Draw(sf::RenderWindow*);

        /**
         * Adding already defined component
         */
        void AddComponent(MapObject*);
    private:
        std::vector<MapObject*> components_;

        sf::RectangleShape panel_shape_;
        sf::Color panel_color_;

        ActionPanel();
};

class Map {
    public:
        explicit Map(sf::RenderWindow*);
        Map(Hex* root, sf::RenderWindow*);

        ~Map();

        void Generate();
        void Draw() const;
        void Click(Player* requester);
        bool NextTurn() const;

        void ShowNotification(std::string);
        void ShowNotification(std::string, int duration);

        /**
         * Adds new Village to a Map.
         * NOTE: This method called only
         * after mouse button released!
         */
        Point* AddVillage(Player*);
        void AddVillage();

        /**
         * Same note here
         */
        Line* AddRoad(Player*);

        /**
         * Generates triples by dice sum:
         * <player_id, terrain_type, quantity>
         */
        std::vector<Triple<int, int, int> >
                GenerateResources(int num);
    private:
        std::vector<MapObject*> map_objects_;

        std::vector<Hex*> hexes_;
        std::vector<Point*> points_;
        std::vector<Line*> lines_;
        NotificationArea* notifications_;
        DiceButton* dice_button_;
        ActionPanel* action_panel_;

        std::vector<Hex*> hexes_by_num_[13];

        const int dims_[GRID_SIZE + 2] = {GRID_SIZE - 1,
                GRID_SIZE, GRID_SIZE + 1, GRID_SIZE + 2,
                GRID_SIZE + 1, GRID_SIZE, GRID_SIZE - 1};

        Hex* root_ = NULL;
        sf::RenderWindow* window_;

        mutable sf::CircleShape hexagon_;
        mutable sf::CircleShape mouse_circle_;
        mutable sf::CircleShape point_circle_;
        mutable sf::VertexArray line_array_;
        mutable sf::Text hex_text_;
        mutable sf::Font hex_font_;

        Player* last_requester_;

        void Init();

        void GeneratePoints();
        /**
         * NOTE: Called STRICTLY
         * after GeneratePoints()
         */
        void GenerateLines();
        void GenerateActionPanel();

        void DrawMap() const;
        void DrawPoints() const;
        void DrawLines() const;
        void DrawMousePointer() const;

        Point* AddPoint(Point*);
        Line* AddLine(Line*);

        bool TryAddPoint(Point*);
};

#endif /* defined(__MAP_H__) */
