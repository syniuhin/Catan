//
//  userbase.h
//  Catan
//
//  Created by Illia Syniuhin on 2/21/15.
//  Copyright (c) 2015 Illia Syniuhin. All rights reserved.
//

#ifndef __Catan__userbase__
#define __Catan__userbase__

#include <stdio.h>
#include <sqlite3.h>

struct User {
    int id;
    char* username;
    char* email;
    long long int password;
};

typedef struct User User;

char* CREATE_TABLE = "CREATE TABLE USERS("  \
                     "ID             INTEGER PRIMARY KEY NOT NULL," \
                     "USERNAME       TEXT    NOT NULL," \
                     "EMAIL          TEXT    NOT NULL," \
                     "PASSWORD       TEXT    NOT NULL);";

const char* TABLE = "USERS";
const int ID_COL = 0;
const int USERNAME_COL = 1;
const int EMAIL_COL = 2;
const int PASSWORD_COL = 3;

int db_connect(sqlite3* db);
int db_create(sqlite3* db);
int db_insert(User user, sqlite3 *db);
int db_delete(User user, sqlite3 *db);

#endif /* defined(__Catan__userbase__) */
