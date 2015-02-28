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
  if (user.username.empty() || user.email.empty())
    return 0;
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

/**
 * Takes (partially constructed) User
 * instance and tries to delete it
 */
int db_delete(sqlite3*& db, User user){
  if ((user.username.empty() &&
      user.email.empty()) &&
      user.id < 0)
    return 0;

  std::string deleteString;
  deleteString.append("delete from users where ");
  if (user.id >= 0){
    deleteString.append("id = ")
      .append(std::to_string(user.id));
  } else if (!user.username.empty()){
    deleteString.append("username = '")
      .append(user.username)
      .append("' ");
  } else if (!user.email.empty()){
    deleteString.append("email = '")
      .append(user.email)
      .append("' ");
  }
  deleteString.append(";");
  sqlite3_stmt *stmt;
  sqlite3_prepare(db, deleteString.c_str(), 
      deleteString.size(), &stmt, NULL);
  int res;
  if (SQLITE_DONE == (res = sqlite3_step(stmt)))
    LOG(INFO) << "db_delete: success " << user.to_str();
  else
    LOG(ERROR) << "db_delete: " << sqlite3_errmsg(db);
  return res;
}

long long int superhash(std::string passwd){
    long long int res = 0;
    for (int i = 0, p = 1; i < passwd.length(); ++i, p *= 31)
      res += (passwd[i] - 'a') * p;
    return res;
}
