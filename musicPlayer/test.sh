#!/bin/bash

# Music Player Integration Test Runner
# This script runs selected integration tests for the music player application

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results tracking
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
FAILED_TEST_LIST=()

# Function to run a test and track results
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    echo -e "${BLUE}Running: $test_name${NC}"
    echo "Command: $test_command"
    echo "----------------------------------------"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Capture both stdout and stderr
    local output
    local exit_code
    
    if output=$(eval "$test_command" 2>&1); then
        echo -e "${GREEN}PASSED: $test_name${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        exit_code=$?
        echo -e "${RED}FAILED: $test_name${NC}"
        echo -e "${RED}Exit code: $exit_code${NC}"
        echo -e "${RED}Error output:${NC}"
        echo "$output" | sed 's/^/  /'
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_LIST+=("$test_name")
    fi
    
    echo ""
}

# Function to show menu and get user choice
show_menu() {
    echo "Music Player Test Suite"
    echo "======================"
    echo ""
    echo "Select tests to run:"
    echo "1) Database Tests Only"
    echo "2) API Tests Only" 
    echo "3) Frontend Tests Only"
    echo "4) Integration Tests Only"
    echo "5) Build Tests Only"
    echo "6) All Tests"
    echo "7) Custom Selection"
    echo "8) Database Interaction Tool"
    echo "9) Exit"
    echo ""
    read -p "Enter your choice (1-9): " choice
}

# Function to run database tests
run_database_tests() {
    echo -e "${BLUE}=== DATABASE TESTS ===${NC}"
    
    # Check if database file exists
    if [ -f "data/musicPlayer.db" ]; then
        echo -e "${GREEN}PASSED: Database file exists${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${YELLOW}WARNING: Database file not found (will be created on first run)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Test database operations through API
    run_test "Database Operations Test" "node tests/api_test.js"
}

# Function to run API tests
run_api_tests() {
    echo -e "${BLUE}=== API TESTS ===${NC}"
    run_test "API Test" "node tests/api_test.js"
}

# Function to run frontend tests
run_frontend_tests() {
    echo -e "${BLUE}=== FRONTEND TESTS ===${NC}"
    run_test "Frontend Test" "node tests/frontend_test.js"
    
    # Check frontend dependencies
    run_test "Frontend Dependencies" "cd frontend && npm install && cd .."
}

# Function to run integration tests
run_integration_tests() {
    echo -e "${BLUE}=== INTEGRATION TESTS ===${NC}"
    run_test "Full Integration Test" "node tests/full_integration_test.js"
}

# Function to run build tests
run_build_tests() {
    echo -e "${BLUE}=== BUILD TESTS ===${NC}"
    run_test "Backend Build Test" "cd api && npm install && cd .."
    
    # Check file structure
    echo -e "${BLUE}=== FILE STRUCTURE TESTS ===${NC}"
    required_dirs=("api" "frontend" "core" "data" "tests")
    for dir in "${required_dirs[@]}"; do
        if [ -d "$dir" ]; then
            echo -e "${GREEN}PASSED: Directory $dir exists${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}FAILED: Directory $dir missing${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
    done
}

# Function to run all tests
run_all_tests() {
    run_database_tests
    run_api_tests
    run_frontend_tests
    run_integration_tests
    run_build_tests
}

# Function for custom selection
run_custom_tests() {
    echo ""
    echo "Available test categories:"
    echo "1) Database Tests"
    echo "2) API Tests"
    echo "3) Frontend Tests"
    echo "4) Integration Tests"
    echo "5) Build Tests"
    echo "6) Database Interaction Tool"
    echo ""
    read -p "Enter test numbers separated by spaces (e.g., 1 3 5): " custom_choice
    
    for choice in $custom_choice; do
        case $choice in
            1) run_database_tests ;;
            2) run_api_tests ;;
            3) run_frontend_tests ;;
            4) run_integration_tests ;;
            5) run_build_tests ;;
            6) run_database_interaction ;;
            *) echo "Invalid choice: $choice" ;;
        esac
    done
}

