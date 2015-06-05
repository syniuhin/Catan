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

void UiObject::Click(sf::RenderWindow* window) {
    sf::Vector2i cursor = sf::Mouse::getPosition(*window);
    for (std::set<UiObject*>::iterator it = children_.begin();
            it != children_.end(); ++it) {
        if ((*it) -> OnMouse(cursor))
            (*it) -> Click(window);
    }
}

void UiObject::AddChild(UiObject* child) {
    children_.insert(child);
}

sf::Vector2f UiObject::get_pos() const {
    return pos_;
}

void UiObject::set_pos(sf::Vector2f p) {
    pos_ = p;
}

TradeWindow* TradeWindow::CreateInstance(int seller) {
    TradeWindow* instance = new TradeWindow;
    instance -> seller_id_ = seller;

    instance -> shape_.setPosition(instance -> pos_);
    instance -> shape_.setFillColor(instance -> bg_color_);

    //TODO: handle errors
    instance -> brick_texture_.loadFromFile("brick_small.png");
    instance -> brick_texture_.setRepeated(true);
    instance -> wool_texture_.loadFromFile("wool_small.png");
    instance -> wool_texture_.setRepeated(true);
    instance -> ore_texture_.loadFromFile("ore_small.png");
    instance -> ore_texture_.setRepeated(true);
    instance -> grain_texture_.loadFromFile("grain_small.png");
    instance -> grain_texture_.setRepeated(true);
    instance -> lumber_texture_.loadFromFile("lumber_small.png");
    instance -> lumber_texture_.setRepeated(true);

    instance -> button_texture_.loadFromFile("trade_button.png");

    sf::Texture* pts[] = { &(instance -> brick_texture_),
        &(instance -> wool_texture_),
        &(instance -> ore_texture_),
        &(instance -> grain_texture_),
        &(instance -> lumber_texture_)};
    for (int i = 0; i < 5; ++i) {
        instance -> children_
            .insert(ResourceCell::CreateInstance(i,
                        [instance, i] () { instance -> PlusGiven(i); },
                        [instance, i] () { instance -> MinusGiven(i); },
                        [instance, i] () { instance -> PlusTaken(i); },
                        [instance, i] () { instance -> MinusTaken(i); },
                        instance -> pos_ +
                            sf::Vector2f(10 +
                                i * (RESOURCE_CELL_SIZE.x + 20),
                            10),
                        *(pts[i])));
    }

    sf::Vector2f go_btn_size = sf::Vector2f(200, 40);
    sf::Vector2f go_btn_pos = instance -> pos_ + sf::Vector2f(
            .5f * (TRADE_WINDOW_WIDTH - go_btn_size.x),
            TRADE_WINDOW_HEIGHT);

    instance -> AddChild((new TradeButton())
            -> set_position(go_btn_pos)
            -> set_texture(instance -> button_texture_)
//            -> set_color(sf::Color::Red)
            -> set_size(go_btn_size)
            -> set_callback([instance] () {
                   instance -> on_propose_();
                }));

    if (!instance -> font_.loadFromFile("cb.ttf"))
        return NULL;
    instance -> button_text_.setFont(instance -> font_);
    instance -> button_text_.setColor(
            sf::Color(220, 230, 230, 255));
    instance -> button_text_.setCharacterSize(18);
    instance -> button_text_.setString("next");
    sf::FloatRect brect = instance -> button_text_.getLocalBounds();
    instance -> button_text_.setPosition(go_btn_pos +
            (go_btn_size - sf::Vector2f(brect.width,
                                        1.5 * brect.height)) * .5f);

    return instance;
}

TradeWindow::TradeWindow()
    : UiObject((sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) -
                sf::Vector2f(TRADE_WINDOW_WIDTH, TRADE_WINDOW_HEIGHT)) * .5f),
      button_text_(),
      shape_(TRADE_WIN_SIZE) {}

void TradeWindow::Draw(sf::RenderWindow* window) {
    window -> draw(shape_);
    UiObject::Draw(window);
    window -> draw(button_text_);
}

bool TradeWindow::OnMouse(sf::Vector2i cursor) const {
    return false;
}

void TradeWindow::set_seller(int s) {
    seller_id_ = s;
}

void TradeWindow::set_on_propose(std::function<void()> f) {
    on_propose_ = f;
}

int** TradeWindow::get_balance(int** balance) {
    balance[0] = give_balance;
    balance[1] = take_balance;
    return balance;
}

void TradeWindow::PlusGiven(int res_id) {
    give_balance[res_id]++;
    UpdateValues();
}

void TradeWindow::MinusGiven(int res_id) {
    if (give_balance[res_id])
        give_balance[res_id]--;
    UpdateValues();
}

void TradeWindow::PlusTaken(int res_id) {
    take_balance[res_id]++;
    UpdateValues();
}

void TradeWindow::MinusTaken(int res_id) {
    if (take_balance[res_id])
        take_balance[res_id]--;
    UpdateValues();
}

void TradeWindow::UpdateValues() {
    for (std::set<UiObject*>::iterator it = children_.begin();
            it != children_.end(); ++it) {
        ResourceCell* cell = dynamic_cast<ResourceCell*>((*it));
        if (cell) {
            int res_id = cell -> get_id();
            cell -> SetValues(give_balance[res_id],
                    take_balance[res_id]);
        }
    }
}

void TradeWindow::ClearValues() {
    for (int i = 0; i < 5; ++i)
        give_balance[i] = take_balance[i] = 0;
    UpdateValues();
}

