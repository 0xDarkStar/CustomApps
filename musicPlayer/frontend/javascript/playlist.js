let isExpanded = false;

function toggleExpanded() {
    const controlBar = document.querySelector('.control-bar');
    const expandBtn = document.querySelector('.expand-btn');

    isExpanded = !isExpanded;

    if (isExpanded) {
        controlBar.classList.add('expanded');
        expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Collapse" style="transform: rotate(180deg);">'
    } else {
        controlBar.classList.remove('expanded');
        expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Expand">'
    }
}

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
    document.querySelector('.expand-btn').addEventListener('click', toggleExpanded);

    loadPlaylist();
});