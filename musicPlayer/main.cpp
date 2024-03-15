// Made by: 0xDarkStar
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int create_table() {
    sqlite3* DB;
    string sql = "CREATE TABLE Songs("
                 "ID             INT      NOT NULL, "
                 "Title          TEXT     NOT NULL, "
                 "Artist         TEXT, "
                 "Has_subs       INT      NOT NULL, "
                 "Length         INT      NOT NULL,"
                 "PRIMARY KEY (ID) );";
    int exit = 0;
    exit = sqlite3_open("main.db", &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error create table" << endl;
        sqlite3_free(messaggeError);
    }
    else
        cout << "Table created successfully" << endl;
    sqlite3_close(DB);
    return 0;
}

int create_database() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("main.db", &DB);

    if (exit) {
        cerr << "Error open DB" << sqlite3_errmsg(DB) << endl;
        return 1;
    }
    else
        cout << "Opened database successfully!" << endl;
    sqlite3_close(DB);
    return 0;
}

static int callback(void* data, int argc, char** argv, char**azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int populate_songs_table(int id, string title, string artist, bool has_subs, int length) { // length is in seconds
    sqlite3* DB;
    char* messaggeError;
    int exit = 0;
    string sql = "INSERT INTO Songs VALUES(" +
                 to_string(id) + ", '" +
                 title + "', '" +
                 artist + "', ";
    sql +=       (has_subs) ? "true":"false";
    sql +=       ", " + to_string(length) + ");";
    
    string query = "SELECT * FROM Songs;";
    cout << "State of table before insert" << endl;
    exit = sqlite3_open("main.db", &DB);
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error adding values" << endl;
        sqlite3_free(messaggeError);
    }
    else
        cout << "Values added successfully" << endl;
    cout << "State of table after insert" << endl;
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
    sqlite3_close(DB);
    return 0;
}

int delete_song(int id) {
    sqlite3* DB;
    char* messaggeError;
    int exit = 0;
    string sql = "DELETE FROM Songs WHERE ID = " + to_string(id) + ";";
    exit = sqlite3_open("main.db", &DB);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        cerr << "Error deleting song" << endl;
        sqlite3_free(messaggeError);
    }
    else 
        cout << "Deleted song successfully" << endl;
    sqlite3_close(DB);
    return 0;
}

int read_table(char* table) {
    sqlite3* DB;
    int exit = 0;
    string sql = "";
    sqlite3_close(DB);
    return 0;
}

int main() {
    int output = 0;
    // output = create_database();
    // if (output != 0) {
    //     return output;
    // }
    // output = create_table();
    // output = populate_songs_table(1, "Title", "Artist", false, 255);
    // output = delete_song(1);
    return output;
}
