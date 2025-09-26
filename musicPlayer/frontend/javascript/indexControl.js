/* All shared functions go here */

class indexController {
    constructor(viewManager) {
        this.viewManager = viewManager;
        this.isExpanded = false;
        this.abortController = new AbortController();
        this.queue = [];
        this.currentQueueIndex = 0;
    }

    /* Control Bar Expansion */ 
    toggleExpanded() {
        const controlBar = document.querySelector('.control-bar');
        const expandBtn = document.querySelector('.expand-btn');
        const songView = document.getElementById('songControl');
        const queueView = document.getElementById('queue');
    
        this.isExpanded = !this.isExpanded;
    
        if (this.isExpanded) {
            controlBar.classList.add('expanded');
            expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Collapse" style="transform: rotate(180deg);">'
            songView.style.display = 'block';
            queueView.style.display = 'block';
        } else {
            controlBar.classList.remove('expanded');
            expandBtn.innerHTML = '<img src="../icons/dropdown.svg" alt="Expand">'
            songView.style.display = 'none';
            queueView.style.display = 'none';
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
        const songFile = document.getElementById('songFileInput').files[0];
        const songTitle = document.getElementById('songTitleInput').value.trim();
        const songArtist = document.getElementById('songArtistInput').value.trim();
        const songAlbum = document.getElementById('songAlbumInput').value.trim();

        const arrayBuffer = await songFile.arrayBuffer();

        const savedPath = await window.musicAPI.saveFile({
            fileName: songFile.name,
            buffer: arrayBuffer,
            targetDirectory: '../data/songs/'
        });
        const targetDirectory = '../../data/songs/' + songFile.name;
        // Read the file and get any data we can
        // May need to ask user for song info (title, artist, etc.)
        console.log(await window.musicAPI.addSong(songTitle, songArtist, songAlbum, 355, targetDirectory));
        this.hideSongModal();
    }

    async addSongPlaylist(playlistID) {
        const songFile = document.getElementById('songFileInput').files[0];
        const songTitle = document.getElementById('songTitleInput').value.trim();
        const songArtist = document.getElementById('songArtistInput').value.trim();
        const songAlbum = document.getElementById('songAlbumInput').value.trim();

        const arrayBuffer = await songFile.arrayBuffer();

        const savedPath = await window.musicAPI.saveFile({
            fileName: songFile.name,
            buffer: arrayBuffer,
            targetDirectory: '../data/songs/'
        });
        const targetDirectory = '../../data/songs/' + songFile.name;
        // Read the file and get any data we can
        // May need to ask user for song info (title, artist, etc.)
        console.log(await window.musicAPI.addSong(songTitle, songArtist, songAlbum, 355, targetDirectory));
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

    /* Queue Controls */
    addToQueue(song = {}) {
        // The song object must reflect:
        // {id: <song_id>, title: <song_title>, artist: <song_artist>, file: <file_path>}
        this.queue.push(song);
        if (this.queue.length === 1) {
            this.loadCurrentSong();
        }
    }
    
    async addPlaylistToQueue(playlistID) {
        let songs;
        if (playlistID == 0) {
            songs = await window.musicAPI.getAllSongs();
        } else {
            songs = await window.musicAPI.getSongsInPlaylist(playlistID);
        }
        const songsLength = Object.keys(songs).length;
        this.queue = [...songs];
        if (this.queue.length === songsLength) {
            this.loadCurrentSong();
        }
    }

    nextSong() {
        if (this.currentQueueIndex < this.queue.length-1) {
            this.currentQueueIndex++;
            this.loadCurrentSong();
        }
    }

    previousSong() {
        const songElement = document.getElementById('songControl');
        if (songElement.currentTime > 5) {
            songElement.currentTime = 0;
        } else if (this.currentQueueIndex > 0) {
            this.currentQueueIndex--;
            this.loadCurrentSong();
        }
    }

    /* Song Controls */
    loadCurrentSong() {
        let songTitle = document.getElementById('title');
        let songArtist = document.getElementById('artist');
        let songFile = document.getElementById('songControl');
        let currSong = this.queue[this.currentQueueIndex];
        if (!currSong) {
            currSong = {id:-1, title:'No Song', artist:'No Artist', path:''};
        }
        songTitle.textContent = currSong.title;
        songArtist.textContent = currSong.artist;
        songFile.src = currSong.path;
        this.isSongPaused();
    }

    pauseOrPlaySong() {
        const songElement = document.getElementById('songControl');
        const btnIcon = document.getElementById('playPauseIcon');
        if (!songElement.paused) {
            btnIcon.src = '../icons/play.svg';
            navigator.mediaSession.playbackState = 'paused';
            songElement.pause();
        } else {
            btnIcon.src = '../icons/pause.svg';
            songElement.play();
            navigator.mediaSession.playbackState = 'paused';
        }
    }

    isSongPaused() {
        const songElement = document.getElementById('songControl');
        const btnIcon = document.getElementById('playPauseIcon');
        const icon = btnIcon.src.split('/').pop();
        if (icon === 'pause.svg') {
            songElement.play();
        }
    }

    oneTimeSetupEventListeners() {
        const video = document.getElementById('songControl');
        const positionSlider = document.getElementById('video-position');
        const volume = document.getElementById('volume');
        const volumeIcon = document.getElementById('volumeIcon');
        try {
            navigator.mediaSession.setActionHandler('play', () => this.pauseOrPlaySong());
            navigator.mediaSession.setActionHandler('pause', () => this.pauseOrPlaySong());
            navigator.mediaSession.setActionHandler('nexttrack', () => this.nextSong());
            navigator.mediaSession.setActionHandler('previoustrack', () => this.previousSong());
        } catch (error) {
            console.log('No media keys for you! :p');
        }
        this.loadCurrentSong();
        document.addEventListener('DOMContentLoaded', () => {
            document.querySelector('.expand-btn').addEventListener('click', () => this.toggleExpanded());
            document.getElementById('previousBtn').addEventListener('click', () => this.previousSong());
            document.getElementById('playPauseBtn').addEventListener('click', () => this.pauseOrPlaySong());
            document.getElementById('nextBtn').addEventListener('click', () => this.nextSong());
            document.getElementById('volume').addEventListener('input', () => {
                video.volume = volume.value;
                if (volume.value > 0.8) {
                    volumeIcon.src = '../icons/volume_max.svg';
                } else if (volume.value > 0.4) {
                    volumeIcon.src = '../icons/volume_mid.svg';
                } else if (volume.value > 0) {
                    volumeIcon.src = '../icons/volume_low.svg';
                } else {
                    volumeIcon.src = '../icons/volume_muted.svg';
                }
            });
            // Song control over slider
            video.addEventListener('loadedmetadata', () => {
                video.volume = volume.value; // Set the correct volume
                positionSlider.max = video.duration;
            });
            video.addEventListener('timeupdate', () => {
                if (!positionSlider.dataset.dragging) {
                    positionSlider.value = video.currentTime;
                }
            });
            video.addEventListener('ended', () => {
                this.nextSong();
            })
            // Slider control over song
            positionSlider.addEventListener('input', () => {video.currentTime = positionSlider.value;});
            positionSlider.addEventListener('mousedown', () => {positionSlider.dataset.dragging = 'true';});
            positionSlider.addEventListener('mouseup', () => {delete positionSlider.dataset.dragging;});
        });
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