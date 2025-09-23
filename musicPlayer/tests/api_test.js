/*
Comprehensive API Test for Music Player
Tests all API functionality in one place - songs, playlists, search, and mock API
*/
const musicAPI = require('../api/index');
const mockMusicAPI = require('../api/mock_api');

async function testAPI() {
    try {
        console.log('=== Testing Music API ===');
        
        // Initialize the API
        console.log('Initializing API...');
        const initResult = await musicAPI.initialize();
        console.log('API initialized:', initResult);
        
        // Test song operations
        console.log('\n=== Testing Song Operations ===');
        
        // Add test songs
        const song1 = await musicAPI.addSong('Test Song', 'Test Artist', 'Test Album', 180, '/path/to/song.mp3');
        console.log('Added song:', song1);
        
        const song2 = await musicAPI.addSong('Another Song', 'Different Artist', 'Different Album', 240, '/path/to/song2.mp3');
        console.log('Added song:', song2);
        
        // Get all songs
        const allSongs = await musicAPI.getAllSongs();
        console.log('Total songs:', allSongs.length);
        
        // Get specific song
        const specificSong = await musicAPI.getSong(song1.id);
        console.log('Retrieved song:', specificSong);
        
        // Update song
        const updateResult = await musicAPI.updateSong(song1.id, 'Updated Title', 'Updated Artist', 'Updated Album');
        console.log('Updated song:', updateResult);
        
        // Search songs
        const searchResults = await musicAPI.searchSongs('Test');
        console.log('Search results:', searchResults.length);
        
        // Test playlist operations
        console.log('\n=== Testing Playlist Operations ===');
        
        // Create playlist
        const playlist = await musicAPI.createPlaylist('Test Playlist');
        console.log('Created playlist:', playlist);
        
        // Add song to playlist
        const addToPlaylist = await musicAPI.addSongToPlaylist(song1.id, playlist.id);
        console.log('Added song to playlist:', addToPlaylist);
        
        // Get songs in playlist
        const playlistSongs = await musicAPI.getSongsInPlaylist(playlist.id);
        console.log('Songs in playlist:', playlistSongs.length);
        
        // Get all playlists
        const allPlaylists = await musicAPI.getAllPlaylists();
        console.log('Total playlists:', allPlaylists.length);
        
        // Test database stats
        console.log('\n=== Testing Database Stats ===');
        const stats = await musicAPI.getDatabaseStats();
        console.log('Database stats:', stats);
        
        // Test mock API
        console.log('\n=== Testing Mock API ===');
        const mockInit = await mockMusicAPI.initialize();
        console.log('Mock API initialized:', mockInit);
        
        const mockSong = await mockMusicAPI.addSong('Mock Song', 'Mock Artist', 200, '/mock/path.mp3');
        console.log('Added mock song:', mockSong);
        
        const mockSongs = await mockMusicAPI.getAllSongs();
        console.log('Mock songs count:', mockSongs.length);
        
        console.log('\n=== All tests completed successfully! ===');
        
    } catch (error) {
        console.error('Test failed:', error);
        process.exit(1);
    }
}

// Run the test
testAPI();
