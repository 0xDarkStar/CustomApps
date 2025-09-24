

async function removePlaylist(playlistId) {
    console.log(`Chosen Playlist: ${playlistId}`);
    if (playlistId === "All Songs") { // They need to see all their songs
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

function makeAllPlaylistsClickable() {
    const playlists = document.querySelectorAll('.playlist');
    playlists.forEach(playlist => {
        playlistId = playlist.id;
        nameElement = playlist.querySelector('p');
        if (nameElement) {
            playlistName = nameElement.textContent;
            makePlaylistClickable(playlist, playlistName, playlistId)
        }
    })
}

function makePlaylistClickable(playlistElement, playlistName, playlistId) {
    playlistElement.addEventListener('click', function() {
        // Go to the page with the ID and name
        sessionStorage.setItem("name", playlistName);
        sessionStorage.setItem("id", playlistId);
        window.location.href = `playlist.html`;
    })
}

async function loadPlaylists() {
    const playlists = await window.musicAPI.getAllPlaylists();

    playlists.forEach(playlist => {
        const name = playlist.title;
        const grid = document.getElementById('playlistGrid');
        const newPlaylist = document.createElement('div');
        newPlaylist.className = 'playlist';
        newPlaylist.id = playlist.id;
        newPlaylist.innerHTML = `<img src="../../data/thumbnails/no_image.svg">\n<p>${name}</p>`;
        
        grid.appendChild(newPlaylist); 
    });
    
    // Make all playlists clickable after they're loaded
    makeAllPlaylistsClickable();
}

// Wait for DOM to be fully loaded before adding event listeners
document.addEventListener('DOMContentLoaded', function() {
    loadPlaylists();
    if (window.electronAPI) {
        window.electronAPI.onRemovePlaylist((event, playlistId) => {
            removePlaylist(playlistId);
        });
    }
    document.getElementById('songCreateBtn').addEventListener('click', addSong);
});
