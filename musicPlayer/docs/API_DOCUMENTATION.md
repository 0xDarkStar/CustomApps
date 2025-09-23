# Music Player API Documentation

## Overview
This API provides comprehensive functionality for managing songs, playlists, and subtitles in your music player application. It's built with a C++ backend and provides a clean JavaScript interface for frontend development.

## Installation & Setup

### 1. Initialize the API
```javascript
const musicAPI = require('./api/index');

// Initialize the API (call this first)
await musicAPI.initialize();
console.log('API initialized successfully');
```

### 2. Check API Status
```javascript
// Check if API is ready
if (musicAPI.isInitialized()) {
    console.log('API is ready to use');
}
```

## Song Management

### Add a Song
```javascript
// Add a new song to the database
const song = await musicAPI.addSong(
    'Bohemian Rhapsody',     // title
    'Queen',                 // artist  
    'A Night at the Opera',  // album
    355,                     // length in seconds
    '/path/to/song.mp3'      // file path
);

console.log('Added song:', song);
// Returns: { id: 1, title: 'Bohemian Rhapsody', artist: 'Queen', album: 'A Night at the Opera', length: 355, path: '/path/to/song.mp3' }
```

### Get All Songs
```javascript
// Get all songs in the database
const songs = await musicAPI.getAllSongs();
console.log(`Found ${songs.length} songs`);

// Display songs
songs.forEach(song => {
    console.log(`${song.title} by ${song.artist} (${song.album})`);
});
```

### Get a Specific Song
```javascript
// Get a song by ID
const song = await musicAPI.getSong(1);
console.log('Song details:', song);
// Returns: { id: 1, title: 'Bohemian Rhapsody', artist: 'Queen', album: 'A Night at the Opera', length: 355, path: '/music/bohemian_rhapsody.mp3' }
```

### Update Song Information
```javascript
// Update song metadata (title, artist, album)
const updatedSong = await musicAPI.updateSong(
    1,                      // song ID
    'New Title',            // new title
    'New Artist',           // new artist
    'New Album'             // new album
);

console.log('Updated song:', updatedSong);
```

### Search Songs
```javascript
// Search songs by title, artist, or album
const results = await musicAPI.searchSongs('Queen');
console.log(`Found ${results.length} songs matching "Queen"`);

// Search by album
const albumResults = await musicAPI.searchSongs('Night at the Opera');
```

### Delete a Song
```javascript
// Delete a song by ID
const success = await musicAPI.deleteSong(1);
if (success) {
    console.log('Song deleted successfully');
}
```

## Playlist Management

### Create a Playlist
```javascript
// Create a new playlist
const playlist = await musicAPI.createPlaylist('My Favorites');
console.log('Created playlist:', playlist);
// Returns: { id: 1, title: 'My Favorites', length: 0, numSongs: 0 }
```

### Get All Playlists
```javascript
// Get all playlists
const playlists = await musicAPI.getAllPlaylists();
console.log(`Found ${playlists.length} playlists`);

playlists.forEach(playlist => {
    console.log(`${playlist.title} (${playlist.numSongs} songs)`);
});
```

### Get a Specific Playlist
```javascript
// Get playlist details by ID
const playlist = await musicAPI.getPlaylist(1);
console.log('Playlist details:', playlist);
// Returns: { id: 1, title: 'My Favorites', length: 0, numSongs: 0 }
```

### Update Playlist Title
```javascript
// Rename a playlist
const updatedPlaylist = await musicAPI.updatePlaylist(1, 'New Playlist Name');
console.log('Updated playlist:', updatedPlaylist);
```

### Add Song to Playlist
```javascript
// Add a song to a playlist
const success = await musicAPI.addSongToPlaylist(1, 1); // song ID, playlist ID
if (success) {
    console.log('Song added to playlist');
}
```

### Remove Song from Playlist
```javascript
// Remove a song from a playlist
const success = await musicAPI.removeSongFromPlaylist(1, 1); // song ID, playlist ID
if (success) {
    console.log('Song removed from playlist');
}
// Returns: true if successful, throws error if song not in playlist
```

### Get Songs in Playlist
```javascript
// Get all songs in a specific playlist
const playlistSongs = await musicAPI.getSongsInPlaylist(1);
console.log(`Playlist contains ${playlistSongs.length} songs`);

playlistSongs.forEach(song => {
    console.log(`${song.title} by ${song.artist}`);
});
```

