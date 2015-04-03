#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "easylogging++.h"

#include "constants.h"
#include "game.h"
#include "menu.h"
#include "userbase.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

void auth(sqlite3*);
int process_menu(sf::RenderWindow*, Main_menu*);
void start_game(sf::RenderWindow*);

int main(int, char const**){
    srand(time(NULL));
    sqlite3* db;

    db_connect(db);
//    auth(db);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,
                SCREEN_HEIGHT), APP_NAME);
    window.setFramerateLimit(30);
//    Main_menu menu(&window);
//    int clicked_button = process_menu(&window, &menu);
//    switch (clicked_button){
//        case 0:
//            //start game
//            game_map.generate();
//            window.clear();
//            game_map.draw(&window);
//            window.display();
//            break;
//        case 4:
//            window.close();
//            break;
//        default:
//            break;
//    }
    start_game(&window);
    db_close(db);
    return 0;
}

void auth(sqlite3* db){
    string username;
    string pass;
    int user_ok = 0;
    while (!user_ok){
        cout << "Sign in or sign up?[i/u]: ";
        char iu;
        cin >> iu;
        if (iu == 'i'){
            cout << "Login: ";
            cin >> username;
            cout << "Passwd: ";
            cin >> pass;
            int user_id = find_user(db,
                  User(username, "", superhash(pass)));
            if (user_id == -1){
                cout << "Login unsuccessful" << endl;
            } else {
                user_ok = 1;
                LOG(INFO) << "Login successful";
            }
        } else if (iu == 'u'){
            std::string email;
            cout << "Login: ";
            cin >> username;
            cout << "Email: ";
            cin >> email;
            cout << "Passwd: ";
            cin >> pass;
            db_insert(db, User(username, email, superhash(pass)));
        }
    }
}

int process_menu(sf::RenderWindow* window, Main_menu* menu){
    while (window -> isOpen()){
        sf::Event event;
        while (window -> pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window -> close();
                    break;
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.button == sf::Mouse::Left){
                        int clicked_button = menu -> get_selected_button();
                        LOG(INFO) << "clicked_button is " << clicked_button;
                        if (clicked_button != -1)
                            return clicked_button;
                    }
                }
                    break;
                default:
                    break;
            }
        }
        window -> clear(sf::Color::Black);
        menu -> draw();
        window -> display();
    }
    return -1;
}

void start_game(sf::RenderWindow* window){
    std::vector<Player*> players;
    for (int i = 0; i < 4; ++i)
        players.push_back(new Player(i));
    Game game = Game(new Map(), players, window);
    game.GenMap();

    while (window -> isOpen()){
        game.Update();
    }
}