# Function for database interaction tool
run_database_interaction() {
    echo -e "${BLUE}=== DATABASE INTERACTION TOOL ===${NC}"
    echo ""
    echo "This tool allows you to interact with the music player database using the API."
    echo "You can add songs, create playlists, search, and view database stats."
    echo ""
    
    # Create a temporary Node.js script for database interaction
    cat > /tmp/db_interaction.js << 'EOF'
const path = require('path');
const musicAPI = require(path.join(process.cwd(), 'api/index'));

class DatabaseInteractionTool {
    constructor() {
        this.isInitialized = false;
    }

    async initialize() {
        try {
            await musicAPI.initialize();
            this.isInitialized = true;
            console.log('✓ Database API initialized successfully');
        } catch (error) {
            console.error('✗ Failed to initialize database API:', error.message);
            process.exit(1);
        }
    }

    async showMenu() {
        console.log('\n=== DATABASE INTERACTION MENU ===');
        console.log('1) View Database Stats');
        console.log('2) List All Songs');
        console.log('3) Add New Song');
        console.log('4) Search Songs');
        console.log('5) List All Playlists');
        console.log('6) Create New Playlist');
        console.log('7) Add Song to Playlist');
        console.log('8) View Songs in Playlist');
        console.log('9) Update Song');
        console.log('10) Delete Song');
        console.log('11) Exit');
        console.log('');
    }

    async handleChoice(choice) {
        switch (choice) {
            case '1':
                await this.showStats();
                break;
            case '2':
                await this.listSongs();
                break;
            case '3':
                await this.addSong();
                break;
            case '4':
                await this.searchSongs();
                break;
            case '5':
                await this.listPlaylists();
                break;
            case '6':
                await this.createPlaylist();
                break;
            case '7':
                await this.addSongToPlaylist();
                break;
            case '8':
                await this.viewPlaylistSongs();
                break;
            case '9':
                await this.updateSong();
                break;
            case '10':
                await this.deleteSong();
                break;
            case '11':
                console.log('Exiting database interaction tool...');
                process.exit(0);
                break;
            default:
                console.log('Invalid choice. Please try again.');
        }
    }

    async showStats() {
        try {
            const stats = await musicAPI.getDatabaseStats();
            console.log('\n=== DATABASE STATISTICS ===');
            console.log(`Total Songs: ${stats.totalSongs}`);
            console.log(`Total Playlists: ${stats.totalPlaylists}`);
            console.log(`Total Subtitles: ${stats.totalSubtitles}`);
            console.log(`Total Playlist-Song Relations: ${stats.totalPlaylistSongs}`);
        } catch (error) {
            console.error('Error getting stats:', error.message);
        }
    }

    async listSongs() {
        try {
            const songs = await musicAPI.getAllSongs();
            console.log('\n=== ALL SONGS ===');
            if (songs.length === 0) {
                console.log('No songs found in database.');
            } else {
                songs.forEach(song => {
                    console.log(`ID: ${song.id} | ${song.title} - ${song.artist} (${song.album}) | ${song.length}s`);
                });
            }
        } catch (error) {
            console.error('Error listing songs:', error.message);
        }
    }

    async addSong() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        console.log('\n=== ADD NEW SONG ===');
        
        const title = await this.question(rl, 'Enter song title: ');
        const artist = await this.question(rl, 'Enter artist: ');
        const album = await this.question(rl, 'Enter album: ');
        const length = await this.question(rl, 'Enter length (seconds): ');
        const path = await this.question(rl, 'Enter file path: ');

        try {
            const song = await musicAPI.addSong(title, artist, album, parseInt(length), path);
            console.log(`✓ Song added successfully with ID: ${song.id}`);
        } catch (error) {
            console.error('Error adding song:', error.message);
        }

        rl.close();
    }

    async updateSong() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        console.log('\n=== UPDATE SONG ===');
        
        const songId = await this.question(rl, 'Enter song ID to update: ');
        const newTitle = await this.question(rl, 'Enter new title: ');
        const newArtist = await this.question(rl, 'Enter new artist: ');
        const newAlbum = await this.question(rl, 'Enter new album: ');

        try {
            const result = await musicAPI.updateSong(parseInt(songId), newTitle, newArtist, newAlbum);
            if (result) {
                console.log('✓ Song updated successfully');
            } else {
                console.log('✗ Failed to update song');
            }
        } catch (error) {
            console.error('Error updating song:', error.message);
        }

        rl.close();
    }

    async deleteSong() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        console.log('\n=== DELETE SONG ===');
        
        const songId = await this.question(rl, 'Enter song ID to delete: ');

        try {
            const result = await musicAPI.deleteSong(parseInt(songId));
            if (result) {
                console.log('✓ Song deleted successfully');
            } else {
                console.log('✗ Failed to delete song');
            }
        } catch (error) {
            console.error('Error deleting song:', error.message);
        }

        rl.close();
    }

    async searchSongs() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        const searchTerm = await this.question(rl, 'Enter search term: ');
        
        try {
            const results = await musicAPI.searchSongs(searchTerm);
            console.log(`\n=== SEARCH RESULTS FOR "${searchTerm}" ===`);
            if (results.length === 0) {
                console.log('No songs found matching your search.');
            } else {
                results.forEach(song => {
                    console.log(`ID: ${song.id} | ${song.title} - ${song.artist} (${song.album})`);
                });
            }
        } catch (error) {
            console.error('Error searching songs:', error.message);
        }

        rl.close();
    }

    async listPlaylists() {
        try {
            const playlists = await musicAPI.getAllPlaylists();
            console.log('\n=== ALL PLAYLISTS ===');
            if (playlists.length === 0) {
                console.log('No playlists found in database.');
            } else {
                playlists.forEach(playlist => {
                    console.log(`ID: ${playlist.id} | ${playlist.title} | ${playlist.numSongs} songs | ${playlist.length}s`);
                });
            }
        } catch (error) {
            console.error('Error listing playlists:', error.message);
        }
    }

    async createPlaylist() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        const title = await this.question(rl, 'Enter playlist title: ');

        try {
            const playlist = await musicAPI.createPlaylist(title);
            console.log(`✓ Playlist created successfully with ID: ${playlist.id}`);
        } catch (error) {
            console.error('Error creating playlist:', error.message);
        }

        rl.close();
    }

    async addSongToPlaylist() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        const songId = await this.question(rl, 'Enter song ID: ');
        const playlistId = await this.question(rl, 'Enter playlist ID: ');

        try {
            const result = await musicAPI.addSongToPlaylist(parseInt(songId), parseInt(playlistId));
            if (result) {
                console.log('✓ Song added to playlist successfully');
            } else {
                console.log('✗ Failed to add song to playlist');
            }
        } catch (error) {
            console.error('Error adding song to playlist:', error.message);
        }

        rl.close();
    }

    async viewPlaylistSongs() {
        const readline = require('readline');
        const rl = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });

        const playlistId = await this.question(rl, 'Enter playlist ID: ');

        try {
            const songs = await musicAPI.getSongsInPlaylist(parseInt(playlistId));
            console.log(`\n=== SONGS IN PLAYLIST ${playlistId} ===`);
            if (songs.length === 0) {
                console.log('No songs found in this playlist.');
            } else {
                songs.forEach(song => {
                    console.log(`ID: ${song.id} | ${song.title} - ${song.artist} (${song.album}) | ${song.length}s`);
                });
            }
        } catch (error) {
            console.error('Error getting playlist songs:', error.message);
        }

        rl.close();
    }

    question(rl, prompt) {
        return new Promise((resolve) => {
            rl.question(prompt, resolve);
        });
    }

    async run() {
        await this.initialize();
        
        while (true) {
            await this.showMenu();
            const readline = require('readline');
            const rl = readline.createInterface({
                input: process.stdin,
                output: process.stdout
            });
            
            const choice = await this.question(rl, 'Enter your choice (1-11): ');
            rl.close();
            
            await this.handleChoice(choice);
        }
    }
}

