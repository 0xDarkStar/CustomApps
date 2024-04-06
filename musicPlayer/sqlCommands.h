#include <iostream>
#include <sqlite3.h>
#include <typeinfo>

namespace sql {
    /* Callbacks for reading the table and its items
    (These comments outside will be for separating sections) */
    static int print_table_items(void* data, int argc, char** argv, char** azColName) {
        int i;
        fprintf(stderr, "%s: ", (const char*)data);

        for (i = 0; i < argc; i++) {
            printf("%s", argv[i] ? argv[i] : "NULL");
            if (i != argc-1) {
                printf(" | ");
            }
        }
        printf("\n");
        return 0;
    }

    extern "C" int return_table_length(void* data, int argc, char** argv, char** azColName) {
        /*
        This code is from
        https://stackoverflow.com/questions/20959338/storing-result-of-count-query-into-a-c-integer-in-sqlite
        */
        if (argc == 1 && argv) {
            *static_cast<int*>(data) = atoi(argv[0]);
            return 0;
        }
        return 1;
    }

    /* Making of all tables (should be a one-time thing) */
    int create_tables() { // FYI, all of this is from geeksforgeeks.org/sql-using-c-c-and-sqlite
        sqlite3* DB; // Of course it's modified a bit to work for me.
        std::string sql = "CREATE TABLE songs("
                    "ID INT NOT NULL, "
                    "Title TEXT NOT NULL, "
                    "Artist TEXT, "
                    "Has_subs INT NOT NULL, "
                    "Length INT NOT NULL,"
                    "PRIMARY KEY (ID) );"; // Make table command
        std::string sql1 = "CREATE TABLE playlists("
                    "ID INT NOT NULL, "
                    "Title TEXT NOT NULL, "
                    "Length INT NOT NULL, "
                    "Num_of_songs INT NOT NULL, "
                    "PRIMARY KEY (ID) );";
        std::string sql2 = "CREATE TABLE songSubs("
                    "Song_ID INT NOT NULL, "
                    "Sub_ID INT NOT NULL, "
                    "Language INT NOT NULL, "
                    "PRIMARY KEY (Song_ID) );";
        std::string sql3 = "CREATE TABLE songPlaylists("
                    "Song_ID INT NOT NULL, "
                    "Playlist_ID INT NOT NULL, "
                    "PRIMARY KEY (Song_ID) );";
        int exit = 0;
        sqlite3_open("main.db", &DB); // Open the database
        char* messaggeError;
        sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to make the tables
        sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messaggeError);
        sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messaggeError);
        exit = sqlite3_exec(DB, sql3.c_str(), NULL, 0, &messaggeError);
        if (exit != SQLITE_OK) { // Table wasn't made
            std::cerr << "Error create table" << std::endl;
            sqlite3_free(messaggeError);
        }
        else // Table was made
            std::cout << "Table created successfully" << std::endl;
        sqlite3_close(DB);
        return 0;
    }

    /* Messing with the contents of the tables (adding/removing values) */
    int add_song(int id, std::string title, std::string artist, bool has_subs, int length) { // length is in seconds
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO songs VALUES(" +
                    std::to_string(id) + ", '" + // Read max ID in songs then change this ID to +1 the max
                    title + "', '" +
                    artist + "', ";
        sql +=       (has_subs) ? "true":"false";
        sql +=       ", " + std::to_string(length) + ");"; // Add song to table command

        sqlite3_open("main.db", &DB);
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to add song
        if (exit != SQLITE_OK) {
            std::cerr << "Error adding values" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Values added successfully" << std::endl;
        sqlite3_close(DB);
        return 0;
    }

    int delete_song(int id) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "DELETE FROM songs WHERE ID = " + std::to_string(id) + ";"; // Delete song from table command
        sqlite3_open("main.db", &DB);
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

    int create_playlist(int id, std::string title, int length, int Num_of_songs) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO playlists VALUES(" + // Make playlist command
                    std::to_string(id) + ", '" +
                    title + "', " +
                    std::to_string(length) + ", " + 
                    std::to_string(Num_of_songs) + ");";
        sqlite3_open("main.db", &DB);
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to make playlist
        if (exit != SQLITE_OK) {
            std::cerr << "Error making playlist" << std::endl;
            sqlite3_free(messaggeError);
        }
        else 
            std::cout << "Made playlist successfully" << std::endl;
        sqlite3_close(DB);
        return 0;
    }

    int add_song_to_playlist(int song_id, int playlist_id) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO songPlaylists VALUES(" +
                     std::to_string(song_id) + ", " + 
                     std::to_string(playlist_id) + ");";
        sqlite3_open("main.db", &DB);
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to add song to playlist
        if (exit != SQLITE_OK) {
            std::cerr << "Error adding song to playlist" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Added song to playlist successfully" << std::endl;
        sqlite3_close(DB);
        return 0;
    }

    /* Looking at the table and its items */
    int read_table(std::string table) {
        sqlite3* DB;
        int exit = 0;
        std::string sql = "SELECT * FROM " + table + ";"; // To be continued...
        sqlite3_open("main.db", &DB);
        std::cout << "Items in " + table << std::endl;
        std::cout << "ID | Title | Artist | Has_subs | Length" << std::endl;
        sqlite3_exec(DB, sql.c_str(), print_table_items, NULL, NULL);
        sqlite3_close(DB);
        return 0;
    }

    int get_table_length(std::string table) {
        sqlite3* DB;
        int length;
        int exit;
        sqlite3_open("main.db", &DB);
        std::string sql = "SELECT COUNT(*) FROM " + table + ";";
        exit = sqlite3_exec(DB, sql.c_str(), return_table_length, &length, NULL);
        if (exit != SQLITE_OK) {
            std::cerr << "Error reading table" << std::endl;
        }
        else
            std::cout << "Length of " << table << " is " << length << std::endl;
        return 0;
    }
}