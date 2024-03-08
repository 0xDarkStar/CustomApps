// Made by: 0xDarkStar
#include <iostream>
#include <sqlite3.h>
using namespace std;

int main() {
    return 0;
}

int create_table() {

    string sql = "CREATE TABLE PERSON("
                 "INT ID PRIMARY KEY      NOT NULL, "
                 "Path           TEXT     NOT NULL, "
                 "Title          TEXT     NOT NULL, "
                 "Artist         TEXT, "
                 "Length         INT      NOT NULL, "
                 "Subs path      TEXT, "
                 "Playlists      Insert datatype here    NOT NULL, ";

}

int create_database() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("songs.db", &DB);

    if (exit) {
        cerr << "Error open DB" << sqlite3_errmsg(DB) << endl;
        return -1;
    }
    else
        cout << "Opened Database Successfully!" << endl;
    sqlite3_close(DB);
}