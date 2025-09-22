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

function showPlaylistModal() {
    const modal = document.getElementById('playlistModal');
    const input = document.getElementById('playlistNameInput');
    
    modal.style.display = 'block';
    input.value = '';
    input.focus();
}

function hidePlaylistModal() {
    const modal = document.getElementById('playlistModal');
    modal.style.display = 'none';
}

function createPlaylist() {
    const input = document.getElementById('playlistNameInput');
    const name = input.value.trim();
    
    if (!name) return;
    
    const grid = document.getElementById('playlistGrid');
    const newPlaylist = document.createElement('div');
    newPlaylist.className = 'playlist';
    newPlaylist.innerHTML = `<p>${name}</p>`;
    
    grid.appendChild(newPlaylist);
    hidePlaylistModal();
}

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

// Wait for DOM to be fully loaded before adding event listeners
document.addEventListener('DOMContentLoaded', function() {
    document.querySelector('.expand-btn').addEventListener('click', toggleExpanded);
    document.getElementById('cancelBtn').addEventListener('click', hidePlaylistModal);
    document.getElementById('createBtn').addEventListener('click', createPlaylist);
    
    // Close modal when clicking outside of it
    document.getElementById('playlistModal').addEventListener('click', function(e) {
        if (e.target === this) {
            hidePlaylistModal();
        }
    });
    
    // Allow Enter key to create playlist
    document.getElementById('playlistNameInput').addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            createPlaylist();
        }
    });
    
    // Listen for context menu events from Electron
    if (window.electronAPI) {
        window.electronAPI.onShowPlaylistModal(() => {
            showPlaylistModal();
        });
        
        window.electronAPI.onRemovePlaylist((event, playlistId) => {
            removePlaylist(playlistId);
        });
    }
});
