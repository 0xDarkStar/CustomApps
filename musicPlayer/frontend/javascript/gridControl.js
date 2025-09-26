class GridController {
    constructor(viewManager) {
        this.viewManager = viewManager;
        this.abortController = new AbortController();
        this.initialize();
    }

    initialize() {
        this.loadPlaylists();
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
        this.setupEventListeners();
        this.viewManager.indexController.setupEventListeners();
    }

    setupEventListeners() {
        const signal = this.abortController.signal;
        // DOM-dependent listeners
        document.getElementById('songCreateBtn').addEventListener('click', () => {this.viewManager.indexController.addSong(), {signal}});
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
        if (playlistId == 0) { // They need to see all their songs
            return;
        }
        // Find the playlist element by its text content (since we're using that as ID for now)
        const playlists = document.querySelectorAll('.playlist');
        for (let playlist of playlists) {
            const currPlaylistID = playlist.id;
            if (currPlaylistID === playlistId) {
                console.log(await window.musicAPI.deletePlaylist(playlistId));
                playlist.remove();
                break;
            }
        }
    }

    cleanup() {
        this.abortController.abort();

        if (this.refreshTimer) {
            clearInterval(this.refreshTimer);
        }
    }
}