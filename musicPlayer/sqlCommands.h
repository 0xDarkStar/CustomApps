/*
Made By: 0xDarkStar
This file contains all the functions for the SQLite3 database.
Said functions are:
 - add_song()/delete_song(): Adds/deletes a song from the database
 - add_subtitles(): Adds a subtitle to a song
 - create_playlist(): Creates a playlist
 - add_song_to_playlist(): Adds a song to a playlist
 - read_table(): Reads the contents of a table
 - read_table_item(): Reads the contents of a specific item in a table
 - get_table_length(): Gets the length of a table
 - get_song_length(): Gets the length of a song
*/
#include <iostream>
#include <sqlite3.h>
#include <typeinfo>

namespace sql {
    /* Callbacks for reading the table and its items
    (These comments outside will be for separating sections) */
    static int print_table_items(void* data, int argc, char** argv, char** azColName) {
        int i;

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

    /* Modifying songs */
    int add_song(int id, std::string title, std::string artist, int length, std::string path) { // length is in seconds
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO songs VALUES(" + // Add song to table command
                    std::to_string(id) + ", '" + // Read max ID in songs then change this ID to +1 the max
                    title + "', '" +
                    artist + "', " +
                    std::to_string(length) + ", '" +
                    path + "');";

        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to add song
        if (exit != SQLITE_OK) {
            std::cerr << "Error adding song" << std::endl;
            sqlite3_free(messaggeError);
        } else {
            std::cout << "Song added successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    int delete_song(int id) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "DELETE FROM songs WHERE ID = " + std::to_string(id) + ";"; // Delete song from table command
        
        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to delete song
        if (exit != SQLITE_OK) {
            std::cerr << "Error deleting song" << std::endl;
            sqlite3_free(messaggeError);
        } else {
            std::cout << "Deleted song successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    /* Modifying subtitles */
    int add_subtitles(int song_id, int sub_id, char language[3]) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO songSubs VALUES(" + // Make sub connection command
                     std::to_string(song_id) + ", " +
                     std::to_string(sub_id) + ", '";
        sql +=       language;
        sql +=       "');";

        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to connect sub to song
        if (exit != SQLITE_OK) {
            std::cerr << "Error adding subtitle connection" << std::endl;
            sqlite3_free(messaggeError);
        } else {
            std::cout << "Added subtitle connection successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    int delete_subtitles(int song_id, int sub_id) {
        sqlite3* DB;
        char* messageError;
        int exit = 0;
        std::string sql = "DELETE FROM songSubs WHERE Song_ID = " + std::to_string(song_id) + " AND Sub_ID = " + std::to_string(sub_id) + ";";
        
        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messageError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError); // Attempt to delete subtitle connection
        if (exit != SQLITE_OK) {
            std::cerr << "Error deleting subtitle connection" << std::endl;
            sqlite3_free(messageError);
        } else {
            std::cout << "Deleted subtitle connection successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    /* Modifying playlists */
    int create_playlist(int id, std::string title, int length, int Num_of_songs) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO playlists VALUES(" + // Make playlist command
                    std::to_string(id) + ", '" +
                    title + "', " +
                    std::to_string(length) + ", " + 
                    std::to_string(Num_of_songs) + ");";

        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to make playlist
        if (exit != SQLITE_OK) {
            std::cerr << "Error making playlist" << std::endl;
            sqlite3_free(messaggeError);
        } else {
            std::cout << "Made playlist successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    int add_song_to_playlist(int song_id, int playlist_id) {
        sqlite3* DB;
        char* messaggeError;
        int exit = 0;
        std::string sql = "INSERT INTO songPlaylists VALUES(" + // Make playlist connection command
                     std::to_string(song_id) + ", " + 
                     std::to_string(playlist_id) + ");";

        sqlite3_open("musicPlayer.db", &DB);
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to add song to playlist
        if (exit != SQLITE_OK) {
            std::cerr << "Error adding song to playlist" << std::endl;
            sqlite3_free(messaggeError);
        } else{
            std::cout << "Added song to playlist successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }

    /* Reading a table and its items */
    int read_table(std::string table) {
        sqlite3* DB;
        int exit = 0;
        std::string sql = "SELECT * FROM " + table + ";"; // Make read command
        sqlite3_open("musicPlayer.db", &DB);
        std::cout << "Items in " + table + ":" << std::endl;
        // Print the proper layout for the table
        if (table == "songs") {
                std::cout << "ID | Title | Artist | Length | Path" << std::endl;
        } else if (table == "songSubs") {
                std::cout << "Song_ID | Sub_ID | Language" << std::endl;
        } else if (table == "playlists") {
                std::cout << "ID | Title | Length | Num_of_songs" << std::endl;
        } else if (table == "songPlaylists") {
                std::cout << "Song_ID | Playlist_ID" << std::endl;
        }
        sqlite3_exec(DB, sql.c_str(), print_table_items, NULL, NULL); // Attempt to read the table
        sqlite3_close(DB);
        return 0;
    }

    int get_table_length(std::string table) {
        sqlite3* DB;
        int length;
        int exit;
        std::string sql = "SELECT COUNT(*) FROM " + table + ";"; // Make command to get length of table

        sqlite3_open("musicPlayer.db", &DB);
        exit = sqlite3_exec(DB, sql.c_str(), return_table_length, &length, NULL); // Attempt to get length of table
        if (exit != SQLITE_OK) {
            std::cerr << "Error reading table" << std::endl;
        } else {
            std::cout << "Length of " << table << " is " << length << std::endl;
        }
        return 0;
    }

    int read_table_item(std::string table, int ID, std::string column) {
        sqlite3* DB;
        int exit = 0;
        /* Read the table to check what the value of a specific column is. Ex:
        Does song 15 have subtitles?
        What languages are there for the subtitles of song 15?
        What playlists (if any) is song 15 in?*/
        // What to do?
        // Find out how to make this work...
        return 0;
    }
}