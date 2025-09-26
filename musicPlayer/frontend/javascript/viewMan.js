class ViewManager {
    constructor() {
        this.currentView = null;
        this.indexController = new indexController(this);
        this.currentController = null;
        this.contentArea = document.getElementById('contentArea');
        this.indexController.oneTimeSetupEventListeners();
    }

    async loadView(viewName, params = {}) {
        if (this.currentController) {
            this.currentController.cleanup();
        }

        switch (viewName) {
            case 'grid':
                await this.loadGridView();
                this.currentController = new GridController(this);
                break;
            case 'playlist':
                await this.loadPlaylistView();
                this.currentController = new PlaylistController(this, params);
                break;
        }
        this.currentView = viewName;
    }

    async loadGridView() {
        const response = await fetch('grid.html');
        const html = await response.text();
        this.contentArea.innerHTML = html;
    }

    async loadPlaylistView() {
        const response = await fetch('playlist.html');
        const html = await response.text();
        this.contentArea.innerHTML = html;
    }

    setupGlobalIPCListeners() {
        window.electronAPI.onShowPlaylistModal(() => this.indexController.showPlaylistModal());
        window.electronAPI.onShowSongModal(() => this.indexController.showSongModal());
        window.electronAPI.onAddPlaylistToQueue((event, playlistID) => this.indexController.addPlaylistToQueue(playlistID));
        window.electronAPI.onRemovePlaylist((event, playlistID) => {
            if (this.currentController && this.currentController.removePlaylist) {
                this.currentController.removePlaylist(playlistID);
            }
        });
        window.electronAPI.removeAllListeners(() => {
            this.indexController.cleanup();
            this.currentController.cleanup();
        });
    }

    navigateTo(viewName, params = {}) {
        this.loadView(viewName, params);
    }
}

const viewManager = new ViewManager();
viewManager.loadView('grid');
viewManager.setupGlobalIPCListeners();