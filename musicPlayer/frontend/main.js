const {app, BrowserWindow, Menu, ipcMain} = require('electron')
// Commented out until NODE-MODULE-VERSIONs match
const musicAPI = require('./../api/index');

const createWindow = () => {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: true,
            preload: require('path').join(__dirname, 'javascript/preload.js')
        }
    })

    // Set Application Menu
    const appMenu = Menu.buildFromTemplate([
        // File Menu
        {
            label: 'File',
            submenu: [
                {
                    label: 'Add Playlist',
                    click: () => {
                        win.webContents.send('show-playlist-modal')
                    }
                },
                {
                    label: 'Add Song',
                    click: () => {
                        win.webContents.send('show-song-modal')
                    }
                },
                {
                    label: 'Add Subtitles',
                    click: () => {
                        win.webContents.send('show-subtitle-modal')
                    }
                },
                { type: 'separator' },
                { role: 'quit' }
            ]
        },
        // View Menu
        {
            label: 'View',
            submenu: [
                { role: 'reload' },
                { role: 'forceReload' },
                { role: 'toggleDevTools' }
            ]
        }
    ])
    win.setMenu(appMenu);
    
    // Context menu for playlist grid area
    const playlistMenu = Menu.buildFromTemplate([
        {
            label: 'Add Playlist',
            click: () => {
                win.webContents.send('show-playlist-modal')
            }
        },
        { type: 'separator'},
        { role: 'copy' }
    ])
    
    // Default context menu for editable areas
    const defaultMenu = Menu.buildFromTemplate([
        { role: 'copy' },
        { role: 'cut' },
        { role: 'paste' }
    ])
    
    win.webContents.on('context-menu', (_event, params) => {
        if (params.isEditable) {
            defaultMenu.popup()
        } else {
            // Check what was right-clicked and show appropriate menu
            win.webContents.executeJavaScript(`
                (function() {
                    const element = document.elementFromPoint(${params.x}, ${params.y});
                    const playlistGrid = element && element.closest('.playlist-grid');
                    const playlist = element && element.closest('.playlist');
                    
                    if (playlist) {
                        return { type: 'playlist', playlistId: playlist.id || playlist.textContent.trim() };
                    } else if (playlistGrid) {
                        return { type: 'grid' };
                    } else {
                        return { type: 'default' };
                    }
                })()
            `).then((result) => {
                if (result.type === 'playlist') {
                    // Show playlist-specific menu
                    const playlistMenu = Menu.buildFromTemplate([
                        {
                            label: 'Remove Playlist',
                            click: () => {
                                win.webContents.send('remove-playlist', result.playlistId)
                            }
                        },
                        { type: 'separator' },
                        { role: 'copy' }
                    ])
                    playlistMenu.popup()
                } else if (result.type === 'grid') {
                    // Show grid menu (add playlist)
                    playlistMenu.popup()
                } else {
                    // Show default menu
                    defaultMenu.popup()
                }
            }).catch(() => {
                // Fallback to default menu if there's an error
                defaultMenu.popup()
            })
        }
    })

    win.loadFile('html/index.html')
}

const fillerFunc = (returnType) => {
    switch (returnType) {
        case 'int':
            return 0;
        case 'bool':
            return false;
        case 'string':
            return 'You have recieved a string!';
        case 'songDetails':
            return { 'id': 0, 'title': 'Generic Song', 'artist': 'Generic Artist', 'album': 'Generic Album', 'length': 355, 'path': '~/generic/path/to/file.mp3'};
        case 'playlistDetails':
            return { 'id': 1, 'title': 'Generic Playlist', 'length': 0, 'numSongs': 0};
        case 'dbStats':
            return { 'totalSongs': 15, 'totalPlaylists': 2, 'totalSubtitles': 0, 'totalPlaylistSongs': 30};
        default:
            break;
    }
    return 'pong'
}

app.whenReady().then(() => {
    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
    /* API Calls */
    // Songs
    ipcMain.handle('addSong', async (event, title, artist, album, length, path) => {
        return await musicAPI.addSong(title, artist, album, length, path);
    });
    ipcMain.handle('updateSong', async (event, songID, newTitle, newArtist, newAlbum) => {
        return await musicAPI.updateSong(songID, newTitle, newArtist, newAlbum);
    });
    ipcMain.handle('removeSong', async (event, songID) => {
        return await musicAPI.deleteSong(songID);
    });
    ipcMain.handle('getAllSongs', async () => {
        return await musicAPI.getAllSongs();
    });
    ipcMain.handle('getSong', async (event, songID) => {
        return await musicAPI.getSong(songID);
    });
    ipcMain.handle('searchSongs', async (event, searchStr) => {
        return await musicAPI.searchSongs(searchStr);
    });

    // Playlists
    ipcMain.handle('createPlaylist', async (event, title) => {
        return await musicAPI.createPlaylist(title);
    });
    ipcMain.handle('updatePlaylist', async (event, playlistID, newTitle) => {
        return await musicAPI.updatePlaylist(playlistID, newTitle);
    });
    ipcMain.handle('deletePlaylist', async (event, playlistID) => {
        return await musicAPI.deletePlaylist(playlistID);
    });
    ipcMain.handle('getAllPlaylists', async () => {
        return await musicAPI.getAllPlaylists();
    });
    ipcMain.handle('getPlaylist', async (event, playlistID) => {
        return await musicAPI.getPlaylist(playlistID);
    });
    
    // Songs to Playlists
    ipcMain.handle('addSongToPlaylist', async (event, songID, playlistID) => {
        return await musicAPI.addSongToPlaylist(songID, playlistID);
    });
    ipcMain.handle('removeSongFromPlaylist', async (event, songID, playlistID) => {
        return await musicAPI.removeSongFromPlaylist(songID, playlistID);
    });
    ipcMain.handle('getSongsInPlaylist', async (event, playlistID) => {
        return await musicAPI.getSongsInPlaylist(playlistID);
    });
    
    // Extras
    ipcMain.handle('initializeAPI', async () => {
        return await musicAPI.initialize();
    });
    ipcMain.handle('isAPIinitialized', async () => {
        return await musicAPI.isInitialized();
    });
    ipcMain.handle('getDatabaseStats', async () => {
        return await musicAPI.getDatabaseStats();
    });
    musicAPI.initialize();
    createWindow()
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
})