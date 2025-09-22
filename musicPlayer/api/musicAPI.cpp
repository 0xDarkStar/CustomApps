/*
Made By: 0xDarkStar
This file implements the main API interface for the music player backend.
*/
#include "musicAPI.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace MusicAPI {
    
    // Global state
    static bool g_initialized = false;
    static std::string g_lastError;
    
    // Helper function to set last error
    void setLastError(const std::string& error) {
        g_lastError = error;
    }
    
    // Helper function to clear last error
    void clearLastError() {
        g_lastError.clear();
    }
    
    // Helper function to get last error
    std::string getLastError() {
        return g_lastError;
    }
    
    // Initialize the API and database
    bool initialize() {
        try {
            clearLastError();
            
            if (!sql::initialize_database()) {
                setLastError("Failed to initialize database");
                return false;
            }
            
            g_initialized = true;
            return true;
            
        } catch (const sql::DatabaseSetupException& e) {
            setLastError("Database setup error: " + std::string(e.what()));
            return false;
        } catch (const std::exception& e) {
            setLastError("Unexpected error during initialization: " + std::string(e.what()));
            return false;
        }
    }
    
    // Check if the API is properly initialized
    bool isInitialized() {
        return g_initialized;
    }
    
    // Shutdown the API (cleanup)
    void shutdown() {
        g_initialized = false;
        clearLastError();
    }
    
    // Song Management API Implementation
    
    Song addSong(const std::string& title, const std::string& artist, int length, const std::string& path) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            int id = sql::add_song(title, artist, length, path);
            return Song(id, title, artist, length, path);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to add song: " + std::string(e.what()));
            throw APIException("Failed to add song: " + std::string(e.what()));
        }
    }
    
    bool deleteSong(int songId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            return sql::delete_song(songId);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to delete song: " + std::string(e.what()));
            throw APIException("Failed to delete song: " + std::string(e.what()));
        }
    }
    
    std::vector<Song> getAllSongs() {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::read_table("song");
            std::vector<Song> songs;
            
            for (const auto& row : results) {
                Song song;
                song.id = std::stoi(row.at("id"));
                song.title = row.at("title");
                song.artist = row.at("artist");
                song.length = std::stoi(row.at("length"));
                song.path = row.at("path");
                songs.push_back(song);
            }
            
            return songs;
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get songs: " + std::string(e.what()));
            throw APIException("Failed to get songs: " + std::string(e.what()));
        }
    }
    
    Song getSong(int songId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::read_table("song");
            
            for (const auto& row : results) {
                if (std::stoi(row.at("id")) == songId) {
                    return Song(
                        std::stoi(row.at("id")),
                        row.at("title"),
                        row.at("artist"),
                        std::stoi(row.at("length")),
                        row.at("path")
                    );
                }
            }
            
            throw APIException("Song with ID " + std::to_string(songId) + " not found");
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get song: " + std::string(e.what()));
            throw APIException("Failed to get song: " + std::string(e.what()));
        }
    }
    
    std::vector<Song> searchSongs(const std::string& query) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto allSongs = getAllSongs();
            std::vector<Song> results;
            
            std::string lowerQuery = query;
            std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
            
            for (const auto& song : allSongs) {
                std::string lowerTitle = song.title;
                std::string lowerArtist = song.artist;
                std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
                std::transform(lowerArtist.begin(), lowerArtist.end(), lowerArtist.begin(), ::tolower);
                
                if (lowerTitle.find(lowerQuery) != std::string::npos ||
                    lowerArtist.find(lowerQuery) != std::string::npos) {
                    results.push_back(song);
                }
            }
            
            return results;
            
        } catch (const std::exception& e) {
            setLastError("Failed to search songs: " + std::string(e.what()));
            throw APIException("Failed to search songs: " + std::string(e.what()));
        }
    }
    
    // Playlist Management API Implementation
    
    Playlist createPlaylist(const std::string& title) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            int id = sql::create_playlist(title);
            return Playlist(id, title, 0, 0);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to create playlist: " + std::string(e.what()));
            throw APIException("Failed to create playlist: " + std::string(e.what()));
        }
    }
    
    bool deletePlaylist(int playlistId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            // Note: This would need to be implemented in sqlCommands.h
            // For now, we'll throw an exception
            throw APIException("Delete playlist not yet implemented");
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to delete playlist: " + std::string(e.what()));
            throw APIException("Failed to delete playlist: " + std::string(e.what()));
        }
    }
    
    std::vector<Playlist> getAllPlaylists() {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::read_table("playlist");
            std::vector<Playlist> playlists;
            
            for (const auto& row : results) {
                Playlist playlist;
                playlist.id = std::stoi(row.at("id"));
                playlist.title = row.at("title");
                playlist.length = std::stoi(row.at("length"));
                playlist.numSongs = std::stoi(row.at("num_songs"));
                playlists.push_back(playlist);
            }
            
            return playlists;
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get playlists: " + std::string(e.what()));
            throw APIException("Failed to get playlists: " + std::string(e.what()));
        }
    }
    
    Playlist getPlaylist(int playlistId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::read_table("playlist");
            
            for (const auto& row : results) {
                if (std::stoi(row.at("id")) == playlistId) {
                    return Playlist(
                        std::stoi(row.at("id")),
                        row.at("title"),
                        std::stoi(row.at("length")),
                        std::stoi(row.at("num_songs"))
                    );
                }
            }
            
            throw APIException("Playlist with ID " + std::to_string(playlistId) + " not found");
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get playlist: " + std::string(e.what()));
            throw APIException("Failed to get playlist: " + std::string(e.what()));
        }
    }
    
    bool addSongToPlaylist(int songId, int playlistId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            return sql::add_song_to_playlist(songId, playlistId);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to add song to playlist: " + std::string(e.what()));
            throw APIException("Failed to add song to playlist: " + std::string(e.what()));
        }
    }
    
    bool removeSongFromPlaylist(int songId, int playlistId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            // Note: This would need to be implemented in sqlCommands.h
            // For now, we'll throw an exception
            throw APIException("Remove song from playlist not yet implemented");
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to remove song from playlist: " + std::string(e.what()));
            throw APIException("Failed to remove song from playlist: " + std::string(e.what()));
        }
    }
    
    std::vector<Song> getSongsInPlaylist(int playlistId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::get_songs_in_playlist(playlistId);
            std::vector<Song> songs;
            
            for (const auto& row : results) {
                Song song;
                song.id = std::stoi(row.at("id"));
                song.title = row.at("title");
                song.artist = row.at("artist");
                song.length = std::stoi(row.at("length"));
                song.path = row.at("path");
                songs.push_back(song);
            }
            
            return songs;
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get songs in playlist: " + std::string(e.what()));
            throw APIException("Failed to get songs in playlist: " + std::string(e.what()));
        }
    }
    
    // Subtitle Management API Implementation
    
    Subtitle addSubtitles(int songId, const std::string& language, const std::string& subtitlePath) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            int subId = sql::add_subtitles(songId, language, subtitlePath);
            return Subtitle(songId, subId, language);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to add subtitles: " + std::string(e.what()));
            throw APIException("Failed to add subtitles: " + std::string(e.what()));
        }
    }
    
    bool deleteSubtitles(int songId, int subId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            return sql::delete_subtitles(songId, subId);
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to delete subtitles: " + std::string(e.what()));
            throw APIException("Failed to delete subtitles: " + std::string(e.what()));
        }
    }
    
    std::vector<Subtitle> getSubtitlesForSong(int songId) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            auto results = sql::get_subtitles_for_song(songId);
            std::vector<Subtitle> subtitles;
            
            for (const auto& row : results) {
                Subtitle subtitle;
                subtitle.songId = std::stoi(row.at("song_id"));
                subtitle.subId = std::stoi(row.at("sub_id"));
                subtitle.language = row.at("language");
                subtitles.push_back(subtitle);
            }
            
            return subtitles;
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get subtitles for song: " + std::string(e.what()));
            throw APIException("Failed to get subtitles for song: " + std::string(e.what()));
        }
    }
    
    // Utility API Implementation
    
    DatabaseStats getDatabaseStats() {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        try {
            clearLastError();
            DatabaseStats stats;
            stats.totalSongs = sql::get_table_length("song");
            stats.totalPlaylists = sql::get_table_length("playlist");
            stats.totalSubtitles = sql::get_table_length("subtitle");
            stats.totalPlaylistSongs = sql::get_table_length("playlist_song");
            return stats;
            
        } catch (const sql::DatabaseException& e) {
            setLastError("Failed to get database stats: " + std::string(e.what()));
            throw APIException("Failed to get database stats: " + std::string(e.what()));
        }
    }
    
    // JSON Serialization helpers (basic implementation)
    std::string songToJson(const Song& song) {
        std::ostringstream oss;
        oss << "{"
            << "\"id\":" << song.id << ","
            << "\"title\":\"" << song.title << "\","
            << "\"artist\":\"" << song.artist << "\","
            << "\"length\":" << song.length << ","
            << "\"path\":\"" << song.path << "\""
            << "}";
        return oss.str();
    }
    
    std::string playlistToJson(const Playlist& playlist) {
        std::ostringstream oss;
        oss << "{"
            << "\"id\":" << playlist.id << ","
            << "\"title\":\"" << playlist.title << "\","
            << "\"length\":" << playlist.length << ","
            << "\"numSongs\":" << playlist.numSongs
            << "}";
        return oss.str();
    }
    
    std::string subtitleToJson(const Subtitle& subtitle) {
        std::ostringstream oss;
        oss << "{"
            << "\"songId\":" << subtitle.songId << ","
            << "\"subId\":" << subtitle.subId << ","
            << "\"language\":\"" << subtitle.language << "\""
            << "}";
        return oss.str();
    }
    
    std::string databaseStatsToJson(const DatabaseStats& stats) {
        std::ostringstream oss;
        oss << "{"
            << "\"totalSongs\":" << stats.totalSongs << ","
            << "\"totalPlaylists\":" << stats.totalPlaylists << ","
            << "\"totalSubtitles\":" << stats.totalSubtitles << ","
            << "\"totalPlaylistSongs\":" << stats.totalPlaylistSongs
            << "}";
        return oss.str();
    }
    
    // Batch operations for better performance
    std::vector<Song> addMultipleSongs(const std::vector<std::map<std::string, std::string>>& songData) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        std::vector<Song> songs;
        for (const auto& data : songData) {
            try {
                Song song = addSong(
                    data.at("title"),
                    data.at("artist"),
                    std::stoi(data.at("length")),
                    data.at("path")
                );
                songs.push_back(song);
            } catch (const std::exception& e) {
                // Log error but continue with other songs
                setLastError("Failed to add song: " + std::string(e.what()));
            }
        }
        return songs;
    }
    
    bool addMultipleSongsToPlaylist(int playlistId, const std::vector<int>& songIds) {
        if (!g_initialized) {
            throw APIException("API not initialized. Call initialize() first.");
        }
        
        bool allSuccess = true;
        for (int songId : songIds) {
            try {
                if (!addSongToPlaylist(songId, playlistId)) {
                    allSuccess = false;
                }
            } catch (const std::exception& e) {
                setLastError("Failed to add song " + std::to_string(songId) + " to playlist: " + std::string(e.what()));
                allSuccess = false;
            }
        }
        return allSuccess;
    }
    
} // namespace MusicAPI
