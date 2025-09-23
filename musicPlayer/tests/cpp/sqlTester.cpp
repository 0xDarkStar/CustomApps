/*
Test file for SQLite database functionality.
Tests all database operations including songs, playlists, and subtitles.
*/
#include <iostream>
#include <string>
#include "../core/sqlCommands.h"
#include "../core/sqlSetup.h"

using namespace std;

void testDatabaseInitialization() {
    cout << "=== Testing Database Initialization ===" << endl;
    
    try {
        bool result = sql::initialize_database();
        if (result) {
            cout << "✓ Database initialized successfully" << endl;
        } else {
            cout << "✗ Database initialization failed" << endl;
        }
    } catch (const sql::DatabaseSetupException& e) {
        cout << "✗ Database setup error: " << e.what() << endl;
    }
}

void testSongOperations() {
    cout << "\n=== Testing Song Operations ===" << endl;
    
    try {
        // Test adding a song
        cout << "Testing add_song..." << endl;
        int songId = sql::add_song("Test Song", "Test Artist", "Test Album", 180, "/path/to/test.mp3");
        cout << "✓ Song added with ID: " << songId << endl;
        
        // Test reading songs
        cout << "Testing read_table for songs..." << endl;
        auto songs = sql::read_table("song");
        cout << "✓ Found " << songs.size() << " songs" << endl;
        
        // Test getting table length
        cout << "Testing get_table_length..." << endl;
        int length = sql::get_table_length("song");
        cout << "✓ Song table length: " << length << endl;
        
        // Test deleting the song
        cout << "Testing delete_song..." << endl;
        bool deleted = sql::delete_song(songId);
        if (deleted) {
            cout << "✓ Song deleted successfully" << endl;
        } else {
            cout << "✗ Failed to delete song" << endl;
        }
        
    } catch (const sql::DatabaseException& e) {
        cout << "✗ Database error: " << e.what() << endl;
    }
}

void testPlaylistOperations() {
    cout << "\n=== Testing Playlist Operations ===" << endl;
    
    try {
        // Test creating a playlist
        cout << "Testing create_playlist..." << endl;
        int playlistId = sql::create_playlist("Test Playlist");
        cout << "✓ Playlist created with ID: " << playlistId << endl;
        
        // Test reading playlists
        cout << "Testing read_table for playlists..." << endl;
        auto playlists = sql::read_table("playlist");
        cout << "✓ Found " << playlists.size() << " playlists" << endl;
        
        // Clean up
        cout << "Cleaning up playlist..." << endl;
        // Note: We would need a delete_playlist function for complete cleanup
        
    } catch (const sql::DatabaseException& e) {
        cout << "✗ Database error: " << e.what() << endl;
    }
}

void testSubtitleOperations() {
    cout << "\n=== Testing Subtitle Operations ===" << endl;
    
    try {
        // First add a song to attach subtitles to
        cout << "Adding test song for subtitle testing..." << endl;
        int songId = sql::add_song("Subtitle Test Song", "Test Artist", "Test Album", 200, "/path/to/subtitle_test.mp3");
        
        // Test adding subtitles
        cout << "Testing add_subtitles..." << endl;
        int subId = sql::add_subtitles(songId, "English", "/path/to/subtitles.srt");
        cout << "✓ Subtitle added with ID: " << subId << endl;
        
        // Test reading subtitles
        cout << "Testing read_table for subtitles..." << endl;
        auto subtitles = sql::read_table("subtitle");
        cout << "✓ Found " << subtitles.size() << " subtitles" << endl;
        
        // Test deleting subtitles
        cout << "Testing delete_subtitles..." << endl;
        bool deleted = sql::delete_subtitles(songId, subId);
        if (deleted) {
            cout << "✓ Subtitle deleted successfully" << endl;
        } else {
            cout << "✗ Failed to delete subtitle" << endl;
        }
        
        // Clean up song
        sql::delete_song(songId);
        
    } catch (const sql::DatabaseException& e) {
        cout << "✗ Database error: " << e.what() << endl;
    }
}

void testErrorHandling() {
    cout << "\n=== Testing Error Handling ===" << endl;
    
    try {
        // Test invalid song ID
        cout << "Testing invalid song ID..." << endl;
        sql::delete_song(-1);
        cout << "✗ Should have thrown exception for invalid ID" << endl;
    } catch (const sql::DatabaseException& e) {
        cout << "✓ Correctly caught invalid ID error: " << e.what() << endl;
    }
    
    try {
        // Test empty title
        cout << "Testing empty song title..." << endl;
        sql::add_song("", "Artist", "Album", 180, "/path/to/test.mp3");
        cout << "✗ Should have thrown exception for empty title" << endl;
    } catch (const sql::DatabaseException& e) {
        cout << "✓ Correctly caught empty title error: " << e.what() << endl;
    }
}

int main() {
    cout << "=== SQLite Database Tester ===" << endl;
    cout << "Testing all database operations..." << endl;
    
    testDatabaseInitialization();
    testSongOperations();
    testPlaylistOperations();
    testSubtitleOperations();
    testErrorHandling();
    
    cout << "\n=== Test Complete ===" << endl;
    cout << "All database tests completed!" << endl;
    
    return 0;
}
