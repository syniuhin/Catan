#ifndef __TRADE_H__
#define __TRADE_H__

#include "constants.h"

#include <set>

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

class TradeWindow : public UiObject {
    public:
        static TradeWindow* CreateInstance();

        void Draw(sf::RenderWindow*);
        void Click();
        bool OnMouse(sf::Vector2i) const;
    private:
        sf::RectangleShape shape_;
        const sf::Color bg_color_ = sf::Color(73, 66, 255, 196);
        TradeWindow();
};

#endif