// Run the database interaction tool
const tool = new DatabaseInteractionTool();
tool.run().catch(error => {
    console.error('Database interaction tool error:', error);
    process.exit(1);
});
EOF

    echo "Starting database interaction tool..."
    echo "Note: This will open an interactive Node.js session."
    echo ""
    
    # Run the database interaction tool
    node /tmp/db_interaction.js
    
    # Clean up temporary file
    rm -f /tmp/db_interaction.js
}

# Check if we're in the right directory
if [ ! -f "api/index.js" ]; then
    echo -e "${RED}Error: Please run this script from the musicPlayer directory${NC}"
    exit 1
fi

# Main menu loop
while true; do
    show_menu
    
    case $choice in
        1) run_database_tests ;;
        2) run_api_tests ;;
        3) run_frontend_tests ;;
        4) run_integration_tests ;;
        5) run_build_tests ;;
        6) run_all_tests ;;
        7) run_custom_tests ;;
        8) run_database_interaction ;;
        9) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice. Please try again."; continue ;;
    esac
    
    # Generate final report
    echo ""
    echo -e "${BLUE}======================================"
    echo -e "FINAL TEST REPORT"
    echo -e "======================================${NC}"
    echo -e "Total Tests: $TOTAL_TESTS"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    # Calculate success rate
    if [ $TOTAL_TESTS -gt 0 ]; then
        SUCCESS_RATE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
        echo -e "Success Rate: $SUCCESS_RATE%"
    else
        SUCCESS_RATE=0
        echo -e "Success Rate: 0%"
    fi
    
    echo ""
    
    # Show failed tests list if any
    if [ ${#FAILED_TEST_LIST[@]} -gt 0 ]; then
        echo -e "${RED}Failed Tests:${NC}"
        for failed_test in "${FAILED_TEST_LIST[@]}"; do
            echo -e "${RED}  - $failed_test${NC}"
        done
        echo ""
    fi
    
    # Final status
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}ALL TESTS PASSED!${NC}"
        echo -e "${GREEN}The music player integration is working correctly!${NC}"
    elif [ $SUCCESS_RATE -ge 80 ]; then
        echo -e "${YELLOW}Most tests passed. Some minor issues detected.${NC}"
        echo -e "${YELLOW}The music player should work with minor limitations.${NC}"
    else
        echo -e "${RED}Multiple test failures detected.${NC}"
        echo -e "${RED}Please review the failed tests and fix the issues.${NC}"
    fi
    
    echo ""
    echo -e "${BLUE}======================================${NC}"
    
    # Ask if user wants to run more tests
    echo ""
    read -p "Run more tests? (y/n): " continue_choice
    if [[ $continue_choice != "y" && $continue_choice != "Y" ]]; then
        break
    fi
    
    # Reset counters for next run
    TOTAL_TESTS=0
    PASSED_TESTS=0
    FAILED_TESTS=0
    FAILED_TEST_LIST=()
done
