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
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <algorithm>

namespace sql {
    const std::string dbpath = "../../data/musicPlayer.db";
    
    // Custom exception class for database errors
    class DatabaseException : public std::runtime_error {
    public:
        DatabaseException(const std::string& message) : std::runtime_error(message) {}
    };
    
    // Utility functions
    namespace utils {
        // Sanitize string to prevent SQL injection
        std::string sanitizeString(const std::string& input) {
            std::string result = input;
            // Replace single quotes with double quotes to escape them
            size_t pos = 0;
            while ((pos = result.find("'", pos)) != std::string::npos) {
                result.replace(pos, 1, "''");
                pos += 2;
            }
            return result;
        }
        
        // Validate file path
        bool isValidPath(const std::string& path) {
            return !path.empty() && path.length() < 1000; // Basic validation
        }
        
        // Get next available ID for a table
        int getNextId(const std::string& table) {
            sqlite3* DB;
            int nextId = 1;
            
            if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
                throw DatabaseException("Failed to open database for ID generation");
            }
            
            std::string sql = "SELECT COALESCE(MAX(id), 0) + 1 FROM " + table + ";";
            sqlite3_stmt* stmt;
            
            if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    nextId = sqlite3_column_int(stmt, 0);
                }
                sqlite3_finalize(stmt);
            }
            
            sqlite3_close(DB);
            return nextId;
        }
    }
    
    /* Callbacks for reading the table and its items
    (These comments outside will be for separating sections) */
    static int print_table_items(void* data, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
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
    
    // Callback to collect table data into a vector of maps
    static int collect_table_data(void* data, int argc, char** argv, char** azColName) {
        std::vector<std::map<std::string, std::string>>* rows = 
            static_cast<std::vector<std::map<std::string, std::string>>*>(data);
        
        std::map<std::string, std::string> row;
        for (int i = 0; i < argc; i++) {
            row[azColName[i]] = argv[i] ? argv[i] : "NULL";
        }
        rows->push_back(row);
        return 0;
    }

    /* Modifying songs */
    int add_song(const std::string& title, const std::string& artist, int length, const std::string& path) {
        // Input validation
        if (title.empty()) {
            throw DatabaseException("Song title cannot be empty");
        }
        if (length <= 0) {
            throw DatabaseException("Song length must be positive");
        }
        if (!utils::isValidPath(path)) {
            throw DatabaseException("Invalid file path");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Get next available ID
        int id = utils::getNextId("song");
        
        // Use prepared statement to prevent SQL injection
        const char* sql = "INSERT INTO song (id, title, artist, length, path) VALUES (?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        // Bind parameters
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, utils::sanitizeString(title).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, utils::sanitizeString(artist).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, length);
        sqlite3_bind_text(stmt, 5, utils::sanitizeString(path).c_str(), -1, SQLITE_STATIC);
        
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to add song to database");
        }
        
        std::cout << "Song added successfully with ID: " << id << std::endl;
        return id;
    }

    bool delete_song(int id) {
        if (id <= 0) {
            throw DatabaseException("Invalid song ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Check if song exists first
        const char* checkSql = "SELECT COUNT(*) FROM song WHERE id = ?;";
        sqlite3_stmt* checkStmt;
        
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, id);
        int exists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (exists == 0) {
            sqlite3_close(DB);
            throw DatabaseException("Song with ID " + std::to_string(id) + " does not exist");
        }
        
        // Delete the song
        const char* deleteSql = "DELETE FROM song WHERE id = ?;";
        sqlite3_stmt* deleteStmt;
        
        if (sqlite3_prepare_v2(DB, deleteSql, -1, &deleteStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare delete statement");
        }
        
        sqlite3_bind_int(deleteStmt, 1, id);
        int result = sqlite3_step(deleteStmt);
        sqlite3_finalize(deleteStmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to delete song from database");
        }
        
        std::cout << "Song with ID " << id << " deleted successfully" << std::endl;
        return true;
    }

    /* Modifying subtitles */
    int add_subtitles(int song_id, const std::string& language, const std::string& subtitle_path) {
        if (song_id <= 0) {
            throw DatabaseException("Invalid song ID");
        }
        if (language.empty()) {
            throw DatabaseException("Language cannot be empty");
        }
        if (!utils::isValidPath(subtitle_path)) {
            throw DatabaseException("Invalid subtitle file path");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Check if song exists
        const char* checkSql = "SELECT COUNT(*) FROM song WHERE id = ?;";
        sqlite3_stmt* checkStmt;
        
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, song_id);
        int exists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (exists == 0) {
            sqlite3_close(DB);
            throw DatabaseException("Song with ID " + std::to_string(song_id) + " does not exist");
        }
        
        // Get next available subtitle ID for this song
        const char* getSubIdSql = "SELECT COALESCE(MAX(sub_id), 0) + 1 FROM subtitle WHERE song_id = ?;";
        sqlite3_stmt* getSubIdStmt;
        
        if (sqlite3_prepare_v2(DB, getSubIdSql, -1, &getSubIdStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare sub_id statement");
        }
        
        sqlite3_bind_int(getSubIdStmt, 1, song_id);
        int sub_id = 1;
        
        if (sqlite3_step(getSubIdStmt) == SQLITE_ROW) {
            sub_id = sqlite3_column_int(getSubIdStmt, 0);
        }
        sqlite3_finalize(getSubIdStmt);
        
        // Add subtitle
        const char* sql = "INSERT INTO subtitle (song_id, sub_id, language) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        sqlite3_bind_int(stmt, 1, song_id);
        sqlite3_bind_int(stmt, 2, sub_id);
        sqlite3_bind_text(stmt, 3, utils::sanitizeString(language).c_str(), -1, SQLITE_STATIC);
        
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to add subtitle to database");
        }
        
        std::cout << "Subtitle added successfully with ID: " << sub_id << std::endl;
        return sub_id;
    }

    bool delete_subtitles(int song_id, int sub_id) {
        if (song_id <= 0 || sub_id <= 0) {
            throw DatabaseException("Invalid song ID or subtitle ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Check if subtitle exists
        const char* checkSql = "SELECT COUNT(*) FROM subtitle WHERE song_id = ? AND sub_id = ?;";
        sqlite3_stmt* checkStmt;
        
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, song_id);
        sqlite3_bind_int(checkStmt, 2, sub_id);
        int exists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (exists == 0) {
            sqlite3_close(DB);
            throw DatabaseException("Subtitle with song_id " + std::to_string(song_id) + 
                                  " and sub_id " + std::to_string(sub_id) + " does not exist");
        }
        
        // Delete the subtitle
        const char* deleteSql = "DELETE FROM subtitle WHERE song_id = ? AND sub_id = ?;";
        sqlite3_stmt* deleteStmt;
        
        if (sqlite3_prepare_v2(DB, deleteSql, -1, &deleteStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare delete statement");
        }
        
        sqlite3_bind_int(deleteStmt, 1, song_id);
        sqlite3_bind_int(deleteStmt, 2, sub_id);
        int result = sqlite3_step(deleteStmt);
        sqlite3_finalize(deleteStmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to delete subtitle from database");
        }
        
        std::cout << "Subtitle with ID " << sub_id << " deleted successfully" << std::endl;
        return true;
    }

    /* Modifying playlists */
    int create_playlist(const std::string& title) {
        if (title.empty()) {
            throw DatabaseException("Playlist title cannot be empty");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Get next available ID
        int id = utils::getNextId("playlist");
        
        // Create playlist with default values
        const char* sql = "INSERT INTO playlist (id, title, length, num_songs) VALUES (?, ?, 0, 0);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, utils::sanitizeString(title).c_str(), -1, SQLITE_STATIC);
        
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to create playlist in database");
        }
        
        std::cout << "Playlist created successfully with ID: " << id << std::endl;
        return id;
    }

    bool add_song_to_playlist(int song_id, int playlist_id) {
        if (song_id <= 0 || playlist_id <= 0) {
            throw DatabaseException("Invalid song ID or playlist ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Check if song and playlist exist
        const char* checkSql = "SELECT COUNT(*) FROM song WHERE id = ?;";
        sqlite3_stmt* checkStmt;
        
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, song_id);
        int songExists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            songExists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (songExists == 0) {
            sqlite3_close(DB);
            throw DatabaseException("Song with ID " + std::to_string(song_id) + " does not exist");
        }
        
        // Check playlist exists
        checkSql = "SELECT COUNT(*) FROM playlist WHERE id = ?;";
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare playlist check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, playlist_id);
        int playlistExists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            playlistExists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (playlistExists == 0) {
            sqlite3_close(DB);
            throw DatabaseException("Playlist with ID " + std::to_string(playlist_id) + " does not exist");
        }
        
        // Check if song is already in playlist
        checkSql = "SELECT COUNT(*) FROM playlist_song WHERE song_id = ? AND playlist_id = ?;";
        if (sqlite3_prepare_v2(DB, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare duplicate check statement");
        }
        
        sqlite3_bind_int(checkStmt, 1, song_id);
        sqlite3_bind_int(checkStmt, 2, playlist_id);
        int alreadyExists = 0;
        
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            alreadyExists = sqlite3_column_int(checkStmt, 0);
        }
        sqlite3_finalize(checkStmt);
        
        if (alreadyExists > 0) {
            sqlite3_close(DB);
            throw DatabaseException("Song is already in this playlist");
        }
        
        // Add song to playlist (no track_number in current schema)
        const char* sql = "INSERT INTO playlist_song (song_id, playlist_id) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare add statement");
        }
        
        sqlite3_bind_int(stmt, 1, song_id);
        sqlite3_bind_int(stmt, 2, playlist_id);
        
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        if (result != SQLITE_DONE) {
            throw DatabaseException("Failed to add song to playlist");
        }
        
        std::cout << "Song " << song_id << " added to playlist " << playlist_id << std::endl;
        return true;
    }

    /* Reading a table and its items */
    std::vector<std::map<std::string, std::string>> read_table(const std::string& table) {
        // Validate table name to prevent SQL injection
        std::vector<std::string> validTables = {"song", "subtitle", "playlist", "playlist_song"};
        if (std::find(validTables.begin(), validTables.end(), table) == validTables.end()) {
            throw DatabaseException("Invalid table name: " + table);
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        std::string sql = "SELECT * FROM " + table + ";";
        std::vector<std::map<std::string, std::string>> results;
        
        char* errorMsg = nullptr;
        int result = sqlite3_exec(DB, sql.c_str(), collect_table_data, &results, &errorMsg);
        
        if (result != SQLITE_OK) {
            std::string error = "Failed to read table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseException(error);
        }
        
        sqlite3_close(DB);
        return results;
    }
    
    // Legacy function for console output
    void print_table(const std::string& table) {
        try {
            auto results = read_table(table);
            std::cout << "Items in " << table << ":" << std::endl;
            
            // Print headers
            if (table == "song") {
                std::cout << "id | title | artist | length | path" << std::endl;
            } else if (table == "subtitle") {
                std::cout << "song_id | sub_id | language" << std::endl;
            } else if (table == "playlist") {
                std::cout << "id | title | length | num_songs" << std::endl;
            } else if (table == "playlist_song") {
                std::cout << "song_id | playlist_id" << std::endl;
            }
            
            // Print data
            for (const auto& row : results) {
                bool first = true;
                for (const auto& pair : row) {
                    if (!first) std::cout << " | ";
                    std::cout << pair.second;
                    first = false;
                }
                std::cout << std::endl;
            }
        } catch (const DatabaseException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    int get_table_length(const std::string& table) {
        // Validate table name
        std::vector<std::string> validTables = {"song", "subtitle", "playlist", "playlist_song"};
        if (std::find(validTables.begin(), validTables.end(), table) == validTables.end()) {
            throw DatabaseException("Invalid table name: " + table);
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        std::string sql = "SELECT COUNT(*) FROM " + table + ";";
        int length = 0;
        
        char* errorMsg = nullptr;
        int result = sqlite3_exec(DB, sql.c_str(), return_table_length, &length, &errorMsg);
        
        if (result != SQLITE_OK) {
            std::string error = "Failed to get table length: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseException(error);
        }
        
        sqlite3_close(DB);
        return length;
    }

    std::string read_table_item(const std::string& table, int id, const std::string& column) {
        // Validate table name
        std::vector<std::string> validTables = {"song", "subtitle", "playlist", "playlist_song"};
        if (std::find(validTables.begin(), validTables.end(), table) == validTables.end()) {
            throw DatabaseException("Invalid table name: " + table);
        }
        
        if (id <= 0) {
            throw DatabaseException("Invalid ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        std::string sql = "SELECT " + column + " FROM " + table + " WHERE id = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        sqlite3_bind_int(stmt, 1, id);
        std::string result;
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            result = value ? value : "NULL";
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(DB);
            throw DatabaseException("No record found with ID " + std::to_string(id));
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return result;
    }
    
    // Additional utility functions
    std::vector<std::map<std::string, std::string>> get_songs_in_playlist(int playlist_id) {
        if (playlist_id <= 0) {
            throw DatabaseException("Invalid playlist ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        std::string sql = "SELECT s.* FROM song s "
                         "JOIN playlist_song ps ON s.id = ps.song_id "
                         "WHERE ps.playlist_id = ?;";
        
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        sqlite3_bind_int(stmt, 1, playlist_id);
        std::vector<std::map<std::string, std::string>> results;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::map<std::string, std::string> row;
            int columnCount = sqlite3_column_count(stmt);
            
            for (int i = 0; i < columnCount; i++) {
                const char* columnName = sqlite3_column_name(stmt, i);
                const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                row[columnName] = value ? value : "NULL";
            }
            results.push_back(row);
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return results;
    }
    
    std::vector<std::map<std::string, std::string>> get_subtitles_for_song(int song_id) {
        if (song_id <= 0) {
            throw DatabaseException("Invalid song ID");
        }
        
        sqlite3* DB;
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseException("Failed to open database");
        }
        
        std::string sql = "SELECT * FROM subtitle WHERE song_id = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseException("Failed to prepare statement");
        }
        
        sqlite3_bind_int(stmt, 1, song_id);
        std::vector<std::map<std::string, std::string>> results;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::map<std::string, std::string> row;
            int columnCount = sqlite3_column_count(stmt);
            
            for (int i = 0; i < columnCount; i++) {
                const char* columnName = sqlite3_column_name(stmt, i);
                const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                row[columnName] = value ? value : "NULL";
            }
            results.push_back(row);
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return results;
    }
}