ResourceCell* ResourceCell::CreateInstance(int res_id,
            std::function<void()> pg,
            std::function<void()> mg,
            std::function<void()> pt,
            std::function<void()> mt,
            sf::Vector2f pos,
            sf::Texture& texture) {
    ResourceCell* instance = new ResourceCell(res_id);
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
    instance -> sprite_.setPosition(pos);
    instance -> sprite_.setTexture(texture);
    instance -> sprite_.setTextureRect({ 0, 0,
            (int) RESOURCE_CELL_SIZE.x,
            (int) RESOURCE_CELL_SIZE.y });
    if (!instance -> font_.loadFromFile("cb.ttf")) {
        return NULL;
    }
    instance -> text_give_.setFont(instance -> font_);
    instance -> text_give_.setCharacterSize(15);
    instance -> text_give_.setColor(sf::Color::Black);
    instance -> text_give_.setString("0");
    instance -> text_give_.setPosition(instance -> pos_ +
            sf::Vector2f((RESOURCE_CELL_SIZE.x -
                    instance -> text_give_
                    .getLocalBounds().width) / 2, 10));

    instance -> text_take_.setFont(instance -> font_);
    instance -> text_take_.setCharacterSize(15);
    instance -> text_take_.setColor(sf::Color::Black);
    instance -> text_take_.setString("0");
    instance -> text_take_.setPosition(instance -> pos_ +
            sf::Vector2f((RESOURCE_CELL_SIZE.x -
                    instance -> text_take_
                    .getLocalBounds().width) / 2, RESOURCE_CELL_SIZE.y - 30));
    instance -> underlying_shape_.setFillColor(
            sf::Color(255, 255, 255, 100));

    sf::Vector2f ul = instance -> pos_ +
            sf::Vector2f(5, 5);
    sf::Vector2f dr = instance -> pos_ +
            RESOURCE_CELL_SIZE - sf::Vector2f(35, 35);
    sf::Vector2f bsz = sf::Vector2f(30, 30);

    //TODO: handle this
    instance -> plus_texture_.loadFromFile("plus_small.png");
    instance -> minus_texture_.loadFromFile("minus_small.png");
    instance -> AddChild(
            (new TradeButton())
                         -> set_position(ul)
//                         -> set_color(sf::Color::Black)
                         -> set_texture(instance -> plus_texture_)
                         -> set_size(bsz)
                         -> set_callback(pg));
    instance -> AddChild(
            (new TradeButton())
                         -> set_position(sf::Vector2f(dr.x, ul.y))
//                         -> set_color(sf::Color::White)
                         -> set_texture(instance -> minus_texture_)
                         -> set_size(bsz)
                         -> set_callback(mg));
    instance -> AddChild(
            (new TradeButton())
                         -> set_position(sf::Vector2f(ul.x, dr.y))
//                         -> set_color(sf::Color::Black)
                         -> set_texture(instance -> plus_texture_)
                         -> set_size(bsz)
                         -> set_callback(pt));
    instance -> AddChild(
            (new TradeButton())
                         -> set_position(dr)
//                         -> set_color(sf::Color::White)
                         -> set_texture(instance -> minus_texture_)
                         -> set_size(bsz)
                         -> set_callback(mt));
    return instance;
}

ResourceCell::ResourceCell(int res_id)
    : res_id_(res_id),
      given_(0),
      taken_(0),
      plus_texture_(),
      minus_texture_(),
      sprite_(),
      underlying_shape_(sf::Vector2f(RESOURCE_CELL_SIZE.x - 6, 34)),
      shape_(RESOURCE_CELL_SIZE) {}

void ResourceCell::Draw(sf::RenderWindow* window) {
//    window -> draw(shape_);
    window -> draw(sprite_);

    underlying_shape_.setPosition(pos_ + sf::Vector2f(3, 3));
    window -> draw(underlying_shape_);

    underlying_shape_.setPosition(pos_ +
            sf::Vector2f(3, RESOURCE_CELL_SIZE.y - 38));
    window -> draw(underlying_shape_);

    window -> draw(text_give_);
    window -> draw(text_take_);
    UiObject::Draw(window);
}

bool ResourceCell::OnMouse(sf::Vector2i cursor) const {
    return shape_
        .getGlobalBounds()
        .contains((float) cursor.x, (float) cursor.y);
}

void ResourceCell::SetValues(int given, int taken) {
    given_ = given;
    taken_ = taken;

    text_give_.setString(std::to_string(given));
    text_take_.setString(std::to_string(taken));
}

int ResourceCell::get_id() {
    return res_id_;
}

TradeButton::TradeButton()
    : shape_(),
      sprite_() {}

void TradeButton::Draw(sf::RenderWindow* window) {
//    window -> draw(shape_);
    window -> draw(sprite_);
    UiObject::Draw(window);
}

void TradeButton::Click(sf::RenderWindow* window) {
    cb_();
    UiObject::Click(window);
}

bool TradeButton::OnMouse(sf::Vector2i cursor) const {
    return shape_
            .getGlobalBounds()
            .contains((float)cursor.x, (float)cursor.y);
}

TradeButton* TradeButton::set_position(sf::Vector2f pos) {
    pos_ = pos;
    shape_.setPosition(pos);
    sprite_.setPosition(pos);
    return this;
}

TradeButton* TradeButton::set_callback(std::function<void()> cb) {
    cb_ = cb;
    return this;
}

TradeButton* TradeButton::set_color(sf::Color color) {
    shape_.setFillColor(color);
    return this;
}

TradeButton* TradeButton::set_size(sf::Vector2f sz) {
    shape_.setSize(sz);

    sf::Vector2u isz = sprite_.getTexture() -> getSize();
    sf::Vector2f usz = sf::Vector2f(
            isz.x / sz.x,
            isz.y / sz.y);
    sprite_.setScale(usz);
    return this;
}

TradeButton* TradeButton::set_texture(const sf::Texture& texture) {
    sprite_.setTexture(texture);
    return this;
}
