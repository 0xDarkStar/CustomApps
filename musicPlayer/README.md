# Music Player
## Brief Description
This is an application meant to allow users to playback any songs or videos they have on their computer. Any videos or songs they playback would also show subtitles (if any) for that song or video.
It will allow the user to:
- Create custom playlists
- Shuffle playlists to listen to songs in a randomized order
- Editing of data related to songs (location of files, subtitles to use, title, artist, etc.)
- Playing of songs and music videos
- Delete songs, subtitles, and playlists
- maybe more in the future...

## Supported File Formats
All file formats that I know work with it are:
- MP4
- WEBM
- MP3

## Installation

### Prerequisites
- **Node.js** (v16 or higher)
- **npm** (comes with Node.js)
- **C++ Compiler** (g++ or clang++)
- **SQLite3** development libraries

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd musicPlayer

# Install dependencies
npm install

# Build the native module
cd api && npm run build

# Start the application
npm start
```

### Development Setup
```bash
# Install all dependencies
npm install
cd api && npm install
cd ../frontend && npm install

# Build the native module
cd ../api && npm run build

# Run tests
./run_tests.sh

# Start development
npm run dev
```


## Development Status

### Completed Features
- **Backend API**: Complete C++ backend with SQLite database
- **Playlist Management**: Create, edit, and delete playlists
- **Song Management**: Add, edit, and remove songs
- **Database Operations**: Full CRUD operations for songs, playlists, and subtitles
- **Input Validation**: Security features and data validation
- **Testing Suite**: Comprehensive test coverage
- **Documentation**: Complete API documentation

### In Progress
- **Frontend UI**: Electron-based user interface
- **Music Player**: Cross-platform sound engine

### Planned Features
- **Custom Images**: Playlist and song thumbnails
- **Advanced Playback**: Shuffle, repeat, and advanced controls
- **File Import**: Bulk file import and organization
- **Search & Filter**: Advanced search and filtering capabilities

## Project Structure

```
musicPlayer/
├── api/              # Node.js wrapper and native C++ module
├── core/             # C++ database implementation
├── frontend/         # Electron frontend application
├── tests/            # Test suite
├── docs/             # Documentation
└── data/             # Database and media files
```

## Documentation

- **[API Documentation](docs/API_DOCUMENTATION.md)** - Complete API reference
- **[Development Guide](docs/DEVELOPMENT_GUIDE.md)** - Development setup and guidelines