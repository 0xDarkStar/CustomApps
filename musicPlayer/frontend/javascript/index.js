

function removePlaylist(playlistId) {
    if (playlistId === "All Songs") { // They need to see all their songs
        return;
    }
    // Find the playlist element by its text content (since we're using that as ID)
    const playlists = document.querySelectorAll('.playlist');
    for (let playlist of playlists) {
        const playlistName = playlist.querySelector('p').textContent.trim();
        if (playlistName === playlistId) {
            playlist.remove();
            break;
        }
    }
}

function makeAllPlaylistsCliclable() {
    const playlists = document.querySelectorAll('.playlist');
    playlists.forEach(playlist => {
        const nameElement = playlist.querySelector('p');
        if (nameElement) {
            const playlistName = nameElement.textContent;
            makePlaylistClickable(playlist, playlistName)
        }
    })
}

function makePlaylistClickable(playlistElement, playlistName) {
    playlistElement.addEventListener('click', function() {
        // Get the playlist ID
        const playlistID = Date.now();

        // Go to the page with the ID and name
        window.location.href = `playlist.html?name=${encodeURIComponent(playlistName)}&id=${playlistID}`;
    })
}

// Wait for DOM to be fully loaded before adding event listeners
document.addEventListener('DOMContentLoaded', function() {
    makeAllPlaylistsCliclable();
    if (window.electronAPI) {
        window.electronAPI.onRemovePlaylist((event, playlistId) => {
            removePlaylist(playlistId);
        });
    }
});
