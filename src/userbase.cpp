//
//  userbase.c
//  Catan
//
//  Created by Illia Syniuhin on 2/21/15.
//  Copyright (c) 2015 Illia Syniuhin. All rights reserved.
//

#include "userbase.h"

int db_connect(sqlite3* db){
    int rc = sqlite3_open("catan.db", &db);
    if (rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    return rc;
}

int db_create(sqlite3* db){
    return 0;
}

int db_insert(User user, sqlite3 *db){
    return 0;
}

int db_delete(User user, sqlite3 *db){
    return 0;
}
