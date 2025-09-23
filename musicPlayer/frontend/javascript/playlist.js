function getPlaylistFromURL() {
    const urlParams = new URLSearchParams(window.location.search);
    return {
        name: urlParams.get('name') || 'Unknown Playlist',
        id: urlParams.get('id') || 0
    };
}

function loadPlaylist() {
    const playlist = getPlaylistFromURL();
    const titleElement = document.getElementById('playlistTitle');
    titleElement.textContent = playlist.name;

    console.log(`Loading Playlist: ${playlist.name} (ID: ${playlist.id})`);
}

document.addEventListener('DOMContentLoaded', function() {
    loadPlaylist();
});
