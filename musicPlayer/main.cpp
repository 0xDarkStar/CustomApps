// Made by: 0xDarkStar
#include <iostream>
#include <sqlite3.h>
using namespace std;

int main() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("song.db", &DB);

    if (exit) {
        cerr << "Error open DB" << sqlite3_errmsg(DB) << endl;
        return -1;
    }
    else
        cout << "Opened Database Successfully!" << endl;
    sqlite3_close(DB);
    return 0;
}