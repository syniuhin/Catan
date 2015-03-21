//
//  menu.h
//  Catan
//
//  Created by Illia Syniuhin on 2/20/15.
//  Copyright (c) 2015 Illia Syniuhin. All rights reserved.
//

#ifndef __Catan__menu__
#define __Catan__menu__

#include <stdio.h>
#include <SFML/Graphics.hpp>

const int TEXT_VERTICAL_OFFSET = 100;
const int TEXT_SIZE = 60;

const int BUTTONS_NUM = 5;
const float BUTTON_WIDTH = 180;
const float BUTTON_HEIGHT = 35;
const float BUTTON_MARGIN = 10;
const int BUTTON_VERTICAL_OFFSET = 40;

class Main_menu {
public:
    Main_menu(sf::RenderWindow*);
    ~Main_menu();

    void draw();

    int get_selected_button();
private:
    sf::RenderWindow* window;

    sf::RectangleShape buttons[BUTTONS_NUM];
    bool buttons_initialized;
    int selected_button;

    bool draw_background();
    bool draw_text();
    bool draw_buttons();

    bool highlight_buttons();
    void refresh_buttons();
};

#endif /* defined(__Catan__menu__) */