### Delete a Playlist
```javascript
// Delete a playlist (removes all songs from it too)
const success = await musicAPI.deletePlaylist(1);
if (success) {
    console.log('Playlist deleted successfully');
}
```

## Database Statistics

### Get Database Stats
```javascript
// Get overall database statistics
const stats = await musicAPI.getDatabaseStats();
console.log('Database stats:', stats);
// Returns: { totalSongs: 10, totalPlaylists: 3, totalSubtitles: 5, totalPlaylistSongs: 15 }
```

## Error Handling

### Basic Error Handling
```javascript
try {
    const song = await musicAPI.addSong('', 'Artist', 'Album', 180, '/path/song.mp3');
} catch (error) {
    console.error('Error adding song:', error.message);
    // Handle the error appropriately
}
```

### Get Last Error
```javascript
try {
    await musicAPI.addSong('Invalid Song', 'Artist', 'Album', -1, '/path/song.mp3');
} catch (error) {
    console.error('Error:', error.message);
    console.error('Last API error:', musicAPI.getLastError());
}
```

### Common Error Scenarios
```javascript
// 1. Invalid song data
try {
    await musicAPI.addSong('', 'Artist', 'Album', 180, '/path/song.mp3'); // Empty title
} catch (error) {
    console.error('Validation error:', error.message);
    // Error: Failed to add song: Invalid song title
}

// 2. Non-existent song
try {
    await musicAPI.getSong(999); // Song doesn't exist
} catch (error) {
    console.error('Not found error:', error.message);
    // Error: Failed to get song: Song with ID 999 not found
}

// 3. Duplicate song in playlist
try {
    await musicAPI.addSongToPlaylist(1, 1); // Song already in playlist
} catch (error) {
    console.error('Duplicate error:', error.message);
    // Error: Failed to add song to playlist: Song is already in this playlist
}

// 4. API not initialized
try {
    musicAPI.shutdown();
    await musicAPI.getAllSongs(); // API not initialized
} catch (error) {
    console.error('Initialization error:', error.message);
    // Error: API not initialized. Call initialize() first.
}
```

### Error Recovery Strategies
```javascript
// 1. Retry with exponential backoff
async function retryOperation(operation, maxRetries = 3) {
    for (let i = 0; i < maxRetries; i++) {
        try {
            return await operation();
        } catch (error) {
            if (i === maxRetries - 1) throw error;
            await new Promise(resolve => setTimeout(resolve, Math.pow(2, i) * 1000));
        }
    }
}

// 2. Graceful degradation
async function safeGetSong(songId) {
    try {
        return await musicAPI.getSong(songId);
    } catch (error) {
        console.warn(`Song ${songId} not found, returning null`);
        return null;
    }
}

// 3. Batch operations with error handling
async function addMultipleSongs(songData) {
    const results = [];
    for (const song of songData) {
        try {
            const result = await musicAPI.addSong(song.title, song.artist, song.album, song.length, song.path);
            results.push({ success: true, song: result });
        } catch (error) {
            results.push({ success: false, error: error.message, song: song });
        }
    }
    return results;
}
```

## Complete Example: Building a Music Library

```javascript
const musicAPI = require('./api/index');

async function buildMusicLibrary() {
    try {
        // Initialize API
        await musicAPI.initialize();
        console.log('API initialized');

        // Add some songs
        const song1 = await musicAPI.addSong('Bohemian Rhapsody', 'Queen', 'A Night at the Opera', 355, '/music/bohemian_rhapsody.mp3');
        const song2 = await musicAPI.addSong('Hotel California', 'Eagles', 'Hotel California', 391, '/music/hotel_california.mp3');
        const song3 = await musicAPI.addSong('Stairway to Heaven', 'Led Zeppelin', 'Led Zeppelin IV', 482, '/music/stairway_to_heaven.mp3');

        console.log('Added songs:', song1.id, song2.id, song3.id);

        // Create playlists
        const rockPlaylist = await musicAPI.createPlaylist('Classic Rock');
        const favoritesPlaylist = await musicAPI.createPlaylist('My Favorites');

        // Add songs to playlists
        await musicAPI.addSongToPlaylist(song1.id, rockPlaylist.id);
        await musicAPI.addSongToPlaylist(song2.id, rockPlaylist.id);
        await musicAPI.addSongToPlaylist(song3.id, rockPlaylist.id);

        await musicAPI.addSongToPlaylist(song1.id, favoritesPlaylist.id);
        await musicAPI.addSongToPlaylist(song3.id, favoritesPlaylist.id);

        // Get playlist contents
        const rockSongs = await musicAPI.getSongsInPlaylist(rockPlaylist.id);
        console.log(`Rock playlist has ${rockSongs.length} songs`);

        // Search for songs
        const queenSongs = await musicAPI.searchSongs('Queen');
        console.log(`Found ${queenSongs.length} Queen songs`);

        // Get database stats
        const stats = await musicAPI.getDatabaseStats();
        console.log('Database stats:', stats);

        console.log('Music library built successfully!');

    } catch (error) {
        console.error('Error building music library:', error.message);
    }
}

// Run the example
buildMusicLibrary();
```

