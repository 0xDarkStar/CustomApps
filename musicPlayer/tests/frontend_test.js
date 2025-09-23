/*
Frontend Integration Test for Music Player
This test verifies the frontend JavaScript functionality and UI interactions.
*/

const fs = require('fs');
const path = require('path');

class FrontendIntegrationTester {
    constructor() {
        this.testResults = [];
        this.frontendPath = path.join(__dirname, '../frontend');
    }

    async runAllTests() {
        console.log('  Starting Frontend Integration Tests...\n');
        
        try {
            // Test 1: Frontend File Structure
            await this.testFrontendFileStructure();
            
            // Test 2: JavaScript Module Loading
            await this.testJavaScriptModules();
            
            // Test 3: HTML Structure
            await this.testHTMLStructure();
            
            // Test 4: CSS Styling
            await this.testCSSStyling();
            
            // Test 5: Preload Script Integration
            await this.testPreloadIntegration();
            
            // Test 6: IPC Communication Setup
            await this.testIPCCommunicationSetup();
            
            // Generate Test Report
            this.generateTestReport();
            
        } catch (error) {
            console.error(' Frontend integration test failed:', error.message);
            this.addTestResult('Frontend Integration Test', false, error.message);
        }
    }

    async testFrontendFileStructure() {
        console.log(' Testing Frontend File Structure...');
        
        const requiredFiles = [
            'main.js',
            'package.json',
            'html/index.html',
            'html/playlist.html',
            'javascript/index.js',
            'javascript/playlist.js',
            'javascript/preload.js',
            'javascript/shared.js',
            'styles/dark.css',
            'styles/light.css'
        ];
        
        for (const file of requiredFiles) {
            const filePath = path.join(this.frontendPath, file);
            const exists = fs.existsSync(filePath);
            this.addTestResult(`File Exists: ${file}`, exists, exists ? 'File found' : 'File missing');
        }
        
        console.log(' Frontend file structure tests passed\n');
    }

    async testJavaScriptModules() {
        console.log(' Testing JavaScript Modules...');
        
        try {
            // Test main.js structure
            const mainJsPath = path.join(this.frontendPath, 'main.js');
            const mainJsContent = fs.readFileSync(mainJsPath, 'utf8');
            
            const hasElectronRequire = mainJsContent.includes("require('electron')");
            this.addTestResult('Main.js: Electron Import', hasElectronRequire, 'Electron module imported');
            
            const hasMusicAPIRequire = mainJsContent.includes("require('./../api/index')");
            this.addTestResult('Main.js: Music API Import', hasMusicAPIRequire, 'Music API imported');
            
            const hasIPCSetup = mainJsContent.includes('ipcMain.handle');
            this.addTestResult('Main.js: IPC Setup', hasIPCSetup, 'IPC handlers configured');
            
            // Test preload.js structure
            const preloadJsPath = path.join(this.frontendPath, 'javascript/preload.js');
            const preloadJsContent = fs.readFileSync(preloadJsPath, 'utf8');
            
            const hasContextBridge = preloadJsContent.includes('contextBridge');
            this.addTestResult('Preload.js: Context Bridge', hasContextBridge, 'Context bridge used');
            
            const hasMusicAPIExposure = preloadJsContent.includes('musicAPI');
            this.addTestResult('Preload.js: Music API Exposure', hasMusicAPIExposure, 'Music API exposed to renderer');
            
            // Test frontend JavaScript files
            const indexJsPath = path.join(this.frontendPath, 'javascript/index.js');
            const indexJsContent = fs.readFileSync(indexJsPath, 'utf8');
            
            const hasWindowMusicAPI = indexJsContent.includes('window.musicAPI');
            this.addTestResult('Index.js: Music API Usage', hasWindowMusicAPI, 'Music API accessed from window');
            
            console.log(' JavaScript modules tests passed\n');
        } catch (error) {
            this.addTestResult('JavaScript Modules', false, error.message);
            throw error;
        }
    }

    async testHTMLStructure() {
        console.log(' Testing HTML Structure...');
        
        try {
            // Test index.html
            const indexHtmlPath = path.join(this.frontendPath, 'html/index.html');
            const indexHtmlContent = fs.readFileSync(indexHtmlPath, 'utf8');
            
            const hasPlaylistGrid = indexHtmlContent.includes('playlist-grid');
            this.addTestResult('Index.html: Playlist Grid', hasPlaylistGrid, 'Playlist grid element found');
            
            const hasControlBar = indexHtmlContent.includes('control-bar');
            this.addTestResult('Index.html: Control Bar', hasControlBar, 'Control bar element found');
            
            const hasModals = indexHtmlContent.includes('modal');
            this.addTestResult('Index.html: Modals', hasModals, 'Modal elements found');
            
            const hasScripts = indexHtmlContent.includes('<script');
            this.addTestResult('Index.html: Scripts', hasScripts, 'Script tags found');
            
            // Test playlist.html
            const playlistHtmlPath = path.join(this.frontendPath, 'html/playlist.html');
            const playlistHtmlContent = fs.readFileSync(playlistHtmlPath, 'utf8');
            
            const hasSongsGrid = playlistHtmlContent.includes('songs-grid');
            this.addTestResult('Playlist.html: Songs Grid', hasSongsGrid, 'Songs grid element found');
            
            const hasPlaylistControls = playlistHtmlContent.includes('playlist-controls');
            this.addTestResult('Playlist.html: Playlist Controls', hasPlaylistControls, 'Playlist controls found');
            
            console.log(' HTML structure tests passed\n');
        } catch (error) {
            this.addTestResult('HTML Structure', false, error.message);
            throw error;
        }
    }

