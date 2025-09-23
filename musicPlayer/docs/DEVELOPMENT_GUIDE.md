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

4. **Test Backend-Frontend Integration**:
```bash
# Test native module in Node.js
cd api
node -e "const api = require('./build/Release/music_api'); console.log('✅ Node.js:', Object.keys(api));"

# Test native module in Electron
cd ../frontend
./node_modules/.bin/electron -e "const api = require('../api/build/Release/music_api'); console.log('✅ Electron:', Object.keys(api));"
```

### Building the Native Module

1. **Install dependencies**:
```bash
cd api
npm install
```

2. **Build for Node.js development**:
```bash
npm run build
```

3. **Build for Electron production**:
```bash
npm run rebuild
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

- Node.js (v24.8.0 or higher)
- npm
- SQLite3 development libraries
- C++ compiler (gcc/clang)
- Python 3.12+ (for node-gyp)
- Electron 38.1.2

### Build Commands

#### For Node.js Development
```bash
# Install dependencies
cd api
npm install

# Build native module for Node.js
npm run build

# Clean build
npm run clean
```

#### For Electron Production
```bash
# Install dependencies
cd api
npm install

# Build native module for Electron 38.1.2
HOME=~/.electron-gyp npx node-gyp rebuild --target=38.1.2 --arch=arm64 --dist-url=https://electronjs.org/headers

# Alternative: Use @electron/rebuild
cd ../frontend
npx @electron/rebuild --module-dir ../api
```

### Important Notes

- **NODE_MODULE_VERSION Compatibility**: The native module must be built specifically for the target environment:
  - Node.js 24.8.0 uses NODE_MODULE_VERSION 137
  - Electron 38.1.2 uses NODE_MODULE_VERSION 139
- **Architecture**: Build for the correct architecture (arm64 for Apple Silicon, x64 for Intel)
- **Electron Headers**: Always use `--dist-url=https://electronjs.org/headers` for Electron builds

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

## Troubleshooting

### Common Issues

1. **NODE_MODULE_VERSION Mismatch**: 
   ```
   Error: The module was compiled against a different Node.js version using NODE_MODULE_VERSION 137. 
   This version of Node.js requires NODE_MODULE_VERSION 139.
   ```
   **Solution**: Rebuild the native module for the correct environment:
   ```bash
   # For Electron
   HOME=~/.electron-gyp npx node-gyp rebuild --target=38.1.2 --arch=arm64 --dist-url=https://electronjs.org/headers
   
   # For Node.js
   npx node-gyp rebuild
   ```

2. **Electron app object undefined**:
   ```
   TypeError: Cannot read properties of undefined (reading 'whenReady')
   ```
   **Solution**: Reinstall Electron completely:
   ```bash
   cd frontend
   rm -rf node_modules package-lock.json
   npm install
   ```

3. **Module not found**: Ensure the native module is built with the correct build command
4. **Database errors**: Check that the data directory exists and is writable
5. **Compilation errors**: Verify SQLite3 development libraries are installed

### Build Environment Issues

- **Python Version**: Ensure Python 3.12+ is available for node-gyp
- **Architecture Mismatch**: Build for the correct architecture (arm64 vs x64)
- **Electron Headers**: Always use the official Electron headers URL
- **Clean Builds**: Use `npm run clean` before rebuilding if issues persist

### Debug Mode

Enable debug logging by setting the environment variable:
```bash
DEBUG=music-api node your_app.js
```

### Verification Commands

Test the integration:
```bash
# Test native module in Node.js
cd api
node -e "const api = require('./build/Release/music_api'); console.log('Methods:', Object.keys(api));"

# Test native module in Electron
cd ../frontend
./node_modules/.bin/electron -e "const api = require('../api/build/Release/music_api'); console.log('Electron + API:', Object.keys(api));"
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## Support

For issues and questions, please create an issue in the repository.
