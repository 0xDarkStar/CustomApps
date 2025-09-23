const { contextBridge, ipcRenderer } = require('electron')

// Expose protected methods that allow the renderer process to use
// the ipcRenderer without exposing the entire object
contextBridge.exposeInMainWorld('electronAPI', {
    onShowPlaylistModal: (callback) => ipcRenderer.on('show-playlist-modal', callback),
    onRemovePlaylist: (callback) => ipcRenderer.on('remove-playlist', callback),
    onShowSongModal: (callback) => ipcRenderer.on('show-song-modal', callback),
    removeAllListeners: (channel) => ipcRenderer.removeAllListeners(channel)
})

contextBridge.exposeInMainWorld('musicAPI', {
    ping: () => ipcRenderer.invoke('ping'),
    /* Song Controls */
    addSong: (title, artist, album, length, path) => ipcRenderer.invoke('addSong', title, artist, album, length, path),
    updateSong: (songID, newTitle, newArtist, newAlbum) => ipcRenderer.invoke('updateSong', songID, newTitle, newArtist, newAlbum),
    deleteSong: (songID) => ipcRenderer.invoke('deleteSong', songID),
    getAllSongs: () => ipcRenderer.invoke('getAllSongs'),
    getSong: (songID) => ipcRenderer.invoke('getSong', songID),
    searchSongs: (searchStr) => ipcRenderer.invoke('searchSongs', searchStr),
    /* Playlist Controls */
    createPlaylist: (title) => ipcRenderer.invoke('createPlaylist', title),
    updatePlaylist: (playlistID, newTitle) => ipcRenderer.invoke('updatePlaylist', playlistID, newTitle),
    deletePlaylist: (playlistID) => ipcRenderer.invoke('deletePlaylist', playlistID),
    getAllPlaylists: () => ipcRenderer.invoke('getAllPlaylists'),
    getPlaylist: (playlistID) => ipcRenderer.invoke('getPlaylist', playlistID),
    /* Song to Playlist Connections */
    addSongToPlaylist: (songID, playlistID) => ipcRenderer.invoke('addSongToPlaylist', songID, playlistID),
    removeSongFromPlaylist: (songID, playlistID) => ipcRenderer.invoke('removeSongFromPlaylist', songID, playlistID),
    getSongsInPlaylist: (playlistID) => ipcRenderer.invoke('getSongsInPlaylist', playlistID),
    /* Extras */
    initializeAPI: () => ipcRenderer.invoke('initializeAPI'),
    isAPIinitialized: () => ipcRenderer.invoke('isAPIinitialized'),
    getDatabaseStats: () => ipcRenderer.invoke('getDatabaseStats')
})