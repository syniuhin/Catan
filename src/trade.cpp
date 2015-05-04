#include "trade.h"

#include "easylogging++.h"

#include <SFML/Graphics.hpp>

UiObject::UiObject()
    : pos_() {}

UiObject::UiObject(sf::Vector2f pos)
    : pos_(pos) {}

void UiObject::Draw(sf::RenderWindow* window) {
    for (std::set<UiObject*>::iterator it = children_.begin();
            it != children_.end(); ++it)
        (*it) -> Draw(window);
}

sf::Vector2f UiObject::get_pos() const {
    return pos_;
}

void UiObject::set_pos(sf::Vector2f p) {
    pos_ = p;
}

TradeWindow* TradeWindow::CreateInstance() {
    TradeWindow* instance = new TradeWindow;
    instance -> shape_.setPosition(instance -> pos_);
    instance -> shape_.setFillColor(instance -> bg_color_);
    return instance;
}

TradeWindow::TradeWindow()
    : UiObject((sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) -
                sf::Vector2f(TRADE_WINDOW_WIDTH, TRADE_WINDOW_HEIGHT)) * .5f),
      shape_(sf::Vector2f(TRADE_WINDOW_WIDTH,
                TRADE_WINDOW_HEIGHT)) {}

void TradeWindow::Draw(sf::RenderWindow* window) {
    window -> draw(shape_);
    UiObject::Draw(window);
}

void TradeWindow::Click() {
    LOG(INFO) << "TradeWindow clicked";
}

bool TradeWindow::OnMouse(sf::Vector2i cursor) const {
    return false;
}

