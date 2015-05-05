#ifndef __TRADE_H__
#define __TRADE_H__

#include "constants.h"

#include <set>
#include <functional>

static const sf::Vector2f TRADE_WIN_SIZE =
        sf::Vector2f(TRADE_WINDOW_WIDTH, TRADE_WINDOW_HEIGHT + 50);

static const sf::Vector2f RESOURCE_CELL_SIZE =
        sf::Vector2f(TRADE_WIN_SIZE.x / 5 - 20, TRADE_WIN_SIZE.y - 70);

class UiObject {
    public:
        virtual void Draw(sf::RenderWindow*);
        virtual void Click(sf::RenderWindow*);
        virtual bool OnMouse(sf::Vector2i) const = 0;
        virtual void AddChild(UiObject*);

        sf::Vector2f get_pos() const;
        void set_pos(sf::Vector2f);
    protected:
        sf::Vector2f pos_;
        std::set<UiObject*> children_;

        UiObject();
        UiObject(sf::Vector2f);
};

class ResourceCell;

class TradeWindow : public UiObject {
    public:
        static TradeWindow* CreateInstance(int seller);

        void Draw(sf::RenderWindow*);
        bool OnMouse(sf::Vector2i) const;

        void set_seller(int);
        void set_on_propose(std::function<void()>);

        int** get_balance(int**);
    private:
        sf::RectangleShape shape_;
        const sf::Color bg_color_ = sf::Color(73, 66, 255, 245);
        sf::Font font_;

        int give_balance[5] = {0, 0, 0, 0, 0};
        int take_balance[5] = {0, 0, 0, 0, 0};

        int seller_id_;
        std::function<void()> on_propose_ = [] () {};

        TradeWindow();

        void PlusGiven(int res_id);
        void MinusGiven(int res_id);
        void PlusTaken(int res_id);
        void MinusTaken(int res_id);

        void UpdateValues();
};

class ResourceCell : public UiObject {
    public:
        static ResourceCell* CreateInstance(int res_id,
                std::function<void()> pg,
                std::function<void()> mg,
                std::function<void()> pt,
                std::function<void()> mt,
                sf::Vector2f pos);

        void Draw(sf::RenderWindow*);
        bool OnMouse(sf::Vector2i) const;

        void SetValues(int given, int taken);

        int get_id();
    private:
        int res_id_;
        int given_, taken_;

        sf::RectangleShape shape_;
        const sf::Color brick_color = sf::Color(191, 89, 0, 255);
        const sf::Color wool_color = sf::Color(232, 232, 232, 255);
        const sf::Color ore_color = sf::Color(85, 85, 125, 255);
        const sf::Color grain_color = sf::Color(235, 255, 107, 255);
        const sf::Color lumber_color = sf::Color(14, 163, 0, 255);

        sf::Font font_;
        sf::Text text_give_;
        sf::Text text_take_;

        ResourceCell(int res_id);
};

class TradeButton : public UiObject {
    public:
        TradeButton();

        void Draw(sf::RenderWindow*);
        void Click(sf::RenderWindow*);
        bool OnMouse(sf::Vector2i) const;

        TradeButton* set_position(sf::Vector2f);
        TradeButton* set_callback(std::function<void()> cb);
        TradeButton* set_color(sf::Color);
        TradeButton* set_size(sf::Vector2f);
    private:
        std::function<void()> cb_ = [] () {};
        sf::RectangleShape shape_;
};
#endif
