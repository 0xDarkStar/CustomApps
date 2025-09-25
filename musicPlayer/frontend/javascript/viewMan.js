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
        window.electronAPI.onShowPlaylistModal(() => {
            if (this.indexController && this.indexController.showPlaylistModal) {
                this.indexController.showPlaylistModal();
            }
        });
        window.electronAPI.onShowSongModal(() => {
            if (this.indexController && this.indexController.showSongModal) {
                this.indexController.showSongModal();
            }
        });
        window.electronAPI.onRemovePlaylist(() => {
            if (this.currentController && this.currentController.removePlaylist) {
                this.currentController.removePlaylist();
            }
        });
        window.electronAPI.removeAllListeners(() => {
            this.indexController.cleanup();
            this.currentController.cleanup();
        })
    }

    navigateTo(viewName, params = {}) {
        // console.log(`I have been called!\n They want to go to ${viewName}.\nTheir parameters are:`, params);
        this.loadView(viewName, params);
    }
}

const viewManager = new ViewManager();
viewManager.loadView('grid');
viewManager.setupGlobalIPCListeners();