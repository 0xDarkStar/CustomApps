/*
Full Integration Test for Music Player Frontend-Backend
This test verifies the complete integration between the Electron frontend and C++ backend.
*/

const path = require('path');
const musicAPI = require('../api/index');

class FullIntegrationTester {
    constructor() {
        this.testResults = [];
        this.testData = {
            songs: [],
            playlists: []
        };
    }

    async runAllTests() {
        console.log('Starting Full Integration Tests...\n');
        
        try {
            // Test 1: Backend API Initialization
            await this.testBackendInitialization();
            
            // Test 2: Database Operations
            await this.testDatabaseOperations();
            
            // Test 3: Song Management
            await this.testSongManagement();
            
            // Test 4: Playlist Management
            await this.testPlaylistManagement();
            
            // Test 5: Search Functionality
            await this.testSearchFunctionality();
            
            // Test 6: Data Persistence
            await this.testDataPersistence();
            
            // Generate Test Report
            this.generateTestReport();
            
        } catch (error) {
            console.error('Full integration test failed:', error.message);
            this.addTestResult('Full Integration Test', false, error.message);
        }
    }

    async testBackendInitialization() {
        console.log('Testing Backend Initialization...');
        
        try {
            // Test API initialization
            const initResult = await musicAPI.initialize();
            this.addTestResult('API Initialization', initResult, 'Backend API initialized successfully');
            
            // Test API status
            const isInitialized = await musicAPI.isInitialized();
            this.addTestResult('API Status Check', isInitialized, 'API status verified');
            
            console.log('PASSED: Backend initialization tests\n');
        } catch (error) {
            this.addTestResult('Backend Initialization', false, error.message);
            throw error;
        }
    }

    async testDatabaseOperations() {
        console.log('Testing Database Operations...');
        
        try {
            // Test database stats
            const stats = await musicAPI.getDatabaseStats();
            this.addTestResult('Database Stats', typeof stats === 'object', 'Database stats retrieved');
            
            // Verify stats structure
            const hasRequiredFields = stats.hasOwnProperty('totalSongs') && 
                                   stats.hasOwnProperty('totalPlaylists') &&
                                   stats.hasOwnProperty('totalSubtitles') &&
                                   stats.hasOwnProperty('totalPlaylistSongs');
            this.addTestResult('Database Stats Structure', hasRequiredFields, 'Stats structure is correct');
            
            console.log('PASSED: Database operations tests\n');
        } catch (error) {
            this.addTestResult('Database Operations', false, error.message);
            throw error;
        }
    }

    async testSongManagement() {
        console.log('Testing Song Management...');
        
        try {
            // Test adding a song
            const testSong = await musicAPI.addSong(
                'Full Integration Test Song',
                'Test Artist',
                'Test Album',
                180,
                '/test/path/song.mp3'
            );
            
            this.addTestResult('Add Song', testSong && testSong.id, 'Song added successfully');
            this.testData.songs.push(testSong);
            
            // Test getting all songs
            const allSongs = await musicAPI.getAllSongs();
            this.addTestResult('Get All Songs', Array.isArray(allSongs), 'All songs retrieved');
            
            // Test getting specific song
            const specificSong = await musicAPI.getSong(testSong.id);
            this.addTestResult('Get Specific Song', specificSong && specificSong.id === testSong.id, 'Specific song retrieved');
            
            // Test updating song
            const updateResult = await musicAPI.updateSong(testSong.id, 'Updated Title', 'Updated Artist', 'Updated Album');
            this.addTestResult('Update Song', updateResult, 'Song updated successfully');
            
            console.log('PASSED: Song management tests\n');
        } catch (error) {
            this.addTestResult('Song Management', false, error.message);
            throw error;
        }
    }

