#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "constants.h"
#include "easylogging++.h"
#include "userbase.h"

INITIALIZE_EASYLOGGINGPP


long long int superhash(char pass[20]){
    long long int res = 0;
    for (int i = 0, p = 1; i < 20 && pass[i] != 0; ++i, p *= 31)
      res += (pass[i] - 'a') * p;
    return res;
}

int main(int, char const**){
	char username[20];
	char pass[20];
	char email[20];
	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter email: ");
	scanf("%s", email);
	printf("Enter password: ");
	scanf("%s", pass);

    sqlite3* db;
    db_connect(db);
    db_create(db);
    db_insert(db, User(username, email, superhash(pass)));
    db_close(db);
    return 0;
}
