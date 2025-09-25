class ViewManager {
    constructor() {
        this.currentView = null;
        this.indexController = new indexController(this);
        this.currentController = null;
        this.contentArea = document.getElementById('contentArea');
    }

    async loadView(viewName, params = {}) {
        if (this.currentController && this.currentController.cleanup) {
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

    navigateTo(viewName, params = {}) {
        // console.log(`I have been called!\n They want to go to ${viewName}.\nTheir parameters are:`, params);
        this.loadView(viewName, params);
    }
}

const viewManager = new ViewManager();
viewManager.loadView('grid');