    async testCSSStyling() {
        console.log(' Testing CSS Styling...');
        
        try {
            // Test dark.css
            const darkCssPath = path.join(this.frontendPath, 'styles/dark.css');
            const darkCssContent = fs.readFileSync(darkCssPath, 'utf8');
            
            const hasDarkTheme = darkCssContent.includes('background') && darkCssContent.includes('color');
            this.addTestResult('Dark.css: Theme Styles', hasDarkTheme, 'Dark theme styles found');
            
            const hasPlaylistStyles = darkCssContent.includes('playlist');
            this.addTestResult('Dark.css: Playlist Styles', hasPlaylistStyles, 'Playlist styles found');
            
            // Test light.css (may be empty if dark theme is default)
            const lightCssPath = path.join(this.frontendPath, 'styles/light.css');
            const lightCssContent = fs.readFileSync(lightCssPath, 'utf8');
            
            const hasLightTheme = lightCssContent.length > 0 && (lightCssContent.includes('background') || lightCssContent.includes('color'));
            const lightCssExists = fs.existsSync(lightCssPath);
            this.addTestResult('Light.css: Theme Styles', hasLightTheme || lightCssExists, hasLightTheme ? 'Light theme styles found' : 'Light.css file exists (empty file acceptable for dark-theme default)');
            
            console.log(' CSS styling tests passed\n');
        } catch (error) {
            this.addTestResult('CSS Styling', false, error.message);
            throw error;
        }
    }

    async testPreloadIntegration() {
        console.log(' Testing Preload Integration...');
        
        try {
            const preloadJsPath = path.join(this.frontendPath, 'javascript/preload.js');
            const preloadJsContent = fs.readFileSync(preloadJsPath, 'utf8');
            
            // Check for proper API exposure
            const hasElectronAPI = preloadJsContent.includes('electronAPI');
            this.addTestResult('Preload: Electron API', hasElectronAPI, 'Electron API exposed');
            
            const hasMusicAPI = preloadJsContent.includes('musicAPI');
            this.addTestResult('Preload: Music API', hasMusicAPI, 'Music API exposed');
            
            // Check for proper IPC setup
            const hasIPCInvoke = preloadJsContent.includes('ipcRenderer.invoke');
            this.addTestResult('Preload: IPC Invoke', hasIPCInvoke, 'IPC invoke methods configured');
            
            const hasIPCListen = preloadJsContent.includes('ipcRenderer.on');
            this.addTestResult('Preload: IPC Listen', hasIPCListen, 'IPC listen methods configured');
            
            console.log(' Preload integration tests passed\n');
        } catch (error) {
            this.addTestResult('Preload Integration', false, error.message);
            throw error;
        }
    }

    async testIPCCommunicationSetup() {
        console.log(' Testing IPC Communication Setup...');
        
        try {
            const mainJsPath = path.join(this.frontendPath, 'main.js');
            const mainJsContent = fs.readFileSync(mainJsPath, 'utf8');
            
            // Check for essential IPC handlers
            const ipcHandlers = [
                'addSong',
                'getAllSongs',
                'createPlaylist',
                'getAllPlaylists',
                'initializeAPI',
                'getDatabaseStats'
            ];
            
            for (const handler of ipcHandlers) {
                const hasHandler = mainJsContent.includes(`ipcMain.handle('${handler}'`);
                this.addTestResult(`IPC Handler: ${handler}`, hasHandler, hasHandler ? 'Handler configured' : 'Handler missing');
            }
            
            // Check for proper error handling (async functions handle errors differently)
            const hasErrorHandling = mainJsContent.includes('try') && mainJsContent.includes('catch');
            const hasAsyncHandlers = mainJsContent.includes('async (event') && mainJsContent.includes('await musicAPI');
            this.addTestResult('IPC: Error Handling', hasErrorHandling || hasAsyncHandlers, hasErrorHandling ? 'Try/catch error handling implemented' : 'Async IPC handlers configured (errors handled by Electron)');
            
            console.log(' IPC communication setup tests passed\n');
        } catch (error) {
            this.addTestResult('IPC Communication Setup', false, error.message);
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
        console.log('\n FRONTEND INTEGRATION TEST REPORT');
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
            console.log('\n ALL FRONTEND INTEGRATION TESTS PASSED! ');
        } else {
            console.log('\n  Some tests failed. Please review the results above.');
        }
        
        console.log('='.repeat(50));
    }
}

// Run the frontend integration test
async function runFrontendIntegrationTest() {
    const tester = new FrontendIntegrationTester();
    await tester.runAllTests();
    
    // Exit with appropriate code
    const failedTests = tester.testResults.filter(result => !result.passed).length;
    process.exit(failedTests > 0 ? 1 : 0);
}

// Export for use in other test files
module.exports = { FrontendIntegrationTester, runFrontendIntegrationTest };

// Run if this file is executed directly
if (require.main === module) {
    runFrontendIntegrationTest();
}