    async testPlaylistManagement() {
        console.log('Testing Playlist Management...');
        
        try {
            // Test creating playlist
            const testPlaylist = await musicAPI.createPlaylist('Full Integration Test Playlist');
            this.addTestResult('Create Playlist', testPlaylist && testPlaylist.id, 'Playlist created successfully');
            this.testData.playlists.push(testPlaylist);
            
            // Test getting all playlists
            const allPlaylists = await musicAPI.getAllPlaylists();
            this.addTestResult('Get All Playlists', Array.isArray(allPlaylists), 'All playlists retrieved');
            
            // Test getting specific playlist
            const specificPlaylist = await musicAPI.getPlaylist(testPlaylist.id);
            this.addTestResult('Get Specific Playlist', specificPlaylist && specificPlaylist.id === testPlaylist.id, 'Specific playlist retrieved');
            
            // Test adding song to playlist
            if (this.testData.songs.length > 0) {
                const addSongResult = await musicAPI.addSongToPlaylist(this.testData.songs[0].id, testPlaylist.id);
                this.addTestResult('Add Song to Playlist', addSongResult, 'Song added to playlist');
                
                // Test getting songs in playlist
                const playlistSongs = await musicAPI.getSongsInPlaylist(testPlaylist.id);
                this.addTestResult('Get Songs in Playlist', Array.isArray(playlistSongs), 'Songs in playlist retrieved');
            }
            
            console.log('PASSED: Playlist management tests\n');
        } catch (error) {
            this.addTestResult('Playlist Management', false, error.message);
            throw error;
        }
    }

    async testSearchFunctionality() {
        console.log('Testing Search Functionality...');
        
        try {
            // Test song search
            const searchResults = await musicAPI.searchSongs('Test');
            this.addTestResult('Search Songs', Array.isArray(searchResults), 'Song search working');
            
            console.log('PASSED: Search functionality tests\n');
        } catch (error) {
            this.addTestResult('Search Functionality', false, error.message);
            throw error;
        }
    }

    async testDataPersistence() {
        console.log('Testing Data Persistence...');
        
        try {
            // Test that data persists across operations
            const finalStats = await musicAPI.getDatabaseStats();
            this.addTestResult('Data Persistence', finalStats.totalSongs > 0 && finalStats.totalPlaylists > 0, 'Data persisted correctly');
            
            console.log('PASSED: Data persistence tests\n');
        } catch (error) {
            this.addTestResult('Data Persistence', false, error.message);
            throw error;
        }
    }

    addTestResult(testName, passed, message) {
        this.testResults.push({
            test: testName,
            passed: passed,
            message: message,
            timestamp: new Date().toISOString()
        });
    }

    generateTestReport() {
        console.log('\nFULL INTEGRATION TEST REPORT');
        console.log('='.repeat(50));
        
        const passedTests = this.testResults.filter(result => result.passed).length;
        const totalTests = this.testResults.length;
        const successRate = ((passedTests / totalTests) * 100).toFixed(1);
        
        console.log(`Total Tests: ${totalTests}`);
        console.log(`Passed: ${passedTests}`);
        console.log(`Failed: ${totalTests - passedTests}`);
        console.log(`Success Rate: ${successRate}%`);
        console.log('\nDetailed Results:');
        console.log('-'.repeat(50));
        
        this.testResults.forEach(result => {
            const status = result.passed ? '✓' : '✗';
            const statusColor = result.passed ? '\x1b[32m' : '\x1b[31m';
            const resetColor = '\x1b[0m';
            console.log(`${statusColor}${status}${resetColor} ${result.test}: ${result.message}`);
        });
        
        if (passedTests === totalTests) {
            console.log('\nALL FULL INTEGRATION TESTS PASSED!');
        } else {
            console.log('\nSome tests failed. Please review the results above.');
        }
        
        console.log('='.repeat(50));
    }
}

// Run the full integration test
async function runFullIntegrationTest() {
    const tester = new FullIntegrationTester();
    await tester.runAllTests();
    
    // Exit with appropriate code
    const failedTests = tester.testResults.filter(result => !result.passed).length;
    process.exit(failedTests > 0 ? 1 : 0);
}

// Export for use in other test files
module.exports = { FullIntegrationTester, runFullIntegrationTest };

// Run if this file is executed directly
if (require.main === module) {
    runFullIntegrationTest();
}
