/*
Made By: notmacos
This file contains the main API interface for the music player backend.
It provides a clean C++ API that can be easily integrated with Electron.
*/
#ifndef MUSIC_API_H
#define MUSIC_API_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "../core/sqlCommands.h"
#include "../core/sqlSetup.h"

namespace MusicAPI {
    
    // Custom exception class for API errors
    class APIException : public std::runtime_error {
    public:
        APIException(const std::string& message) : std::runtime_error(message) {}
    };
    
    // Initialize the API and database
    bool initialize();
    
    // Song Management API
    struct Song {
        int id;
        std::string title;
        std::string artist;
        std::string album;
        int length; // in seconds
        std::string path;
        
        Song() : id(0), length(0) {}
        Song(int id, const std::string& title, const std::string& artist, const std::string& album, int length, const std::string& path)
            : id(id), title(title), artist(artist), album(album), length(length), path(path) {}
    };
    
    // Add a new song to the database
    Song addSong(const std::string& title, const std::string& artist, const std::string& album, int length, const std::string& path);
    
    // Delete a song from the database
    bool deleteSong(int songId);
    
    // Update song data (title, artist, album)
    Song updateSong(int songId, const std::string& title, const std::string& artist, const std::string& album);
    
    // Get all songs
    std::vector<Song> getAllSongs();
    
    // Get a specific song by ID
    Song getSong(int songId);
    
    // Search songs by title, artist, or album
    std::vector<Song> searchSongs(const std::string& query);
    
    // Playlist Management API
    struct Playlist {
        int id;
        std::string title;
        int length; // total length in seconds
        int numSongs;
        
        Playlist() : id(0), length(0), numSongs(0) {}
        Playlist(int id, const std::string& title, int length, int numSongs)
            : id(id), title(title), length(length), numSongs(numSongs) {}
    };
    
    // Create a new playlist
    Playlist createPlaylist(const std::string& title);
    
    // Delete a playlist
    bool deletePlaylist(int playlistId);
    
    // Update playlist title
    Playlist updatePlaylist(int playlistId, const std::string& title);
    
    // Get all playlists
    std::vector<Playlist> getAllPlaylists();
    
    // Get a specific playlist by ID
    Playlist getPlaylist(int playlistId);
    
    // Add a song to a playlist
    bool addSongToPlaylist(int songId, int playlistId);
    
    // Remove a song from a playlist
    bool removeSongFromPlaylist(int songId, int playlistId);
    
    // Get all songs in a playlist
    std::vector<Song> getSongsInPlaylist(int playlistId);
    
    // Subtitle Management API
    struct Subtitle {
        int songId;
        int subId;
        std::string language;
        
        Subtitle() : songId(0), subId(0) {}
        Subtitle(int songId, int subId, const std::string& language)
            : songId(songId), subId(subId), language(language) {}
    };
    
    // Add subtitles to a song
    Subtitle addSubtitles(int songId, const std::string& language, const std::string& subtitlePath);
    
    // Delete subtitles from a song
    bool deleteSubtitles(int songId, int subId);
    
    // Get all subtitles for a song
    std::vector<Subtitle> getSubtitlesForSong(int songId);
    
    // Utility API
    // Get database statistics
    struct DatabaseStats {
        int totalSongs;
        int totalPlaylists;
        int totalSubtitles;
        int totalPlaylistSongs;
        
        DatabaseStats() : totalSongs(0), totalPlaylists(0), totalSubtitles(0), totalPlaylistSongs(0) {}
    };
    
    DatabaseStats getDatabaseStats();
    
    // Check if the API is properly initialized
    bool isInitialized();
    
    // Shutdown the API (cleanup)
    void shutdown();
    
    // JSON Serialization helpers
    std::string songToJson(const Song& song);
    std::string playlistToJson(const Playlist& playlist);
    std::string subtitleToJson(const Subtitle& subtitle);
    std::string databaseStatsToJson(const DatabaseStats& stats);
    
    // JSON Deserialization helpers
    Song jsonToSong(const std::string& json);
    Playlist jsonToPlaylist(const std::string& json);
    Subtitle jsonToSubtitle(const std::string& json);
    
    // Batch operations for better performance
    std::vector<Song> addMultipleSongs(const std::vector<std::map<std::string, std::string>>& songData);
    bool addMultipleSongsToPlaylist(int playlistId, const std::vector<int>& songIds);
    
    // Error handling
    std::string getLastError();
    void clearLastError();
    
} // namespace MusicAPI

#endif // MUSIC_API_H
