/*
Made By: 0xDarkStar
Test file for the mock API.
*/
const mockMusicAPI = require('../api/mock_api');

async function testMockAPI() {
    try {
        console.log('=== Testing Mock Music API ===');
        
        // Initialize the API
        console.log('Initializing API...');
        const initResult = await mockMusicAPI.initialize();
        console.log('API initialized:', initResult);
        
        // Test song operations
        console.log('\n=== Testing Song Operations ===');
        
        // Add some test songs
        const song1 = await mockMusicAPI.addSong('Test Song 1', 'Test Artist', 180, '/path/to/song1.mp3');
        console.log('Added song:', JSON.stringify(song1, null, 2));
        
        const song2 = await mockMusicAPI.addSong('Another Song', 'Different Artist', 240, '/path/to/song2.mp4');
        console.log('Added song:', JSON.stringify(song2, null, 2));
        
        // Get all songs
        const allSongs = await mockMusicAPI.getAllSongs();
        console.log('Total songs:', allSongs.length);
        
        // Search songs
        const searchResults = await mockMusicAPI.searchSongs('Test');
        console.log('Search results for "Test":', searchResults.length);
        
        // Test playlist operations
        console.log('\n=== Testing Playlist Operations ===');
        
        // Create a playlist
        const playlist = await mockMusicAPI.createPlaylist('My Test Playlist');
        console.log('Created playlist:', JSON.stringify(playlist, null, 2));
        
        // Add songs to playlist
        const addResult1 = await mockMusicAPI.addSongToPlaylist(song1.id, playlist.id);
        console.log('Added song 1 to playlist:', addResult1);
        
        const addResult2 = await mockMusicAPI.addSongToPlaylist(song2.id, playlist.id);
        console.log('Added song 2 to playlist:', addResult2);
        
        // Get songs in playlist
        const playlistSongs = await mockMusicAPI.getSongsInPlaylist(playlist.id);
        console.log('Songs in playlist:', playlistSongs.length);
        
        // Test database stats
        console.log('\n=== Testing Database Stats ===');
        const stats = await mockMusicAPI.getDatabaseStats();
        console.log('Database stats:', JSON.stringify(stats, null, 2));
        
        // Test error handling
        console.log('\n=== Testing Error Handling ===');
        try {
            await mockMusicAPI.addSong('', 'Artist', 180, '/path/to/song.mp3'); // Empty title should fail
        } catch (error) {
            console.log('Caught expected error:', error.message);
        }
        
        try {
            await mockMusicAPI.deleteSong(999); // Non-existent song should fail
        } catch (error) {
            console.log('Caught expected error:', error.message);
        }
        
        // Test uninitialized API
        console.log('\n=== Testing Uninitialized API ===');
        mockMusicAPI.shutdown();
        try {
            await mockMusicAPI.getAllSongs();
        } catch (error) {
            console.log('Caught expected error:', error.message);
        }
        
        console.log('\n=== All mock API tests completed successfully! ===');
        
    } catch (error) {
        console.error('Test failed:', error.message);
        console.error('Last error:', mockMusicAPI.getLastError());
        process.exit(1);
    }
}

// Run the test
testMockAPI();
