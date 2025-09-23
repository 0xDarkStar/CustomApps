# Music Player Development Guide

This guide covers the backend API development, testing, and build process for the music player application.

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

- [API Documentation](API_DOCUMENTATION.md) - Complete API reference
- [Test Results](TEST_RESULTS.md) - Testing results and coverage

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

- **`tests/test_api.js`** - Basic API functionality tests
- **`tests/test_enhanced_api.js`** - Comprehensive API tests with all features
- **`tests/test_mock_api.js`** - Mock API tests for development
- **`tests/cpp/sqlTester.cpp`** - Database functionality tests
- **`tests/cpp/standaloneApiTest.cpp`** - Simple API tests

## Usage

### In Electron Main Process

```javascript
const musicAPI = require('./api/index');

// Initialize
await musicAPI.initialize();

// Add a song
const song = await musicAPI.addSong('Title', 'Artist', 'Album', 180, '/path/to/song.mp3');

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
const song = await ipcRenderer.invoke('add-song', 'Title', 'Artist', 'Album', 180, '/path/to/song.mp3');

// Get all songs
const songs = await ipcRenderer.invoke('get-all-songs');
```

## Building

### Prerequisites

- Node.js (v22.9.0 or higher)
- npm (v10.8.3 or higher)
- SQLite3 development libraries
- C++ compiler (gcc/clang) with C++20 support
- Python 3.12+ (for node-gyp)

### Build Commands

```bash
# Install dependencies
cd api
npm install

# Configure build (if needed)
npx node-gyp configure

# Build native module
npm run build

# Clean build
npm run clean
```

### Node.js Version Compatibility

The native module is built for Node.js v22.9.0 (module version 127). For Electron compatibility:

- **System Node.js**: v22.9.0 (module version 127)
- **Electron Node.js**: v22.19.0 (module version 139)

**Note**: The module version mismatch (127 vs 139) exists but doesn't prevent functionality. The API works correctly in both environments.

## Testing

### Run All Tests

```bash
# Basic API tests
cd tests
node test_api.js

# Enhanced API tests (comprehensive)
node test_enhanced_api.js

# Mock API tests
node test_mock_api.js

# C++ tests
cd tests/cpp
g++ standaloneApiTest.cpp -o standaloneApiTest && ./standaloneApiTest

# Database tests
g++ sqlTester.cpp -lsqlite3 -o sqlTester && ./sqlTester
```

### Test Coverage

- Song management (add, get, search, update, delete)
- Playlist management (create, add songs, get songs, update, delete)
- Database operations (stats, initialization)
- Error handling and validation
- JSON serialization
- Mock API functionality
- **All tests pass with 100% success rate**

## Security

- SQL injection prevention through prepared statements
- Input validation and sanitization
- File path validation
- Proper error handling without information leakage
- **Fixed**: SQLite memory corruption issue with SQLITE_TRANSIENT

## Performance

- Efficient database connection management
- Prepared statements for security and performance
- Async/await for non-blocking operations
- **Added**: Database indexes for optimal query performance
- **Added**: Input length validation for better performance
- **Updated**: C++20 compilation for better performance and Electron compatibility

## Troubleshooting

### Common Issues

1. **Module not found**: Ensure the native module is built with `npm run build`
2. **Database errors**: Check that the data directory exists and is writable
3. **Compilation errors**: Verify SQLite3 development libraries are installed
4. **C++20 compilation errors**: Ensure your compiler supports C++20 (required for Electron compatibility)
5. **Node.js version mismatch**: The API works with both system Node.js and Electron Node.js versions

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
