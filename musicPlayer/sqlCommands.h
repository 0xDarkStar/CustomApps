#include <iostream>
#include <sqlite3.h>

int create_table() { // FYI, all of this is from geeksforgeeks.org/sql-using-c-c-and-sqlite
    sqlite3* DB; // Of course it's modified a bit to work for me.
    std::string sql = "CREATE TABLE Songs("
                 "ID             INT      NOT NULL, "
                 "Title          TEXT     NOT NULL, "
                 "Artist         TEXT, "
                 "Has_subs       INT      NOT NULL, "
                 "Length         INT      NOT NULL,"
                 "PRIMARY KEY (ID) );"; // Make table command
    int exit = 0;
    exit = sqlite3_open("main.db", &DB); // Open the database
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to make a table
    if (exit != SQLITE_OK) { // Table wasn't made
        std::cerr << "Error create table" << std::endl;
        sqlite3_free(messaggeError);
    }
    else // Table was made
        std::cout << "Table created successfully" << std::endl;
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

int populate_songs_table(int id, std::string title, std::string artist, bool has_subs, int length) { // length is in seconds
    sqlite3* DB;
    char* messaggeError;
    int exit = 0;
    std::string sql = "INSERT INTO Songs VALUES(" +
                 std::to_string(id) + ", '" +
                 title + "', '" +
                 artist + "', ";
    sql +=       (has_subs) ? "true":"false";
    sql +=       ", " + std::to_string(length) + ");"; // Add song to table command
    
    std::string query = "SELECT * FROM Songs;";
    std::cout << "State of table before insert" << std::endl;
    exit = sqlite3_open("main.db", &DB);
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL); // Check all songs
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to add song
    if (exit != SQLITE_OK) {
        std::cerr << "Error adding values" << std::endl;
        sqlite3_free(messaggeError);
    }
    else
        std::cout << "Values added successfully" << std::endl;
    std::cout << "State of table after insert" << std::endl;
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
    sqlite3_close(DB);
    return 0;
}

int delete_song(int id) {
    sqlite3* DB;
    char* messaggeError;
    int exit = 0;
    std::string sql = "DELETE FROM Songs WHERE ID = " + std::to_string(id) + ";"; // Delete song from table command
    exit = sqlite3_open("main.db", &DB);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to delete song
    if (exit != SQLITE_OK) {
        std::cerr << "Error deleting song" << std::endl;
        sqlite3_free(messaggeError);
    }
    else 
        std::cout << "Deleted song successfully" << std::endl;
    sqlite3_close(DB);
    return 0;
}

int read_table(char* table) {
    sqlite3* DB;
    int exit = 0;
    std::string sql = ""; // To be continued...
    sqlite3_close(DB);
    return 0;
}