#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "constants.h"
#include "easylogging++.h"
#include "userbase.h"
#include "game.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

int main(int, char const**){
  srand(time(NULL));
  sqlite3* db;

  Map* game_map = new Map();
  game_map -> gen_map();

  db_connect(db);

  cout << "Sign in or sign up?[i/u]: ";
  char iu;
  cin >> iu;
  string username;
  string pass;
  if (iu == 'i'){
    cout << "Login: ";
    cin >> username;
    cout << "Passwd: ";
    cin >> pass;
    int user_id = find_user(db,
          User(username, "", superhash(pass)));
    if (user_id == -1)
      LOG(INFO) << "Login unsuccessful";
    else
      LOG(INFO) << "Login successful";
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

  db_close(db);
  return 0;
}
