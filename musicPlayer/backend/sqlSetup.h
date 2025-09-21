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
        std::string sql = "CREATE TABLE songs("
                    "ID INT NOT NULL UNIQUE, "
                    "Title TEXT NOT NULL, "
                    "Artist TEXT, "
                    "Length INT NOT NULL,"
                    "Path TEXT NOT NULL,"
                    "PRIMARY KEY (ID) );"; // Make table command
        std::string sql1 = "CREATE TABLE playlists("
                    "ID INT NOT NULL UNIQUE, "
                    "Title TEXT NOT NULL, "
                    "Length INT NOT NULL, "
                    "Num_of_songs INT NOT NULL, "
                    "PRIMARY KEY (ID) );";
        std::string sql2 = "CREATE TABLE songSubs("
                    "Song_ID INT, "
                    "Sub_ID INT NOT NULL UNIQUE, "
                    "Language INT NOT NULL, "
                    "FOREIGN KEY(Song_ID) REFERENCES songs(ID) ON DELETE CASCADE, "
                    "PRIMARY KEY (Song_ID, Sub_ID) );";
        std::string sql3 = "CREATE TABLE songPlaylists("
                    "Song_ID INT, "
                    "Playlist_ID INT, "
                    "FOREIGN KEY(Song_ID) REFERENCES songs(ID) ON DELETE CASCADE, "
                    "FOREIGN KEY(Playlist_ID) REFERENCES playlists(ID) ON DELETE CASCADE, "
                    "PRIMARY KEY (Song_ID, Playlist_ID) );";

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