/*
Made By: 0xDarkStar
This is a simple music player that uses SQLite3 to keep track of the songs, playlists, and subtitles.
This file specifically tests the sqlCommands.h file.
*/
#include "../../core/sqlSetup.h"
#include "../../core/sqlCommands.h"
#include <iostream>
using namespace std;

int main() {
    try {
        // Initialize database
        cout << "Initializing database..." << endl;
        if (!sql::initialize_database()) {
            cerr << "Failed to initialize database" << endl;
            return 1;
        }
        
        // Test adding songs
        cout << "\n=== Testing Song Operations ===" << endl;
        int songId1 = sql::add_song("Test Song 1", "Test Artist", 180, "/path/to/song1.mp3");
        int songId2 = sql::add_song("Test Song 2", "Another Artist", 240, "/path/to/song2.mp4");
        
        // Test creating playlist
        cout << "\n=== Testing Playlist Operations ===" << endl;
        int playlistId = sql::create_playlist("Test Playlist");
        
        // Test adding songs to playlist
        sql::add_song_to_playlist(songId1, playlistId);
        sql::add_song_to_playlist(songId2, playlistId);
        
        // Test adding subtitles
        cout << "\n=== Testing Subtitle Operations ===" << endl;
        try {
            int subId1 = sql::add_subtitles(songId1, "en", "/path/to/subtitles1.vtt");
            cout << "Added subtitle with ID: " << subId1 << endl;
        } catch (const sql::DatabaseException& e) {
            cout << "Error adding first subtitle: " << e.what() << endl;
        }
        
        try {
            int subId2 = sql::add_subtitles(songId1, "fr", "/path/to/subtitles1_fr.vtt");
            cout << "Added subtitle with ID: " << subId2 << endl;
        } catch (const sql::DatabaseException& e) {
            cout << "Error adding second subtitle: " << e.what() << endl;
        }
        
        // Test reading data
        cout << "\n=== Testing Read Operations ===" << endl;
        cout << "Songs in database:" << endl;
        sql::print_table("song");
        
        cout << "\nPlaylists in database:" << endl;
        sql::print_table("playlist");
        
        cout << "\nSubtitles in database:" << endl;
        sql::print_table("subtitle");
        
        cout << "\nPlaylist-song relationships:" << endl;
        sql::print_table("playlist_song");
        
        // Test getting songs in playlist
        cout << "\n=== Testing Advanced Queries ===" << endl;
        auto songsInPlaylist = sql::get_songs_in_playlist(playlistId);
        cout << "Songs in playlist " << playlistId << ":" << endl;
        for (const auto& song : songsInPlaylist) {
            cout << "  " << song.at("title") << " by " << song.at("artist") << endl;
        }
        
        // Test getting subtitles for song
        auto subtitles = sql::get_subtitles_for_song(songId1);
        cout << "\nSubtitles for song " << songId1 << ":" << endl;
        for (const auto& sub : subtitles) {
            cout << "  Language: " << sub.at("language") << " (ID: " << sub.at("sub_id") << ")" << endl;
        }
        
        // Test table lengths
        cout << "\n=== Testing Table Lengths ===" << endl;
        cout << "Number of songs: " << sql::get_table_length("song") << endl;
        cout << "Number of playlists: " << sql::get_table_length("playlist") << endl;
        cout << "Number of subtitles: " << sql::get_table_length("subtitle") << endl;
        cout << "Number of playlist-song relationships: " << sql::get_table_length("playlist_song") << endl;
        
        // Test reading specific items
        cout << "\n=== Testing Specific Item Reads ===" << endl;
        cout << "Song " << songId1 << " title: " << sql::read_table_item("song", songId1, "title") << endl;
        cout << "Song " << songId1 << " artist: " << sql::read_table_item("song", songId1, "artist") << endl;
        
        // Test error handling
        cout << "\n=== Testing Error Handling ===" << endl;
        try {
            sql::add_song("", "Artist", 180, "/path/to/song.mp3"); // Empty title should fail
        } catch (const sql::DatabaseException& e) {
            cout << "Caught expected error: " << e.what() << endl;
        }
        
        try {
            sql::delete_song(999); // Non-existent song should fail
        } catch (const sql::DatabaseException& e) {
            cout << "Caught expected error: " << e.what() << endl;
        }
        
        cout << "\n=== All tests completed successfully! ===" << endl;
        return 0;
        
    } catch (const sql::DatabaseException& e) {
        cerr << "Database error: " << e.what() << endl;
        return 1;
    } catch (const sql::DatabaseSetupException& e) {
        cerr << "Database setup error: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
        return 1;
    }
}
