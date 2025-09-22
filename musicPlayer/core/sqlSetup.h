/*
Made By: 0xDarkStar
This file will hold all the functions that are used to set up the database.
*/
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

namespace sql {
    
    // Custom exception class for database setup errors
    class DatabaseSetupException : public std::runtime_error {
    public:
        DatabaseSetupException(const std::string& message) : std::runtime_error(message) {}
    };
    
    // Check if database file exists and is accessible
    bool database_exists() {
        struct stat buffer;
        return (stat("../../data/musicPlayer.db", &buffer) == 0 && S_ISREG(buffer.st_mode));
    }
    
    // Create data directory if it doesn't exist
    void ensure_data_directory() {
        struct stat buffer;
        if (stat("../../data", &buffer) != 0) {
            // Directory doesn't exist, create it
            if (mkdir("../../data", 0755) != 0) {
                throw DatabaseSetupException("Failed to create data directory: ../../data");
            }
            std::cout << "Created data directory: ../../data" << std::endl;
        } else if (!S_ISDIR(buffer.st_mode)) {
            throw DatabaseSetupException("../../data exists but is not a directory");
        }
    }
    
    // Check if tables exist
    bool tables_exist() {
        if (!database_exists()) {
            return false;
        }
        
        sqlite3* DB;
        if (sqlite3_open("../../data/musicPlayer.db", &DB) != SQLITE_OK) {
            return false;
        }
        
        // Check if all required tables exist
        const char* checkSql = "SELECT name FROM sqlite_master WHERE type='table' AND name IN ('song', 'playlist', 'subtitle', 'playlist_song');";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, checkSql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            return false;
        }
        
        int tableCount = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            tableCount++;
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        return tableCount == 4; // All 4 tables should exist
    }

    /* Create all tables (should be a one-time thing) */
    bool create_tables() {
        try {
            // Ensure data directory exists
            ensure_data_directory();
            
            sqlite3* DB;
            if (sqlite3_open("../../data/musicPlayer.db", &DB) != SQLITE_OK) {
                throw DatabaseSetupException("Failed to open database");
            }
            
            // Enable foreign keys
            char* errorMsg = nullptr;
            if (sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errorMsg) != SQLITE_OK) {
                std::string error = "Failed to enable foreign keys: ";
                error += errorMsg ? errorMsg : "Unknown error";
                sqlite3_free(errorMsg);
                sqlite3_close(DB);
                throw DatabaseSetupException(error);
            }
            
            // Create tables with proper error handling
            std::vector<std::string> createStatements = {
                "CREATE TABLE IF NOT EXISTS song("
                "id INTEGER NOT NULL UNIQUE, "
                "title TEXT NOT NULL, "
                "artist TEXT, "
                "length INTEGER NOT NULL, "
                "path TEXT NOT NULL, "
                "PRIMARY KEY (id) );",
                
                "CREATE TABLE IF NOT EXISTS playlist("
                "id INTEGER NOT NULL UNIQUE, "
                "title TEXT NOT NULL, "
                "length INTEGER NOT NULL DEFAULT 0, "
                "num_songs INTEGER NOT NULL DEFAULT 0, "
                "PRIMARY KEY (id) );",
                
                "CREATE TABLE IF NOT EXISTS subtitle("
                "song_id INTEGER, "
                "sub_id INTEGER NOT NULL UNIQUE, "
                "language TEXT NOT NULL, "
                "FOREIGN KEY(song_id) REFERENCES song(id) ON DELETE CASCADE, "
                "PRIMARY KEY (song_id, sub_id) );",
                
                "CREATE TABLE IF NOT EXISTS playlist_song("
                "song_id INTEGER, "
                "playlist_id INTEGER, "
                "track_number INTEGER NOT NULL, "
                "FOREIGN KEY(song_id) REFERENCES song(id) ON DELETE CASCADE, "
                "FOREIGN KEY(playlist_id) REFERENCES playlist(id) ON DELETE CASCADE, "
                "PRIMARY KEY (song_id, playlist_id) );"
            };
            
            for (const auto& sql : createStatements) {
                if (sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
                    std::string error = "Failed to create table: ";
                    error += errorMsg ? errorMsg : "Unknown error";
                    sqlite3_free(errorMsg);
                    sqlite3_close(DB);
                    throw DatabaseSetupException(error);
                }
            }
            
            sqlite3_close(DB);
            std::cout << "Database tables created successfully" << std::endl;
            return true;
            
        } catch (const DatabaseSetupException& e) {
            std::cerr << "Database setup error: " << e.what() << std::endl;
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error during database setup: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Initialize database - create tables if they don't exist
    bool initialize_database() {
        if (tables_exist()) {
            std::cout << "Database tables already exist" << std::endl;
            return true;
        }
        
        std::cout << "Creating database tables..." << std::endl;
        return create_tables();
    }
    
    // Drop all tables (for testing/reset purposes)
    bool drop_tables() {
        try {
            sqlite3* DB;
            if (sqlite3_open("../../data/musicPlayer.db", &DB) != SQLITE_OK) {
                throw DatabaseSetupException("Failed to open database");
            }
            
            // Drop tables in reverse order due to foreign key constraints
            std::vector<std::string> dropStatements = {
                "DROP TABLE IF EXISTS playlist_song;",
                "DROP TABLE IF EXISTS subtitle;",
                "DROP TABLE IF EXISTS playlist;",
                "DROP TABLE IF EXISTS song;"
            };
            
            char* errorMsg = nullptr;
            for (const auto& sql : dropStatements) {
                if (sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
                    std::string error = "Failed to drop table: ";
                    error += errorMsg ? errorMsg : "Unknown error";
                    sqlite3_free(errorMsg);
                    sqlite3_close(DB);
                    throw DatabaseSetupException(error);
                }
            }
            
            sqlite3_close(DB);
            std::cout << "All tables dropped successfully" << std::endl;
            return true;
            
        } catch (const DatabaseSetupException& e) {
            std::cerr << "Error dropping tables: " << e.what() << std::endl;
            return false;
        }
    }
}