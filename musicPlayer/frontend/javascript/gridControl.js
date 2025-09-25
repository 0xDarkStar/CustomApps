class GridController {
    constructor(viewManager) {
        this.viewManager = viewManager;
        this.initialize();
    }

    initialize() {
        this.loadPlaylists();
        this.setupEventListeners();
    }

    async loadPlaylists() {
        try {
            const playlists = await window.musicAPI.getAllPlaylists();
            this.displayPlaylists(playlists);
        } catch (error) {
            console.error('Error loading playlists:', error);
        }
    }

    displayPlaylists(playlists) {
        playlists.forEach(playlist => {
            // console.log(`Current playlist info:\nName: ${playlist.title}\nID: ${playlist.id}`);
            const name = playlist.title;
            const grid = document.getElementById('playlistGrid');
            const newPlaylist = document.createElement('div');
            newPlaylist.className = 'playlist';
            newPlaylist.id = playlist.id;
            newPlaylist.innerHTML = `<img src="../../data/thumbnails/no_image.svg">\n<p>${name}</p>`;
            
            grid.appendChild(newPlaylist); 
        });
        
        // Make playlists clickable after they're added to the DOM
        this.makeAllPlaylistsClickable();
        this.viewManager.indexController.setupEventListeners();
    }

    setupEventListeners() {
        // Set up Electron IPC listeners immediately (don't wait for DOM)
        if (window.electronAPI) {
            window.electronAPI.onRemovePlaylist((event, playlistId) => {
                this.removePlaylist(playlistId);
            });
        }
        
        // DOM-dependent listeners
        document.addEventListener('DOMContentLoaded', () => {
            document.getElementById('songCreateBtn').addEventListener('click', addSong);
        });
    }

    makeAllPlaylistsClickable() {
        const playlists = document.querySelectorAll('.playlist');
        playlists.forEach(playlist => {
            let playlistId = playlist.id;
            let nameElement = playlist.querySelector('p');
            if (nameElement) {
                let playlistName = nameElement.textContent;
                this.makePlaylistClickable(playlist, playlistName, playlistId)
            }
        })
    }
    
    makePlaylistClickable(playlistElement, playlistName, playlistId) {
        playlistElement.addEventListener('click', () => {
            // Go to the page with the ID and name
            this.viewManager.navigateTo('playlist', {id: playlistId, name: playlistName});
        })
    }

    async removePlaylist(playlistId) {
        console.log(`Chosen Playlist: ${playlistId}`);
        if (playlistId == 0) { // They need to see all their songs
            return;
        }
        // Find the playlist element by its text content (since we're using that as ID for now)
        const playlists = document.querySelectorAll('.playlist');
        for (let playlist of playlists) {
            const currPlaylistID = playlist.id;
            console.log(`Current Playlist ID: ${currPlaylistID}`);
            if (currPlaylistID === playlistId) {
                console.log(await window.musicAPI.deletePlaylist(playlistId));
                playlist.remove();
                break;
            }
        }
    }
}