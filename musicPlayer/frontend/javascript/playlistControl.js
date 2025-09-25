async function removeFromPlaylist(songID, playlistID) {
    const response = await window.musicAPI.removeSongFromPlaylist(songID, playlistID);
    console.log(response);
}

class PlaylistController {
    constructor(viewManager, params) {
        this.viewManager = viewManager;
        this.playlistID = params.id;
        this.playlistName = params.name;
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
        // console.log(`Loading Playlist: ${this.playlistName} (ID: ${this.playlistID})`);
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
                    <button class="menu-btn" onclick="toggleMenu(this)">
                        <img src="../icons/menu.svg">
                    </button>
                    <div class="menu-dropdown">
                        <div class="menu-item" onclick="addNext(${song.id})">Play Next</div>
                        <div class="menu-item" onclick="addToQueue(${song.id})">Add To Queue</div>
                        <div class="menu-item" onclick="removeFromPlaylist(${song.id}, ${this.playlistID});loadSongs(${this.playlistID})">Remove From This Playlist</div>
                        <div class="menu-item" onclick="viewSongDetails(${song.id})">Open Song Settings</div>
                    </div>
                </div>
                <input type="checkbox" id="${song.id}Select">
                <div class="song-item-length" id="${song.id}Length">
                    ${duration}
                </div>
            </div>
        `;
        songsContainer.appendChild(songItem);
    }

    formatDuration(seconds) {
        const minutes = Math.floor(seconds/60);
        const remainingSeconds = seconds%60;
        return `${minutes}:${remainingSeconds.toString().padStart(2,'0')}`
    }

    setupEventListeners() {
        document.getElementById('addSongBtn').addEventListener('click', () => this.viewManager.indexController.showSongModal(this.playlistID))
        document.getElementById('songCreateBtn-pl').addEventListener('click', () => this.viewManager.indexController.addSong(this.playlistID));
        document.getElementById('backBtn').addEventListener('click', () => this.viewManager.navigateTo('grid'));
    }
}