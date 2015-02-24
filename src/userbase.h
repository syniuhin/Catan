#ifndef __Catan__userbase__
#define __Catan__userbase__

#include <stdio.h>
#include <sqlite3.h>
#include <string>

struct User {
    int id;
    std::string username;
    std::string email;
    long long int password;
public:
    User() {}
    User(std::string username, std::string email, 
        long long int password){
        this -> id = -1;
        this -> username = username;
        this -> email = email;
        this -> password = password;
    }
    std::string to_str() {
        return username
          .append(" ")
          .append(email)
          .append(" ")
          .append(std::to_string(password));
    }
};

typedef struct User User;

static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS USERS("  \
                           "ID             INTEGER PRIMARY KEY AUTOINCREMENT," \
                           "USERNAME       TEXT    NOT NULL UNIQUE," \
                           "EMAIL          TEXT    NOT NULL UNIQUE," \
                           "PASSWORD       TEXT    NOT NULL);";

//static const char* TABLE = "USERS";
const int ID_COL = 0;
const int USERNAME_COL = 1;
const int EMAIL_COL = 2;
const int PASSWORD_COL = 3;
const std::string COLUMN_NAMES[] = {"ID", "USERNAME", 
  "EMAIL", "PASSWORD"};

int db_connect(sqlite3*& db);
int db_close(sqlite3*& db);
int db_create(sqlite3*& db);
int db_insert(sqlite3*& db, User user);
int db_delete(sqlite3*& db, User user);

#endif /* defined(__Catan__userbase__) */
