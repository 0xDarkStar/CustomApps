/* All shared functions go here */

class indexController {
    constructor(viewManager) {
        this.viewManager = viewManager;
        this.isExpanded = false;
        this.abortController = new AbortController();
    }

    /* Control Bar Expansion */ 
    toggleExpanded() {
        const controlBar = document.querySelector('.control-bar');
        const expandBtn = document.querySelector('.expand-btn');
        const songView = document.getElementById('songControl');
    
        this.isExpanded = !this.isExpanded;
    
        if (this.isExpanded) {
            controlBar.classList.add('expanded');
            expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Collapse" style="transform: rotate(180deg);">'
            songView.style.display = 'block';
        } else {
            controlBar.classList.remove('expanded');
            expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Expand">'
            songView.style.display = 'none';
        }
    }

    /* Show Dropdown Menu */
    toggleMenu(button) {
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

    /* Song Modal */
    showSongModal() {
        const modal = document.getElementById('songModal');
        modal.style.display = 'block';
    }

    hideSongModal() {
        const modal = document.getElementById('songModal');
        modal.style.display = 'none';
    }

    async addSong() {
        // const songFile = document.getElementById('songFileInput');
        // Read the file and get any data we can
        // May need to ask user for song info (title, artist, etc.)
        console.log(await window.musicAPI.addSong("Generic Title", "Generic Artist", "Generic Album", 355, "~/generic/path/to/file.mp3"));
        hideSongModal();
    }

    async addSong(playlistID) {
        console.log(`Add Song was called with the playlist ID: ${playlistID}`);
        // const songFile = document.getElementById('songFileInput');
        // Read the file and get any data we can
        // May need to ask user for song info (title, artist, etc.)
        const response = await window.musicAPI.addSong("Generic Title", "Generic Artist", "Generic Album", 355, "~/generic/path/to/file.mp3");
        console.log(response);
        if (playlistID != 0) {
            console.log(await window.musicAPI.addSongToPlaylist(response.id, playlistID));
        }
        this.viewManager.currentController.loadSongs(playlistID);
        this.hideSongModal();
    }

    /* Playlist Modal */
    showPlaylistModal() {
        const modal = document.getElementById('playlistModal');
        const input = document.getElementById('playlistNameInput');
        
        modal.style.display = 'block';
        input.value = '';
        input.focus();
    }

    hidePlaylistModal() {
        const modal = document.getElementById('playlistModal');
        modal.style.display = 'none';
    }

    /* Create a playlist */
    async createPlaylist() {
        // Make sure to change to code to use the API instead of messing with the HTML
        const input = document.getElementById('playlistNameInput');
        const name = input.value.trim();
        
        if (!name) return;
        const response = await window.musicAPI.createPlaylist(name);
        console.log(response);
        const grid = document.getElementById('playlistGrid');
        const newPlaylist = document.createElement('div');
        newPlaylist.className = 'playlist';
        newPlaylist.id = response.id;
        newPlaylist.innerHTML = `<img src="../../data/thumbnails/no_image.svg">\n<p>${name}</p>`;
        
        grid.appendChild(newPlaylist);
        newPlaylist.addEventListener('click', () => {
            this.viewManager.navigateTo('playlist', {id: response.id, name: response.name});
        })
        this.hidePlaylistModal();
    }

    /* Song Info */
    updateSongDetails() {
        const songTitle = document.getElementById('title');
        const songArtist = document.getElementById('artist');
        songTitle.textContent = "Generic Title";
        songArtist.textContent = "Generic Artist";
    }

    /* Song Controls */

    pauseOrPlaySong() {
        const songElement = document.getElementById('songControl');
        if (!songElement.paused) {
            songElement.pause();
        } else {
            songElement.play();
        }
    }

    oneTimeSetupEventListeners() {
        const video = document.getElementById('songControl');
        const positionSlider = document.getElementById('video-position');
        const volume = document.getElementById('volume');
        document.addEventListener('DOMContentLoaded', () => {
            document.querySelector('.expand-btn').addEventListener('click', () => this.toggleExpanded());
            document.getElementById('playPauseBtn').addEventListener('click', () => this.pauseOrPlaySong());
            document.getElementById('volume').addEventListener('input', () => {video.volume = volume.value});
            // Song control over slider
            video.addEventListener('loadedmetadata', () => {
                this.updateSongDetails();
                positionSlider.max = video.duration;
                volume.value = video.volume
            });
            video.addEventListener('timeupdate', () => {
                if (!positionSlider.dataset.dragging) {
                    positionSlider.value = video.currentTime;
                }
            });
            // Slider control over song
            positionSlider.addEventListener('input', () => {video.currentTime = positionSlider.value;});
            positionSlider.addEventListener('mousedown', () => {positionSlider.dataset.dragging = 'true';});
            positionSlider.addEventListener('mouseup', () => {delete positionSlider.dataset.dragging;});
        })
    }

    setupEventListeners() {
        const signal = this.abortController.signal;
        // Playlist Modal Buttons
        document.getElementById('playlistCancelBtn').addEventListener('click', () => this.hidePlaylistModal(), {signal});
        document.getElementById('playlistCreateBtn').addEventListener('click', () => this.createPlaylist(), {signal});
        // Song Modal Buttons
        document.getElementById('songCancelBtn').addEventListener('click', () => this.hideSongModal(), {signal});
        // Close modal when clicking outside of it
        // document.getElementById('playlistModal').addEventListener('click', function(e) {
        //     if (e.target === this) {
        //         this.hidePlaylistModal();
        //     }
        // }.bind(this));
        // document.getElementById('songModal').addEventListener('click', function(e) {
        //     if (e.target === this) {
        //         this.hideSongModal();
        //     }
        // }.bind(this));
        
        // Allow Enter key to create playlist
        document.getElementById('playlistNameInput').addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.createPlaylist();
            }
        });
    
        // Open the dropdown menu when clicked
        document.addEventListener('click', function(event) {
            if(!event.target.closest('.song-menu')) {
                document.querySelectorAll('.menu-dropdown.show').forEach(menu => {
                    menu.classList.remove('show');
                });
            }
        }, {signal});
    }

    cleanup() {
        this.abortController.abort();

        if (this.refreshTimer) {
            clearInterval(this.refreshTimer);
        }
    }
}