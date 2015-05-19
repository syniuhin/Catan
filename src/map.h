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

static const int TYPE_COUNT[] = {0, 0, 3, 4, 3, 4, 4};
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

class MapObject {
    public:
        virtual ~MapObject();

        sf::Vector2f get_pos() const;
        void set_pos(sf::Vector2f);

        virtual void Click() = 0;
        virtual bool OnMouse(sf::Vector2i) const = 0;
        //TODO: make it pure virtual, using tr1::shared_ptr
        virtual void Draw(sf::RenderWindow*);
    protected:
        MapObject();
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

        std::vector<int> GetBoundPlayersIds() const;
    private:
        int num_;
        int type_;

        Point* points_[6] = {NULL};

        void AddPoint(Point*, int index);

        int GetIndexForPoint(Point*);
};

class Line;

class Point : public MapObject {
    public:
        Point(Hex* first_,
                Hex* second_,
                Hex* third_);
        ~Point();

        static bool AreAdjacent(const Point, const Point);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        bool CanBeOwned(const int _id);

        void AddLine(Line*);
        int get_owner_id() const;
        void set_owner_id(int);

        Hex** get_hexes(Hex**) const;
        std::set<Hex*> get_hexes();
    private:
        std::set<Hex*> hexes_;
        std::set<Line*> lines_;
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

class Button : public MapObject {
    public:
        static Button* CreateInstance(sf::Vector2f pos,
                                      sf::Vector2f b_size);

        virtual void Click();
        virtual bool OnMouse(sf::Vector2i) const;

        virtual void Draw(sf::RenderWindow*);

        /**
         * Builder method(s)
         */
        Button* AddCallback(std::function<void()> cb);
        Button* SetColors(sf::Color idle, sf::Color focused);
    protected:
        std::vector<std::function<void()> > callbacks_;
        sf::RectangleShape shape_;
        sf::Color idle_;
        sf::Color focused_;

        explicit Button(sf::Vector2f b_size);
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

class PlayerCard : public Button {
    public:
        static PlayerCard* CreateInstance(sf::Vector2f pos, Player&);

        void Draw(sf::RenderWindow*);
    private:
        sf::RectangleShape playerpic_shape_;
        sf::Text name_text_;
        sf::Text resources_text_;

        sf::Font font_;

        Player& player_;

        PlayerCard(sf::Vector2f sz, Player&);
};

class PlayerPanel : public MapObject {
    public:
        static PlayerPanel* CreateInstance(int* lpc);

        void Click();
        bool OnMouse(sf::Vector2i) const;

        void Draw(sf::RenderWindow*);

        void Insert(Player&);
    private:
        sf::RenderWindow* window_;
        sf::RectangleShape panel_shape_;
        sf::Color panel_color_;

        std::vector<MapObject*> player_cards_;
        int* lpc_;

        PlayerPanel();
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
        bool AddVillage();

        /**
         * Same note here
         */
        Line* AddRoad(Player*);

        void AddButton(Button*);
       /**
         * Generates triples by dice sum:
         * <player_id, terrain_type, quantity>
         */
        std::vector<Triple<int, int, int> >
                GenerateResources(int num);

        void AddRandomVillageRoad(Player*);

        void DisplayPlayersInfo(std::vector<Player*>);

        int get_lpc() const;
    private:
        std::vector<MapObject*> map_objects_;

        std::vector<Hex*> hexes_;
        std::vector<Point*> points_;
        std::vector<Line*> lines_;
        NotificationArea* notifications_;
        ActionPanel* action_panel_;
        PlayerPanel* player_panel_;

        std::vector<Hex*> hexes_by_num_[13];

        const int dims_[GRID_SIZE + 2] = {GRID_SIZE - 1,
                GRID_SIZE, GRID_SIZE + 1, GRID_SIZE + 2,
                GRID_SIZE + 1, GRID_SIZE, GRID_SIZE - 1};

        Hex* root_ = NULL;
        sf::RenderWindow* window_;

        mutable sf::CircleShape hexagon_;

        sf::Texture sea_texture_;
        sf::Texture desert_texture_;
        sf::Texture hills_texture_;
        sf::Texture pasture_texture_;
        sf::Texture mountains_texture_;
        sf::Texture fields_texture_;
        sf::Texture forest_texture_;

        sf::Texture village_texture_;
        std::vector<sf::Texture> textures_;

        mutable sf::Sprite sea_sprite_;
        mutable sf::Sprite desert_sprite_;
        mutable sf::Sprite hills_sprite_;
        mutable sf::Sprite pasture_sprite_;
        mutable sf::Sprite mountains_sprite_;
        mutable sf::Sprite fields_sprite_;
        mutable sf::Sprite forest_sprite_;

        mutable sf::Sprite village_sprite_;

        mutable sf::CircleShape point_circle_;
        mutable sf::VertexArray line_array_;
        mutable sf::Text hex_text_;
        mutable sf::Font hex_font_;

        Player* last_requester_;
        int last_player_clicked_ = -1;

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

        Point* AddPoint(Point*);
        Line* AddLine(Line*);

        bool TryAddPoint(Point*);
};

#endif /* defined(__MAP_H__) */
