#include "menu.h"

#include <SFML/Graphics.hpp>

#include "easylogging++.h"

#include "constants.h"

Main_menu::Main_menu(sf::RenderWindow* window)
    : text_(),
      font_() {
    this -> window = window;
    font_.loadFromFile("black_jack.ttf");
    text_.setFont(font_);
}

Main_menu::~Main_menu(){
    LOG(INFO) << "Destruction phase";
}

void Main_menu::draw(){
    bool process = false;

    process = draw_background();
    if (process)
        process = draw_text();
    if (process)
        process = draw_buttons();
}

int Main_menu::get_selected_button(){
    return selected_button;
}

bool Main_menu::draw_background(){
    sf::Texture texture;
    if (!texture.loadFromFile("menu_background.jpg"))
        return false;
    sf::Sprite sprite(texture);
    window -> draw(sprite);
    return true;
}

bool Main_menu::draw_text(){
    sf::Font font;
    if (!font.loadFromFile("black_jack.ttf"))
        return false;
    sf::Text text(APP_NAME, font, TEXT_SIZE);
    text.setColor(sf::Color::White);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - TEXT_VERTICAL_OFFSET));

    window -> draw(text);
    return true;
}

bool Main_menu::draw_buttons(){
    if (!buttons_initialized){
        buttons_initialized = true;
        sf::RectangleShape b;
        for (int i = 0; i < BUTTONS_NUM; ++i){
            b.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            b.setOrigin(BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2);
            sf::Vector2f pos(SCREEN_WIDTH / 2.0f,
                    SCREEN_HEIGHT / 2.0f + BUTTON_VERTICAL_OFFSET
                    + i * (BUTTON_HEIGHT + BUTTON_MARGIN));
            b.setPosition(pos);

            buttons[i] = b;
        }
    } else {
        refresh_buttons();
        highlight_buttons();
    }
    for (int i = 0; i < BUTTONS_NUM; ++i) {
        window -> draw(buttons[i]);

        text_.setColor(sf::Color::Black);
        text_.setCharacterSize(24);
        text_.setString( (i)
                ? "Exit"
                : "Start");
        sf::FloatRect rect = text_.getLocalBounds();
        text_.setOrigin(rect.width / 2, rect.height / 2);
        text_.setPosition(buttons[i].getPosition());
        window -> draw(text_);
    }
    return true;
}

bool Main_menu::highlight_buttons(){
    selected_button = -1;
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
    //LOG(INFO) << "mouse_pos: " << mouse_pos.x << " " << mouse_pos.y;
    for (int i = 0; i < BUTTONS_NUM; ++i){
        sf::Vector2f pos = buttons[i].getPosition();
        sf::Rect<float> rect(pos.x - BUTTON_WIDTH / 2, pos.y - BUTTON_HEIGHT / 2,
                             BUTTON_WIDTH, BUTTON_HEIGHT);
        if (rect.contains((float)mouse_pos.x, (float)mouse_pos.y)){
            //LOG(INFO) << "OK button_pos: " << pos.x << " " << pos.y;
            buttons[i].setFillColor(sf::Color::Blue);
            selected_button = i;
        }
    }
    return true;
}

void Main_menu::refresh_buttons(){
    for (int i = 0; i < BUTTONS_NUM; ++i)
        buttons[i].setFillColor(sf::Color::White);
}
