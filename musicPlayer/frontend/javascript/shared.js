/* All shared functions go here */
/* Control Bar expansion */
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

/* Song Modal */
function showSongModal() {
    const modal = document.getElementById('songModal');
    modal.style.display = 'block';
}

function hideSongModal() {
    const modal = document.getElementById('songModal');
    modal.style.display = 'none';
}

async function addSong() {
    const songFile = document.getElementById('songFileInput');
    // Read the file and get any data we can
    // May need to ask user for song info (title, artist, etc.)
    console.log(await window.musicAPI.addSong("Generic Title", "Generic Artist", "Generic Album", 355, "~/generic/path/to/file.mp3"));
}

/* Playlist Modal */
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

/* Create a playlist */
async function createPlaylist() {
    // Make sure to change to code to use the API instead of messing with the HTML
    const input = document.getElementById('playlistNameInput');
    const name = input.value.trim();
    
    if (!name) return;
    console.log(await window.musicAPI.createPlaylist(name));
    
    const grid = document.getElementById('playlistGrid');
    const newPlaylist = document.createElement('div');
    newPlaylist.className = 'playlist';
    newPlaylist.innerHTML = `<p>${name}</p>`;
    
    grid.appendChild(newPlaylist);
    hidePlaylistModal();
}

/* Show Dropdown Menu */
function toggleMenu(button) {
    const dropdown = button.nextElementSibling;

    // Close all other open menus
    document.querySelectorAll('.menu-dropdown.show').forEach(menu => {
        if (menu !== dropdown) {
            menu.classList.remove('show');
        }
    });

    // Toggle the menu
    dropdown.classList.toggle('show');
}

// Wait for DOM to be fully loaded before adding event listeners
document.addEventListener('DOMContentLoaded', function() {
    document.querySelector('.expand-btn').addEventListener('click', toggleExpanded);
    // Playlist Modal Buttons
    document.getElementById('playlistCancelBtn').addEventListener('click', hidePlaylistModal);
    document.getElementById('playlistCreateBtn').addEventListener('click', createPlaylist);
    // Song Modal Buttons
    document.getElementById('songCancelBtn').addEventListener('click', hideSongModal);
    document.getElementById('songCreateBtn').addEventListener('click', addSong);
    
    // Close modal when clicking outside of it
    document.getElementById('playlistModal').addEventListener('click', function(e) {
        if (e.target === this) {
            hidePlaylistModal();
        }
    });
    document.getElementById('songModal').addEventListener('click', function(e) {
        if (e.target === this) {
            hideSongModal();
        }
    });
    
    // Allow Enter key to create playlist
    document.getElementById('playlistNameInput').addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            createPlaylist();
        }
    });

    // Open the dropdown menu when clicked
    document.addEventListener('click', function(event) {
        if(!event.target.closest('.song-menu')) {
            document.querySelectorAll('.menu-dropdown.show').forEach(menu => {
                menu.classList.remove('show');
            });
        }
    });
    
    // Listen for context menu events from Electron
    if (window.electronAPI) {
        window.electronAPI.onShowPlaylistModal(() => {
            showPlaylistModal();
        });
        window.electronAPI.onShowSongModal(() => {
            showSongModal();
        });
    }
});
