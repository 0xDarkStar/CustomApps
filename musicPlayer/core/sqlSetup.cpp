/*
Implementation of database initialization and setup functions.
*/
#include "sqlSetup.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

namespace sql {
    
    // Define the database path
    const std::string dbpath = "../data/musicPlayer.db"; // It was making the database in frontend
    
    // Check if database file exists
    bool database_exists() {
        std::ifstream file(dbpath);
        return file.good();
    }
    
    // Create all necessary tables
    bool create_tables() {
        sqlite3* DB;
        
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseSetupException("Failed to open database for table creation");
        }
        
        // Enable foreign keys
        sqlite3_exec(DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        
        // Create songs table
        const char* createSongsTable = R"(
            CREATE TABLE IF NOT EXISTS song (
                id INTEGER PRIMARY KEY,
                title TEXT NOT NULL,
                artist TEXT NOT NULL,
                album TEXT DEFAULT '',
                length INTEGER NOT NULL CHECK(length > 0),
                path TEXT NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";
        
        // Create playlists table
        const char* createPlaylistsTable = R"(
            CREATE TABLE IF NOT EXISTS playlist (
                id INTEGER PRIMARY KEY,
                title TEXT NOT NULL,
                length INTEGER DEFAULT 0 CHECK(length >= 0),
                num_songs INTEGER DEFAULT 0 CHECK(num_songs >= 0),
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";
        
        // Create subtitles table
        const char* createSubtitlesTable = R"(
            CREATE TABLE IF NOT EXISTS subtitle (
                song_id INTEGER NOT NULL,
                sub_id INTEGER NOT NULL,
                language TEXT NOT NULL,
                path TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                PRIMARY KEY (song_id, sub_id),
                FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE
            );
        )";
        
        // Create playlist_song junction table
        const char* createPlaylistSongTable = R"(
            CREATE TABLE IF NOT EXISTS playlist_song (
                song_id INTEGER NOT NULL,
                playlist_id INTEGER NOT NULL,
                track_number INTEGER,
                added_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                PRIMARY KEY (song_id, playlist_id),
                FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE,
                FOREIGN KEY (playlist_id) REFERENCES playlist(id) ON DELETE CASCADE
            );
        )";
        
        // Create database version table
        const char* createVersionTable = R"(
            CREATE TABLE IF NOT EXISTS db_version (
                version INTEGER PRIMARY KEY,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";
        
        // Create indexes for better performance
        const char* createIndexes = R"(
            CREATE INDEX IF NOT EXISTS idx_song_title ON song(title);
            CREATE INDEX IF NOT EXISTS idx_song_artist ON song(artist);
            CREATE INDEX IF NOT EXISTS idx_song_album ON song(album);
            CREATE INDEX IF NOT EXISTS idx_playlist_title ON playlist(title);
            CREATE INDEX IF NOT EXISTS idx_subtitle_song_id ON subtitle(song_id);
            CREATE INDEX IF NOT EXISTS idx_playlist_song_playlist_id ON playlist_song(playlist_id);
            CREATE INDEX IF NOT EXISTS idx_playlist_song_song_id ON playlist_song(song_id);
        )";
        
        char* errorMsg = nullptr;
        
        // Execute table creation statements
        if (sqlite3_exec(DB, createSongsTable, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create songs table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        if (sqlite3_exec(DB, createPlaylistsTable, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create playlists table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        if (sqlite3_exec(DB, createSubtitlesTable, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create subtitles table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        if (sqlite3_exec(DB, createPlaylistSongTable, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create playlist_song table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        if (sqlite3_exec(DB, createVersionTable, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create version table: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        // Execute index creation statements
        if (sqlite3_exec(DB, createIndexes, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
            std::string error = "Failed to create indexes: ";
            error += errorMsg ? errorMsg : "Unknown error";
            sqlite3_free(errorMsg);
            sqlite3_close(DB);
            throw DatabaseSetupException(error);
        }
        
        sqlite3_close(DB);
        return true;
    }
    
    // Get database version
    int get_database_version() {
        sqlite3* DB;
        int version = 0;
        
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            return 0; // Return 0 if database doesn't exist
        }
        
        const char* sql = "SELECT MAX(version) FROM db_version;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                version = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(DB);
        return version;
    }
    
    // Set database version
    bool set_database_version(int version) {
        sqlite3* DB;
        
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseSetupException("Failed to open database for version update");
        }
        
        const char* sql = "INSERT INTO db_version (version) VALUES (?);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(DB);
            throw DatabaseSetupException("Failed to prepare version statement");
        }
        
        sqlite3_bind_int(stmt, 1, version);
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        
        return result == SQLITE_DONE;
    }
    
    // Run database migrations
    bool run_migrations() {
        sqlite3* DB;
        
        if (sqlite3_open(dbpath.c_str(), &DB) != SQLITE_OK) {
            throw DatabaseSetupException("Failed to open database for migrations");
        }
        
        int currentVersion = get_database_version();
        
        // Migration 1: Add album column to song table
        if (currentVersion < 2) {
            const char* addAlbumColumn = "ALTER TABLE song ADD COLUMN album TEXT DEFAULT '';";
            char* errorMsg = nullptr;
            
            if (sqlite3_exec(DB, addAlbumColumn, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
                // Check if column already exists (SQLite doesn't support IF NOT EXISTS for ALTER TABLE)
                if (errorMsg && strstr(errorMsg, "duplicate column name") == nullptr) {
                    std::string error = "Failed to add album column: ";
                    error += errorMsg;
                    sqlite3_free(errorMsg);
                    sqlite3_close(DB);
                    throw DatabaseSetupException(error);
                }
                sqlite3_free(errorMsg);
            }
            
            set_database_version(2);
            std::cout << "Applied migration: Added album column to song table" << std::endl;
        }
        
        sqlite3_close(DB);
        return true;
    }
    
    // Initialize the database
    bool initialize_database() {
        try {
            // Create data directory if it doesn't exist
            std::string dataDir = dbpath.substr(0, dbpath.find_last_of('/'));
            struct stat st;
            if (stat(dataDir.c_str(), &st) != 0) {
                // Directory doesn't exist, create it
                std::string mkdirCmd = "mkdir -p " + dataDir;
                system(mkdirCmd.c_str());
            }
            
            // Create tables
            if (!create_tables()) {
                return false;
            }
            
        // Set initial version if not set
        if (get_database_version() == 0) {
            set_database_version(1);
        }
        
        // Run migrations
        run_migrations();
            
            std::cout << "Database initialized successfully" << std::endl;
            return true;
            
        } catch (const DatabaseSetupException& e) {
            std::cerr << "Database setup error: " << e.what() << std::endl;
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error during database initialization: " << e.what() << std::endl;
            return false;
        }
    }
    
} // namespace sql
