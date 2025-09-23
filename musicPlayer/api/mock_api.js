/*
Mock API for testing the interface without the native module.
This simulates the behavior of the real API for testing purposes.
*/
class MockMusicAPI {
    constructor() {
        this.initialized = false;
        this.songs = [];
        this.playlists = [];
        this.nextSongId = 1;
        this.nextPlaylistId = 1;
    }

    // Initialize the API
    async initialize() {
        console.log('Mock API: Initializing...');
        this.initialized = true;
        return true;
    }

    // Check if API is initialized
    isInitialized() {
        return this.initialized;
    }

    // Shutdown the API
    shutdown() {
        console.log('Mock API: Shutting down...');
        this.initialized = false;
    }

    // Song Management
    async addSong(title, artist, length, path) {
        this._checkInitialized();
        console.log(`Mock API: Adding song "${title}" by "${artist}"`);
        
        const song = {
            id: this.nextSongId++,
            title,
            artist,
            length,
            path
        };
        
        this.songs.push(song);
        return song;
    }

    async deleteSong(songId) {
        this._checkInitialized();
        console.log(`Mock API: Deleting song with ID ${songId}`);
        
        const index = this.songs.findIndex(song => song.id === songId);
        if (index === -1) {
            throw new Error(`Song with ID ${songId} not found`);
        }
        
        this.songs.splice(index, 1);
        return true;
    }

    async getAllSongs() {
        this._checkInitialized();
        console.log(`Mock API: Getting all songs (${this.songs.length} total)`);
        return [...this.songs]; // Return a copy
    }

    async searchSongs(query) {
        this._checkInitialized();
        console.log(`Mock API: Searching for "${query}"`);
        
        const lowerQuery = query.toLowerCase();
        return this.songs.filter(song => 
            song.title.toLowerCase().includes(lowerQuery) ||
            song.artist.toLowerCase().includes(lowerQuery)
        );
    }

    // Playlist Management
    async createPlaylist(title) {
        this._checkInitialized();
        console.log(`Mock API: Creating playlist "${title}"`);
        
        const playlist = {
            id: this.nextPlaylistId++,
            title,
            length: 0,
            numSongs: 0
        };
        
        this.playlists.push(playlist);
        return playlist;
    }

    async getAllPlaylists() {
        this._checkInitialized();
        console.log(`Mock API: Getting all playlists (${this.playlists.length} total)`);
        return [...this.playlists]; // Return a copy
    }

    async addSongToPlaylist(songId, playlistId) {
        this._checkInitialized();
        console.log(`Mock API: Adding song ${songId} to playlist ${playlistId}`);
        
        const song = this.songs.find(s => s.id === songId);
        const playlist = this.playlists.find(p => p.id === playlistId);
        
        if (!song) {
            throw new Error(`Song with ID ${songId} not found`);
        }
        if (!playlist) {
            throw new Error(`Playlist with ID ${playlistId} not found`);
        }
        
        // In a real implementation, this would update the database
        playlist.numSongs++;
        playlist.length += song.length;
        
        return true;
    }

    async getSongsInPlaylist(playlistId) {
        this._checkInitialized();
        console.log(`Mock API: Getting songs in playlist ${playlistId}`);
        
        const playlist = this.playlists.find(p => p.id === playlistId);
        if (!playlist) {
            throw new Error(`Playlist with ID ${playlistId} not found`);
        }
        
        // In a real implementation, this would query the database
        // For now, return all songs (simplified)
        return [...this.songs];
    }

    // Database Statistics
    async getDatabaseStats() {
        this._checkInitialized();
        console.log('Mock API: Getting database stats');
        
        return {
            totalSongs: this.songs.length,
            totalPlaylists: this.playlists.length,
            totalSubtitles: 0, // Not implemented in mock
            totalPlaylistSongs: this.songs.length // Simplified
        };
    }

    // Error handling
    getLastError() {
        return this.lastError || '';
    }

    // Private helper methods
    _checkInitialized() {
        if (!this.initialized) {
            throw new Error('API not initialized. Call initialize() first.');
        }
    }
}

// Create and export a singleton instance
const mockMusicAPI = new MockMusicAPI();

module.exports = mockMusicAPI;
