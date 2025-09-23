/*
Made By: 0xDarkStar
JavaScript wrapper for the native C++ music API.
*/
const musicApi = require('./build/Release/music_api');

class MusicAPI {
    constructor() {
        this.initialized = false;
    }

    // Initialize the API
    async initialize() {
        try {
            const result = musicApi.initialize();
            this.initialized = result;
            return result;
        } catch (error) {
            throw new Error(`Failed to initialize API: ${error.message}`);
        }
    }

    // Check if API is initialized
    isInitialized() {
        return musicApi.isInitialized();
    }

    // Shutdown the API
    shutdown() {
        musicApi.shutdown();
        this.initialized = false;
    }

    // Song Management
    async addSong(title, artist, album, length, path) {
        this._checkInitialized();
        try {
            return musicApi.addSong(title, artist, album, length, path);
        } catch (error) {
            throw new Error(`Failed to add song: ${error.message}`);
        }
    }

    async deleteSong(songId) {
        this._checkInitialized();
        try {
            return musicApi.deleteSong(songId);
        } catch (error) {
            throw new Error(`Failed to delete song: ${error.message}`);
        }
    }

    async updateSong(songId, title, artist, album) {
        this._checkInitialized();
        try {
            return musicApi.updateSong(songId, title, artist, album);
        } catch (error) {
            throw new Error(`Failed to update song: ${error.message}`);
        }
    }

    async getSong(songId) {
        this._checkInitialized();
        try {
            return musicApi.getSong(songId);
        } catch (error) {
            throw new Error(`Failed to get song: ${error.message}`);
        }
    }

    async getAllSongs() {
        this._checkInitialized();
        try {
            return musicApi.getAllSongs();
        } catch (error) {
            throw new Error(`Failed to get songs: ${error.message}`);
        }
    }

    async searchSongs(query) {
        this._checkInitialized();
        try {
            return musicApi.searchSongs(query);
        } catch (error) {
            throw new Error(`Failed to search songs: ${error.message}`);
        }
    }

    // Playlist Management
    async createPlaylist(title) {
        this._checkInitialized();
        try {
            return musicApi.createPlaylist(title);
        } catch (error) {
            throw new Error(`Failed to create playlist: ${error.message}`);
        }
    }

    async deletePlaylist(playlistId) {
        this._checkInitialized();
        try {
            return musicApi.deletePlaylist(playlistId);
        } catch (error) {
            throw new Error(`Failed to delete playlist: ${error.message}`);
        }
    }

    async updatePlaylist(playlistId, title) {
        this._checkInitialized();
        try {
            return musicApi.updatePlaylist(playlistId, title);
        } catch (error) {
            throw new Error(`Failed to update playlist: ${error.message}`);
        }
    }

    async getPlaylist(playlistId) {
        this._checkInitialized();
        try {
            return musicApi.getPlaylist(playlistId);
        } catch (error) {
            throw new Error(`Failed to get playlist: ${error.message}`);
        }
    }

    async getAllPlaylists() {
        this._checkInitialized();
        try {
            return musicApi.getAllPlaylists();
        } catch (error) {
            throw new Error(`Failed to get playlists: ${error.message}`);
        }
    }

    async addSongToPlaylist(songId, playlistId) {
        this._checkInitialized();
        try {
            return musicApi.addSongToPlaylist(songId, playlistId);
        } catch (error) {
            throw new Error(`Failed to add song to playlist: ${error.message}`);
        }
    }

    async removeSongFromPlaylist(songId, playlistId) {
        this._checkInitialized();
        try {
            return musicApi.removeSongFromPlaylist(songId, playlistId);
        } catch (error) {
            throw new Error(`Failed to remove song from playlist: ${error.message}`);
        }
    }

    async getSongsInPlaylist(playlistId) {
        this._checkInitialized();
        try {
            return musicApi.getSongsInPlaylist(playlistId);
        } catch (error) {
            throw new Error(`Failed to get songs in playlist: ${error.message}`);
        }
    }

    // Database Statistics
    async getDatabaseStats() {
        this._checkInitialized();
        try {
            return musicApi.getDatabaseStats();
        } catch (error) {
            throw new Error(`Failed to get database stats: ${error.message}`);
        }
    }

    // Error handling
    getLastError() {
        return musicApi.getLastError();
    }

    // Private helper methods
    _checkInitialized() {
        if (!this.initialized) {
            throw new Error('API not initialized. Call initialize() first.');
        }
    }
}

// Create and export a singleton instance
const musicAPI = new MusicAPI();

module.exports = musicAPI;
