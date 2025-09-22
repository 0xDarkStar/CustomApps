const { contextBridge, ipcRenderer } = require('electron')

// Expose protected methods that allow the renderer process to use
// the ipcRenderer without exposing the entire object
contextBridge.exposeInMainWorld('electronAPI', {
    onShowPlaylistModal: (callback) => {
        ipcRenderer.on('show-playlist-modal', callback)
    },
    onRemovePlaylist: (callback) => {
        ipcRenderer.on('remove-playlist', callback)
    },
    removeAllListeners: (channel) => {
        ipcRenderer.removeAllListeners(channel)
    }
})