## Frontend Integration Examples

### Electron Main Process Setup
```javascript
// main.js
const { app, BrowserWindow, ipcMain } = require('electron');
const musicAPI = require('./api/index');

let mainWindow;

async function createWindow() {
    // Initialize API
    await musicAPI.initialize();

    mainWindow = new BrowserWindow({
        width: 1200,
        height: 800,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(__dirname, 'preload.js')
        }
    });

    mainWindow.loadFile('frontend/index.html');
}

// IPC handlers for frontend communication
ipcMain.handle('add-song', async (event, title, artist, album, length, path) => {
    return await musicAPI.addSong(title, artist, album, length, path);
});

ipcMain.handle('get-all-songs', async () => {
    return await musicAPI.getAllSongs();
});

ipcMain.handle('update-song', async (event, songId, title, artist, album) => {
    return await musicAPI.updateSong(songId, title, artist, album);
});

ipcMain.handle('delete-song', async (event, songId) => {
    return await musicAPI.deleteSong(songId);
});

ipcMain.handle('create-playlist', async (event, title) => {
    return await musicAPI.createPlaylist(title);
});

ipcMain.handle('add-song-to-playlist', async (event, songId, playlistId) => {
    return await musicAPI.addSongToPlaylist(songId, playlistId);
});

ipcMain.handle('remove-song-from-playlist', async (event, songId, playlistId) => {
    return await musicAPI.removeSongFromPlaylist(songId, playlistId);
});

ipcMain.handle('get-songs-in-playlist', async (event, playlistId) => {
    return await musicAPI.getSongsInPlaylist(playlistId);
});

ipcMain.handle('delete-playlist', async (event, playlistId) => {
    return await musicAPI.deletePlaylist(playlistId);
});

app.whenReady().then(createWindow);
```

### Frontend Usage (Renderer Process)
```javascript
// frontend/scripts/musicManager.js
const { ipcRenderer } = require('electron');

class MusicManager {
    async addSong(title, artist, album, length, path) {
        try {
            const song = await ipcRenderer.invoke('add-song', title, artist, album, length, path);
            console.log('Song added:', song);
            return song;
        } catch (error) {
            console.error('Error adding song:', error);
            throw error;
        }
    }

    async getAllSongs() {
        try {
            const songs = await ipcRenderer.invoke('get-all-songs');
            return songs;
        } catch (error) {
            console.error('Error getting songs:', error);
            throw error;
        }
    }

    async updateSong(songId, title, artist, album) {
        try {
            const updatedSong = await ipcRenderer.invoke('update-song', songId, title, artist, album);
            console.log('Song updated:', updatedSong);
            return updatedSong;
        } catch (error) {
            console.error('Error updating song:', error);
            throw error;
        }
    }

    async deleteSong(songId) {
        try {
            const success = await ipcRenderer.invoke('delete-song', songId);
            if (success) {
                console.log('Song deleted successfully');
            }
            return success;
        } catch (error) {
            console.error('Error deleting song:', error);
            throw error;
        }
    }

    async createPlaylist(title) {
        try {
            const playlist = await ipcRenderer.invoke('create-playlist', title);
            console.log('Playlist created:', playlist);
            return playlist;
        } catch (error) {
            console.error('Error creating playlist:', error);
            throw error;
        }
    }

    async addSongToPlaylist(songId, playlistId) {
        try {
            const success = await ipcRenderer.invoke('add-song-to-playlist', songId, playlistId);
            if (success) {
                console.log('Song added to playlist');
            }
            return success;
        } catch (error) {
            console.error('Error adding song to playlist:', error);
            throw error;
        }
    }

    async removeSongFromPlaylist(songId, playlistId) {
        try {
            const success = await ipcRenderer.invoke('remove-song-from-playlist', songId, playlistId);
            if (success) {
                console.log('Song removed from playlist');
            }
            return success;
        } catch (error) {
            console.error('Error removing song from playlist:', error);
            throw error;
        }
    }

    async getSongsInPlaylist(playlistId) {
        try {
            const songs = await ipcRenderer.invoke('get-songs-in-playlist', playlistId);
            return songs;
        } catch (error) {
            console.error('Error getting playlist songs:', error);
            throw error;
        }
    }

    async deletePlaylist(playlistId) {
        try {
            const success = await ipcRenderer.invoke('delete-playlist', playlistId);
            if (success) {
                console.log('Playlist deleted successfully');
            }
            return success;
        } catch (error) {
            console.error('Error deleting playlist:', error);
            throw error;
        }
    }
}

// Export for use in frontend
window.MusicManager = new MusicManager();
```

