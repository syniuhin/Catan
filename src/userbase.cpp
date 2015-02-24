#include "userbase.h"
#include "easylogging++.h"

int db_connect(sqlite3*& db){
    int rc = sqlite3_open("catan.db", &db);
    if (rc){
        LOG(ERROR) << "Error opening db";
    } else {
        LOG(INFO) << "Opened database successfully";
    }
    return rc;
}

int db_close(sqlite3*& db){
    return sqlite3_close(db);
}

static int create_callback(void *NotUsed, int argc, 
    char **argv, char **azColName){
    for (int i = 0; i < argc; i++){
      const char* value  = (argv[i]) ? argv[i] : "NULL";
      LOG(INFO) << azColName[i] << " = " << value;
    }
    return 0;
}

int db_create(sqlite3*& db){
  sqlite3_stmt* createStmt;
  sqlite3_prepare_v2(db, CREATE_TABLE.c_str(), -1,
      &createStmt, 0);
  int res;
  if (SQLITE_OK != (res = sqlite3_step(createStmt)))
    LOG(ERROR) << "db_create: " << sqlite3_errmsg(db);
  sqlite3_finalize(createStmt);
  return res;
}

int db_insert(sqlite3*& db, User user){
  std::string insertString;
  insertString.append("insert into users(")
    .append(COLUMN_NAMES[1]).append(", ")
    .append(COLUMN_NAMES[2]).append(", ")
    .append(COLUMN_NAMES[3]).append(") ")
    .append(" values('")
    .append(user.username)
    .append("', '")
    .append( user.email)
    .append("', '")
    .append(std::to_string(user.password))
    .append("');");
  sqlite3_stmt *stmt;
  sqlite3_prepare(db, insertString.c_str(), 
      insertString.size(), &stmt, NULL);
  int res;
  if (SQLITE_DONE == (res = sqlite3_step(stmt))) 
    LOG(INFO) << "db_insert: successfuly inserted user: " 
      << user.to_str();
  else
    LOG(ERROR) << "db_insert: " << sqlite3_errmsg(db);
  sqlite3_finalize(stmt);
  return res;
}

int db_delete(sqlite3*& db, User user){
  int res;
  return res;
}
