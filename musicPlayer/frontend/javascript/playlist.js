function getPlaylistFromStorage() {
    const playlistName = sessionStorage.getItem("name");
    const playlistID = sessionStorage.getItem("id");
    console.log(`Recieved playlist name: ${playlistName}`);
    console.log(`Recieved playlist ID: ${playlistID}`);
    return {
        name: playlistName || 'Unknown Playlist',
        id: playlistID || 0
    };
}

function loadPlaylist() {
    const playlist = getPlaylistFromStorage();
    const titleElement = document.getElementById('playlistTitle');
    titleElement.textContent = playlist.name;

    console.log(`Loading Playlist: ${playlist.name} (ID: ${playlist.id})`);
    loadAllSongs(playlist.id);
    document.getElementById('songCreateBtn-pl').addEventListener('click', () => addSong(playlist.id));
}

async function removeFromPlaylist(songID, playlistID) {
    const response = await window.musicAPI.removeSongFromPlaylist(songID, playlistID);
    console.log(response);
}

async function loadAllSongs(playlistID) {
    let songs;
    if (playlistID == 0) {
        songs = await window.musicAPI.getAllSongs();
    } else {
        songs = await window.musicAPI.getSongsInPlaylist(playlistID);
    }

    const songsContainer = document.getElementById("songsGrid");
    songsContainer.innerHTML = ''; // Clear existing songs

    songs.forEach(song => {
        createSongElement(song, playlistID);
    });
}

function createSongElement(song, playlistID) {
    const songsContainer = document.getElementById('songsGrid');

    const songItem = document.createElement('div');
    songItem.className = 'song-item';
    songItem.id = song.id;
    duration = formatDuration(song.length);

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
                    <div class="menu-item" onclick="removeFromPlaylist(${song.id}, ${playlistID});loadAllSongs(${playlistID})">Remove From This Playlist</div>
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

function formatDuration(seconds) {
    const minutes = Math.floor(seconds/60);
    const remainingSeconds = seconds%60;
    return `${minutes}:${remainingSeconds.toString().padStart(2,'0')}`
}

document.addEventListener('DOMContentLoaded', function() {
    loadPlaylist();
});