## API Reference

### Song Object Structure
```javascript
{
    id: number,           // Unique song ID
    title: string,        // Song title
    artist: string,       // Artist name
    album: string,        // Album name
    length: number,       // Length in seconds
    path: string          // File path
}
```

### Playlist Object Structure
```javascript
{
    id: number,           // Unique playlist ID
    title: string,        // Playlist name
    length: number,       // Total length in seconds
    numSongs: number      // Number of songs in playlist
}
```

### Database Stats Object Structure
```javascript
{
    totalSongs: number,           // Total number of songs
    totalPlaylists: number,       // Total number of playlists
    totalSubtitles: number,       // Total number of subtitles
    totalPlaylistSongs: number    // Total playlist-song relationships
}
```

## Performance Considerations

### Database Performance
- **Indexes**: The database includes optimized indexes for:
  - Song titles, artists, and albums
  - Playlist titles
  - Foreign key relationships
- **Search Performance**: Song search is performed in-memory for optimal performance with local music libraries
- **Memory Usage**: The API is designed for local use with typical music libraries (hundreds to thousands of songs)

### Best Practices
```javascript
// 1. Batch operations for better performance
async function addMultipleSongs(songData) {
    const results = [];
    for (const song of songData) {
        try {
            const result = await musicAPI.addSong(song.title, song.artist, song.album, song.length, song.path);
            results.push(result);
        } catch (error) {
            console.error(`Failed to add song: ${song.title}`, error.message);
        }
    }
    return results;
}

// 2. Efficient playlist management
async function createPlaylistWithSongs(title, songIds) {
    const playlist = await musicAPI.createPlaylist(title);
    for (const songId of songIds) {
        try {
            await musicAPI.addSongToPlaylist(songId, playlist.id);
        } catch (error) {
            console.warn(`Failed to add song ${songId} to playlist:`, error.message);
        }
    }
    return playlist;
}

// 3. Optimized search usage
async function searchSongsEfficiently(query) {
    // Search is case-insensitive and matches title, artist, and album
    const results = await musicAPI.searchSongs(query);
    return results;
}
```

## Security Considerations

### Input Validation
The API includes comprehensive input validation:

- **Text Fields**: Limited to 255 characters, no control characters
- **File Paths**: Validated for security (no directory traversal)
- **File Extensions**: Restricted to valid audio/subtitle formats
- **SQL Injection**: Protected with prepared statements

### Security Features
```javascript
// 1. Path validation prevents directory traversal
try {
    await musicAPI.addSong('Song', 'Artist', 'Album', 180, '../../../etc/passwd');
} catch (error) {
    console.error('Invalid path:', error.message);
    // Error: Failed to add song: Invalid file path
}

// 2. Input sanitization
try {
    await musicAPI.addSong('Song<script>alert("xss")</script>', 'Artist', 'Album', 180, '/path/song.mp3');
} catch (error) {
    console.error('Invalid input:', error.message);
    // Error: Failed to add song: Invalid song title
}

// 3. File extension validation
try {
    await musicAPI.addSong('Song', 'Artist', 'Album', 180, '/path/song.exe');
} catch (error) {
    console.error('Invalid file type:', error.message);
    // Error: Failed to add song: Invalid file path
}
```

### Supported File Types
- **Audio**: `.mp3`, `.wav`, `.flac`, `.m4a`, `.ogg`
- **Subtitles**: `.srt`, `.vtt`

### Database Security
- **Foreign Key Constraints**: Enforced with CASCADE deletes
- **Prepared Statements**: All database operations use parameterized queries
- **Input Sanitization**: All user inputs are sanitized before database operations