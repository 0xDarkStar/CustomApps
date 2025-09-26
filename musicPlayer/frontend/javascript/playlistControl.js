

class PlaylistController {
    constructor(viewManager, params) {
        this.viewManager = viewManager;
        this.playlistID = params.id;
        this.playlistName = params.name;
        this.abortController = new AbortController();
        this.initialize();
    }

    initialize() {
        this.setTitle();
        this.loadSongs();
        this.setupEventListeners();
    }

    setTitle() {
        const titleElement = document.getElementById('playlistTitle');
        titleElement.textContent = this.playlistName;
    }

    async removeFromPlaylist(songID, playlistID) {
        const response = await window.musicAPI.removeSongFromPlaylist(songID, playlistID);
        console.log(response);
    }

    async loadSongs() {
        let songs;
        if (this.playlistID == 0) {
            songs = await window.musicAPI.getAllSongs();
        } else {
            songs = await window.musicAPI.getSongsInPlaylist(this.playlistID);
        }

        const songsContainer = document.getElementById("songsGrid");
        songsContainer.innerHTML = ''; // Clear existing songs

        songs.forEach(song => {
            this.createSongElement(song);
        });
        this.viewManager.indexController.setupEventListeners();
    }

    createSongElement(song) {
        const songsContainer = document.getElementById('songsGrid');

        const songItem = document.createElement('div');
        songItem.className = 'song-item';
        songItem.id = song.id;
        let duration = this.formatDuration(song.length);

        songItem.innerHTML = `
            <div class="song-item-thumbnail">
                <img src="../../data/thumbnails/no_image.svg">
            </div>
            <div class="song-item-details">
                <div class="song-item-title" id="${song.id}Title">${song.title}</div>
                <div class="song-item-artist" id="${song.id}Artist">${song.artist}</div>
            </div>
            <div class="song-item-controls">
                <div class="song-menu">
                    <button class="menu-btn" id="${song.id}Menu">
                        <img src="../icons/menu.svg">
                    </button>
                    <div class="menu-dropdown">
                        <div class="menu-item" id="${song.id}AddNext">Play Next</div>
                        <div class="menu-item" id="${song.id}AddQueue">Add To Queue</div>
                        <div class="menu-item" id="${song.id}Remove">Remove From This Playlist</div>
                        <div class="menu-item" id="${song.id}View">Open Song Settings</div>
                    </div>
                </div>
                <input type="checkbox" id="${song.id}Select">
                <div class="song-item-length" id="${song.id}Length">
                    ${duration}
                </div>
            </div>
        `;
        songsContainer.appendChild(songItem);
        let songMenu = document.getElementById(`${song.id}Menu`);
        let songQueue = document.getElementById(`${song.id}AddQueue`);
        let songRemove = document.getElementById(`${song.id}Remove`);
        songMenu.addEventListener('click', () => this.viewManager.indexController.toggleMenu(songMenu));
        songQueue.addEventListener('click', () => {
            this.viewManager.indexController.addToQueue({id: song.id, title: song.title, artist: song.artist, path: song.path});
            this.viewManager.indexController.toggleMenu(songMenu);
        });
        songRemove.addEventListener('click', () => {
            this.removeFromPlaylist(song.id, this.playlistID);
            this.loadSongs();
        })
    }

    formatDuration(seconds) {
        const minutes = Math.floor(seconds/60);
        const remainingSeconds = seconds%60;
        return `${minutes}:${remainingSeconds.toString().padStart(2,'0')}`
    }

    setupEventListeners() {
        const signal = this.abortController.signal;
        document.getElementById('addSongBtn').addEventListener('click', () => this.viewManager.indexController.showSongModal(this.playlistID), {signal});
        document.getElementById('songCreateBtn-pl').addEventListener('click', () => this.viewManager.indexController.addSongPlaylist(this.playlistID), {signal});
        document.getElementById('backBtn').addEventListener('click', () => this.viewManager.navigateTo('grid'), {signal});
    }

    cleanup() {
        this.abortController.abort();

        if (this.refreshTimer) {
            clearInterval(this.refreshTimer);
        }
    }
}