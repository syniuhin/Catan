#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "constants.h"
#include "easylogging++.h"
#include "userbase.h"

INITIALIZE_EASYLOGGINGPP

int main(int, char const**){
    sqlite3* db;
    db_connect(db);

	char username[20];
	char pass[20];

    db_close(db);
    return 0;
}
