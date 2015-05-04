#ifndef __TRADE_H__
#define __TRADE_H__

#include "constants.h"

#include <set>

static const sf::Vector2f TRADE_WIN_SIZE =
        sf::Vector2f(TRADE_WINDOW_WIDTH, TRADE_WINDOW_HEIGHT);

static const sf::Vector2f RESOURCE_CELL_SIZE =
        sf::Vector2f(TRADE_WIN_SIZE.x / 5 - 20, TRADE_WIN_SIZE.y - 20);

class UiObject {
    public:
        virtual void Draw(sf::RenderWindow*);
        virtual void Click() = 0;
        virtual bool OnMouse(sf::Vector2i) const = 0;

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
        static TradeWindow* CreateInstance();

        void Draw(sf::RenderWindow*);
        void Click();
        bool OnMouse(sf::Vector2i) const;
    private:
        sf::RectangleShape shape_;
        const sf::Color bg_color_ = sf::Color(73, 66, 255, 245);
        TradeWindow();
};

class ResourceCell : public UiObject {
    public:
        static ResourceCell* CreateInstance(int res_id, sf::Vector2f pos);

        void Draw(sf::RenderWindow*);
        void Click();
        bool OnMouse(sf::Vector2i) const;
    private:
        sf::RectangleShape shape_;
        const sf::Color brick_color = sf::Color(191, 89, 0, 255);
        const sf::Color wool_color = sf::Color(232, 232, 232, 255);
        const sf::Color ore_color = sf::Color(85, 85, 125, 255);
        const sf::Color grain_color = sf::Color(235, 255, 107, 255);
        const sf::Color lumber_color = sf::Color(14, 163, 0, 255);

        sf::Font font_;
        sf::Text text_give_;
        sf::Text text_take_;

        ResourceCell();
};
#endif
