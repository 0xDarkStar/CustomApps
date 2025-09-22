# Music Player Backend

This directory contains the backend API for the music player application.

## Quick Start

### Testing the API

1. **Test the Mock API** (recommended for development):
```bash
cd tests
node test_mock_api.js
```

2. **Test the C++ API**:
```bash
cd tests/cpp
g++ standaloneApiTest.cpp -o standaloneApiTest
./standaloneApiTest
```

3. **Test the Database**:
```bash
cd tests/cpp
g++ sqlTester.cpp -lsqlite3 -o sqlTester
./sqlTester
```

### Building the Native Module

1. **Install dependencies**:
```bash
cd api
npm install
```

2. **Build the module**:
```bash
npm run build
```

## Documentation

- [API Documentation](docs/API_DOCUMENTATION.md) - Complete API reference
- [Test Results](docs/TEST_RESULTS.md) - Testing results and coverage

## Development

### Core Components

- **`core/sqlCommands.h`** - Database operations (songs, playlists, subtitles)
- **`core/sqlSetup.h`** - Database initialization and setup

### API Components

- **`api/musicAPI.h`** - C++ API interface
- **`api/musicAPI.cpp`** - C++ API implementation
- **`api/index.js`** - JavaScript wrapper for Electron
- **`api/mock_api.js`** - Mock API for testing

### Testing

- **`tests/test_mock_api.js`** - Comprehensive mock API tests
- **`tests/cpp/sqlTester.cpp`** - Database functionality tests
- **`tests/cpp/standaloneApiTest.cpp`** - Simple API tests

## Usage

### In Electron Main Process

```javascript
const musicAPI = require('./backend/api/index');

// Initialize
await musicAPI.initialize();

// Add a song
const song = await musicAPI.addSong('Title', 'Artist', 180, '/path/to/song.mp3');

// Get all songs
const songs = await musicAPI.getAllSongs();

// Create a playlist
const playlist = await musicAPI.createPlaylist('My Playlist');

// Add song to playlist
await musicAPI.addSongToPlaylist(song.id, playlist.id);
```

### In Electron Renderer Process

```javascript
const { ipcRenderer } = require('electron');

// Add a song
const song = await ipcRenderer.invoke('add-song', 'Title', 'Artist', 180, '/path/to/song.mp3');

// Get all songs
const songs = await ipcRenderer.invoke('get-all-songs');
```

## Building

### Prerequisites

- Node.js (v14 or higher)
- npm
- SQLite3 development libraries
- C++ compiler (gcc/clang)

### Build Commands

```bash
# Install dependencies
cd api
npm install

# Build native module
npm run build

# Clean build
npm run clean
```

## Testing

### Run All Tests

```bash
# Mock API tests
cd tests
node test_mock_api.js

# C++ tests
cd tests/cpp
g++ standaloneApiTest.cpp -o standaloneApiTest && ./standaloneApiTest

# Database tests
g++ sqlTester.cpp -lsqlite3 -o sqlTester && ./sqlTester
```

### Test Coverage

- Song management (add, get, search, delete)
- Playlist management (create, add songs, get songs)
- Database operations (stats, initialization)
- Error handling and validation
- JSON serialization
- Mock API functionality

## Security

- SQL injection prevention through prepared statements
- Input validation and sanitization
- File path validation
- Proper error handling without information leakage

## Performance

- Efficient database connection management
- Prepared statements for security and performance
- Async/await for non-blocking operations
- Optimized queries with proper indexing

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

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## Support

For issues and questions, please create an issue in the repository.
