/*
Made By: 0xDarkStar
Comprehensive test for the enhanced Music API.
Tests all new functionality including album support, song editing, and playlist management.
*/
const musicAPI = require('../api/index');

async function testEnhancedAPI() {
    try {
        console.log('=== Testing Enhanced Music API ===');
        
        // Initialize the API
        console.log('Initializing API...');
        const initResult = await musicAPI.initialize();
        console.log('✓ API initialized:', initResult);
        
        // Test 1: Add songs with album information
        console.log('\n=== Testing Song Management with Albums ===');
        
        const song1 = await musicAPI.addSong('Bohemian Rhapsody', 'Queen', 'A Night at the Opera', 355, '/music/bohemian_rhapsody.mp3');
        console.log('✓ Added song with album:', song1);
        
        const song2 = await musicAPI.addSong('Hotel California', 'Eagles', 'Hotel California', 391, '/music/hotel_california.mp3');
        console.log('✓ Added song with album:', song2);
        
        const song3 = await musicAPI.addSong('Stairway to Heaven', 'Led Zeppelin', 'Led Zeppelin IV', 482, '/music/stairway_to_heaven.mp3');
        console.log('✓ Added song with album:', song3);
        
        // Test 2: Get individual songs
        console.log('\n=== Testing Individual Song Retrieval ===');
        
        const retrievedSong = await musicAPI.getSong(song1.id);
        console.log('✓ Retrieved song by ID:', retrievedSong);
        
        // Test 3: Update song information
        console.log('\n=== Testing Song Updates ===');
        
        const updatedSong = await musicAPI.updateSong(song1.id, 'Bohemian Rhapsody (Remastered)', 'Queen', 'A Night at the Opera (Deluxe Edition)');
        console.log('✓ Updated song:', updatedSong);
        
        // Test 4: Search songs
        console.log('\n=== Testing Song Search ===');
        
        const queenSongs = await musicAPI.searchSongs('Queen');
        console.log(`✓ Found ${queenSongs.length} Queen songs`);
        
        const albumSongs = await musicAPI.searchSongs('Night at the Opera');
        console.log(`✓ Found ${albumSongs.length} songs matching album search`);
        
        // Test 5: Playlist management
        console.log('\n=== Testing Playlist Management ===');
        
        const rockPlaylist = await musicAPI.createPlaylist('Classic Rock');
        console.log('✓ Created playlist:', rockPlaylist);
        
        const favoritesPlaylist = await musicAPI.createPlaylist('My Favorites');
        console.log('✓ Created playlist:', favoritesPlaylist);
        
        // Test 6: Add songs to playlists
        console.log('\n=== Testing Playlist Song Management ===');
        
        await musicAPI.addSongToPlaylist(song1.id, rockPlaylist.id);
        console.log('✓ Added song to rock playlist');
        
        await musicAPI.addSongToPlaylist(song2.id, rockPlaylist.id);
        console.log('✓ Added song to rock playlist');
        
        await musicAPI.addSongToPlaylist(song3.id, rockPlaylist.id);
        console.log('✓ Added song to rock playlist');
        
        await musicAPI.addSongToPlaylist(song1.id, favoritesPlaylist.id);
        console.log('✓ Added song to favorites playlist');
        
        await musicAPI.addSongToPlaylist(song3.id, favoritesPlaylist.id);
        console.log('✓ Added song to favorites playlist');
        
        // Test 7: Get songs in playlists
        console.log('\n=== Testing Playlist Content Retrieval ===');
        
        const rockSongs = await musicAPI.getSongsInPlaylist(rockPlaylist.id);
        console.log(`✓ Rock playlist contains ${rockSongs.length} songs`);
        
        const favoriteSongs = await musicAPI.getSongsInPlaylist(favoritesPlaylist.id);
        console.log(`✓ Favorites playlist contains ${favoriteSongs.length} songs`);
        
        // Test 8: Update playlist title
        console.log('\n=== Testing Playlist Updates ===');
        
        const updatedPlaylist = await musicAPI.updatePlaylist(rockPlaylist.id, 'Classic Rock Masterpieces');
        console.log('✓ Updated playlist title:', updatedPlaylist);
        
        // Test 9: Remove song from playlist
        console.log('\n=== Testing Song Removal from Playlist ===');
        
        await musicAPI.removeSongFromPlaylist(song2.id, rockPlaylist.id);
        console.log('✓ Removed song from playlist');
        
        const updatedRockSongs = await musicAPI.getSongsInPlaylist(rockPlaylist.id);
        console.log(`✓ Rock playlist now contains ${updatedRockSongs.length} songs`);
        
        // Test 10: Get all playlists
        console.log('\n=== Testing Playlist Retrieval ===');
        
        const allPlaylists = await musicAPI.getAllPlaylists();
        console.log(`✓ Found ${allPlaylists.length} playlists`);
        
        // Test 11: Get individual playlist
        console.log('\n=== Testing Individual Playlist Retrieval ===');
        
        const retrievedPlaylist = await musicAPI.getPlaylist(favoritesPlaylist.id);
        console.log('✓ Retrieved playlist by ID:', retrievedPlaylist);
        
        // Test 12: Database statistics
        console.log('\n=== Testing Database Statistics ===');
        
        const stats = await musicAPI.getDatabaseStats();
        console.log('✓ Database stats:', stats);
        
        // Test 13: Error handling
        console.log('\n=== Testing Error Handling ===');
        
        try {
            await musicAPI.getSong(999); // Non-existent song
        } catch (error) {
            console.log('✓ Correctly caught error for non-existent song:', error.message);
        }
        
        try {
            await musicAPI.addSong('', 'Artist', 'Album', 180, '/path/song.mp3'); // Empty title
        } catch (error) {
            console.log('✓ Correctly caught error for invalid song data:', error.message);
        }
        
        try {
            await musicAPI.removeSongFromPlaylist(song1.id, 999); // Non-existent playlist
        } catch (error) {
            console.log('✓ Correctly caught error for non-existent playlist:', error.message);
        }
        
        // Test 14: Delete playlist
        console.log('\n=== Testing Playlist Deletion ===');
        
        const deleteResult = await musicAPI.deletePlaylist(favoritesPlaylist.id);
        console.log('✓ Deleted playlist:', deleteResult);
        
        const remainingPlaylists = await musicAPI.getAllPlaylists();
        console.log(`✓ Remaining playlists: ${remainingPlaylists.length}`);
        
        // Test 15: Delete song
        console.log('\n=== Testing Song Deletion ===');
        
        const deleteSongResult = await musicAPI.deleteSong(song2.id);
        console.log('✓ Deleted song:', deleteSongResult);
        
        const remainingSongs = await musicAPI.getAllSongs();
        console.log(`✓ Remaining songs: ${remainingSongs.length}`);
        
        console.log('\n=== All Enhanced API Tests Completed Successfully! ===');
        
        // Shutdown
        musicAPI.shutdown();
        console.log('✓ API shutdown completed');
        
    } catch (error) {
        console.error('❌ Test failed:', error.message);
        console.error('Last error:', musicAPI.getLastError());
        process.exit(1);
    }
}

// Run the enhanced test
testEnhancedAPI();
