/*
Made By: 0xDarkStar
This file will hold all the functions that are used to set up the database.
*/
#include <iostream>
#include <sqlite3.h>
#include <typeinfo>

namespace sql {

    /* Make of all tables (should be a one-time thing) */
    int create_tables() { // FYI, all of this is from geeksforgeeks.org/sql-using-c-c-and-sqlite
        sqlite3* DB; // Of course it's modified a bit to work for me.
        char* messaggeError;
        int exit = 0;
        std::string foreignKeys = "PRAGMA foreign_keys = ON;";
        std::string sql = "CREATE TABLE song("
                    "id INT NOT NULL UNIQUE, "
                    "title TEXT NOT NULL, "
                    "artist TEXT, "
                    "length INT NOT NULL,"
                    "path TEXT NOT NULL,"
                    "PRIMARY KEY (id) );"; // Make table command
        std::string sql1 = "CREATE TABLE playlist("
                    "id INT NOT NULL UNIQUE, "
                    "title TEXT NOT NULL, "
                    "length INT NOT NULL, "
                    "img_path TEXT, "
                    "PRIMARY KEY (id) );";
        std::string sql2 = "CREATE TABLE subtitle("
                    "song_id INT, "
                    "sub_id INT NOT NULL UNIQUE, "
                    "language TEXT NOT NULL, "
                    "FOREIGN KEY(song_id) REFERENCES song(id) ON DELETE CASCADE, "
                    "PRIMARY KEY (song_id, sub_id) );";
        std::string sql3 = "CREATE TABLE playlist_song("
                    "song_id INT, "
                    "playlist_id INT, "
                    "track_number INT NOT NULL, "
                    "FOREIGN KEY(song_id) REFERENCES song(id) ON DELETE CASCADE, "
                    "FOREIGN KEY(playlist_id) REFERENCES playlist(id) ON DELETE CASCADE, "
                    "PRIMARY KEY (song_id, playlist_id) );";

        sqlite3_open("../data/musicPlayer.db", &DB); // Open the database
        sqlite3_exec(DB, foreignKeys.c_str(), NULL, 0, &messaggeError); // Enable foreign keys
        sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); // Attempt to make the tables
        sqlite3_exec(DB, sql1.c_str(), NULL, 0, &messaggeError);
        sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messaggeError);
        exit = sqlite3_exec(DB, sql3.c_str(), NULL, 0, &messaggeError); // Check if tables were made
        if (exit != SQLITE_OK) { // Table wasn't made
            std::cerr << "Error creating table" << std::endl;
            sqlite3_free(messaggeError);
        } else { // Table was made
            std::cout << "Table created successfully" << std::endl;
        }
        sqlite3_close(DB);
        return 0;
    }
}