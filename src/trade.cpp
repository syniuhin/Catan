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

    for (int i = 0; i < 5; ++i) {
        instance -> children_
            .insert(ResourceCell::CreateInstance(i,
                    instance -> pos_ +
                        sf::Vector2f(10 +
                            i * (RESOURCE_CELL_SIZE.x + 20),
                        10)));
    }
    return instance;
}

TradeWindow::TradeWindow()
    : UiObject((sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) -
                sf::Vector2f(TRADE_WINDOW_WIDTH, TRADE_WINDOW_HEIGHT)) * .5f),
      shape_(TRADE_WIN_SIZE) {}

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

ResourceCell* ResourceCell::CreateInstance(int res_id,
            sf::Vector2f pos) {
    ResourceCell* instance = new ResourceCell;
    switch (res_id) {
        case BRICKS_ID:
            instance -> shape_.setFillColor(instance -> brick_color);
            break;
        case WOOL_ID:
            instance -> shape_.setFillColor(instance -> wool_color);
            break;
        case ORE_ID:
            instance -> shape_.setFillColor(instance -> ore_color);
            break;
        case GRAIN_ID:
            instance -> shape_.setFillColor(instance -> grain_color);
            break;
        case LUMBER_ID:
            instance -> shape_.setFillColor(instance -> lumber_color);
            break;
    }

    instance -> pos_ = pos;
    instance -> shape_.setPosition(pos);
    return instance;
}

ResourceCell::ResourceCell()
    : shape_(RESOURCE_CELL_SIZE) {}

void ResourceCell::Draw(sf::RenderWindow* window) {
    window -> draw(shape_);
}

void ResourceCell::Click() {}

bool ResourceCell::OnMouse(sf::Vector2i cursor) const {
    return shape_
        .getGlobalBounds()
        .contains((float) cursor.x, (float) cursor.y);
}
