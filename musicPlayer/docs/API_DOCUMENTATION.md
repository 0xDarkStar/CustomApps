# Music Player API Documentation

## Overview

The Music Player API provides a clean interface for managing songs, playlists, and subtitles in the music player application. It consists of:

1. **C++ Backend** (`musicAPI.h`, `musicAPI.cpp`) - Core functionality
2. **Node.js Native Module** (`node_music_api.cpp`) - Bridge to Electron
3. **JavaScript Wrapper** (`index.js`) - Easy-to-use interface

## Architecture

```
Electron Frontend → JavaScript Wrapper → Node.js Native Module → C++ API → SQLite Database
```

## Installation

### Prerequisites
- Node.js (v14 or higher)
- npm
- SQLite3 development libraries
- C++ compiler (gcc/clang)

### Build Steps

1. Install dependencies:
```bash
cd backend
npm install
```

2. Build the native module:
```bash
npm run build
```

3. Test the API:
```bash
node test_api.js
```

## API Reference

### Initialization

#### `initialize()`
Initializes the API and database connection.

**Returns:** `Promise<boolean>` - Success status

**Example:**
```javascript
const musicAPI = require('./index');
await musicAPI.initialize();
```

#### `isInitialized()`
Checks if the API is properly initialized.

**Returns:** `boolean`

#### `shutdown()`
Shuts down the API and cleans up resources.

### Song Management

#### `addSong(title, artist, length, path)`
Adds a new song to the database.

**Parameters:**
- `title` (string) - Song title
- `artist` (string) - Artist name
- `length` (number) - Duration in seconds
- `path` (string) - File path

**Returns:** `Promise<Object>` - Song object with ID

**Example:**
```javascript
const song = await musicAPI.addSong('My Song', 'Artist Name', 180, '/path/to/song.mp3');
console.log('Added song with ID:', song.id);
```

#### `deleteSong(songId)`
Deletes a song from the database.

**Parameters:**
- `songId` (number) - Song ID

**Returns:** `Promise<boolean>` - Success status

#### `getAllSongs()`
Retrieves all songs from the database.

**Returns:** `Promise<Array>` - Array of song objects

#### `searchSongs(query)`
Searches for songs by title or artist.

**Parameters:**
- `query` (string) - Search term

**Returns:** `Promise<Array>` - Array of matching song objects

### Playlist Management

#### `createPlaylist(title)`
Creates a new playlist.

**Parameters:**
- `title` (string) - Playlist name

**Returns:** `Promise<Object>` - Playlist object with ID

#### `getAllPlaylists()`
Retrieves all playlists.

**Returns:** `Promise<Array>` - Array of playlist objects

#### `addSongToPlaylist(songId, playlistId)`
Adds a song to a playlist.

**Parameters:**
- `songId` (number) - Song ID
- `playlistId` (number) - Playlist ID

**Returns:** `Promise<boolean>` - Success status

#### `getSongsInPlaylist(playlistId)`
Gets all songs in a specific playlist.

**Parameters:**
- `playlistId` (number) - Playlist ID

**Returns:** `Promise<Array>` - Array of song objects

### Database Statistics

#### `getDatabaseStats()`
Gets database statistics.

**Returns:** `Promise<Object>` - Statistics object

**Example:**
```javascript
const stats = await musicAPI.getDatabaseStats();
console.log('Total songs:', stats.totalSongs);
console.log('Total playlists:', stats.totalPlaylists);
```

### Error Handling

#### `getLastError()`
Gets the last error message.

**Returns:** `string` - Error message

## Data Structures

### Song Object
```javascript
{
  id: number,
  title: string,
  artist: string,
  length: number,  // in seconds
  path: string
}
```

### Playlist Object
```javascript
{
  id: number,
  title: string,
  length: number,    // total length in seconds
  numSongs: number   // number of songs
}
```

### Database Stats Object
```javascript
{
  totalSongs: number,
  totalPlaylists: number,
  totalSubtitles: number,
  totalPlaylistSongs: number
}
```

## Error Handling

The API uses JavaScript Promises and throws errors for various conditions:

- **APIException** - General API errors
- **DatabaseException** - Database-related errors
- **ValidationError** - Input validation errors

**Example:**
```javascript
try {
  const song = await musicAPI.addSong('', 'Artist', 180, '/path'); // Empty title
} catch (error) {
  console.error('Error:', error.message);
  console.error('Last error:', musicAPI.getLastError());
}
```

## Performance Considerations

- The API uses prepared statements for security and performance
- Database connections are managed efficiently
- Batch operations are available for bulk inserts
- All operations are asynchronous and non-blocking

## Security Features

- SQL injection prevention through prepared statements
- Input validation and sanitization
- File path validation
- Proper error handling without information leakage

## Integration with Electron

The API is designed to work seamlessly with Electron's main process:

```javascript
// In main process
const musicAPI = require('./backend/index');

// Initialize
await musicAPI.initialize();

// Use in IPC handlers
ipcMain.handle('add-song', async (event, title, artist, length, path) => {
  return await musicAPI.addSong(title, artist, length, path);
});
```

## Troubleshooting

### Common Issues

1. **Module not found**: Ensure the native module is built with `npm run build`
2. **Database errors**: Check that the data directory exists and is writable
3. **Compilation errors**: Verify SQLite3 development libraries are installed

### Debug Mode

Enable debug logging by setting the environment variable:
```bash
DEBUG=music-api node your_app.js
```

## Future Enhancements

- Subtitle management functions
- Playlist deletion
- Song removal from playlists
- Advanced search filters
- Database backup/restore
- Performance monitoring
