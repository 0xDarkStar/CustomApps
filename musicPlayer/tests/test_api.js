/*
Made By: 0xDarkStar
Test file for the JavaScript API wrapper.
*/
const musicAPI = require('../api/index');

async function testAPI() {
    try {
        console.log('=== Testing Music API ===');
        
        // Initialize the API
        console.log('Initializing API...');
        const initResult = await musicAPI.initialize();
        console.log('API initialized:', initResult);
        
        // Test song operations
        console.log('\n=== Testing Song Operations ===');
        
        // Add a test song
        const song = await musicAPI.addSong('Test Song', 'Test Artist', 180, 'Test Album', '/path/to/song.mp3');
        console.log('Added song:', song);
        
        // Get all songs
        const allSongs = await musicAPI.getAllSongs();
        console.log('Total songs:', allSongs.length);
        
        // Search songs
        const searchResults = await musicAPI.searchSongs('Test');
        console.log('Search results:', searchResults.length);
        
        // Test playlist operations
        console.log('\n=== Testing Playlist Operations ===');
        
        // Create a playlist
        const playlist = await musicAPI.createPlaylist('My Test Playlist');
        console.log('Created playlist:', playlist);
        
        // Add song to playlist
        const addResult = await musicAPI.addSongToPlaylist(song.id, playlist.id);
        console.log('Added song to playlist:', addResult);
        
        // Get songs in playlist
        const playlistSongs = await musicAPI.getSongsInPlaylist(playlist.id);
        console.log('Songs in playlist:', playlistSongs.length);
        
        // Test database stats
        console.log('\n=== Testing Database Stats ===');
        const stats = await musicAPI.getDatabaseStats();
        console.log('Database stats:', stats);
        
        console.log('\n=== All tests completed successfully! ===');
        
        // Shutdown
        musicAPI.shutdown();
        
    } catch (error) {
        console.error('Test failed:', error.message);
        console.error('Last error:', musicAPI.getLastError());
        process.exit(1);
    }
}

// Run the test
testAPI();
