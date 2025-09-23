const {app, BrowserWindow, Menu, ipcMain} = require('electron')

const createWindow = () => {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: false,
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

app.whenReady().then(() => {
    createWindow()

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
})