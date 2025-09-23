/*
Made By: 0xDarkStar
This file provides a Node.js native module interface for the MusicAPI.
It allows Electron to call the C++ backend functions.
*/
#include <node.h>
#include <v8.h>
#include <nan.h>
#include "musicAPI.h"
#include <string>
#include <vector>

using namespace v8;

// Helper function to convert std::string to v8::String
Local<String> stringToV8(const std::string& str) {
    return Nan::New<String>(str.c_str()).ToLocalChecked();
}

// Helper function to convert v8::String to std::string
std::string v8ToString(Local<String> str) {
    Nan::Utf8String utf8(str);
    return std::string(*utf8);
}

// Helper function to get context
Local<Context> getContext() {
    return Nan::GetCurrentContext();
}

// Initialize the API
NAN_METHOD(Initialize) {
    try {
        bool result = MusicAPI::initialize();
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Check if API is initialized
NAN_METHOD(IsInitialized) {
    bool result = MusicAPI::isInitialized();
    info.GetReturnValue().Set(Nan::New<Boolean>(result));
}

// Shutdown the API
NAN_METHOD(Shutdown) {
    MusicAPI::shutdown();
    info.GetReturnValue().SetUndefined();
}

// Add a song
NAN_METHOD(AddSong) {
    if (info.Length() < 5) {
        Nan::ThrowError("AddSong requires 5 arguments: title, artist, album, length, path");
        return;
    }
    
    try {
        Local<Context> context = getContext();
        std::string title = v8ToString(info[0]->ToString(getContext()).ToLocalChecked());
        std::string artist = v8ToString(info[1]->ToString(getContext()).ToLocalChecked());
        std::string album = v8ToString(info[2]->ToString(getContext()).ToLocalChecked());
        int length = info[3]->Int32Value(getContext()).FromJust();
        std::string path = v8ToString(info[4]->ToString(getContext()).ToLocalChecked());
        
        MusicAPI::Song song = MusicAPI::addSong(title, artist, album, length, path);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(song.id));
        result->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(song.title));
        result->Set(getContext(), Nan::New("artist").ToLocalChecked(), stringToV8(song.artist));
        result->Set(getContext(), Nan::New("album").ToLocalChecked(), stringToV8(song.album));
        result->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(song.length));
        result->Set(getContext(), Nan::New("path").ToLocalChecked(), stringToV8(song.path));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Delete a song
NAN_METHOD(DeleteSong) {
    if (info.Length() < 1) {
        Nan::ThrowError("DeleteSong requires 1 argument: songId");
        return;
    }
    
    try {
        Local<Context> context = getContext();
        int songId = info[0]->Int32Value(context).FromJust();
        bool result = MusicAPI::deleteSong(songId);
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get all songs
NAN_METHOD(GetAllSongs) {
    try {
        std::vector<MusicAPI::Song> songs = MusicAPI::getAllSongs();
        Local<Array> result = Nan::New<Array>(songs.size());
        Local<Context> context = getContext();
        
        for (size_t i = 0; i < songs.size(); i++) {
            Local<Object> songObj = Nan::New<Object>();
            songObj->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(getContext(), Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(getContext(), Nan::New("album").ToLocalChecked(), stringToV8(songs[i].album));
            songObj->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(getContext(), Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(getContext(), i, songObj);
        }
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Search songs
NAN_METHOD(SearchSongs) {
    if (info.Length() < 1) {
        Nan::ThrowError("SearchSongs requires 1 argument: query");
        return;
    }
    
    try {
        std::string query = v8ToString(info[0]->ToString(getContext()).ToLocalChecked());
        std::vector<MusicAPI::Song> songs = MusicAPI::searchSongs(query);
        Local<Array> result = Nan::New<Array>(songs.size());
        
        for (size_t i = 0; i < songs.size(); i++) {
            Local<Object> songObj = Nan::New<Object>();
            songObj->Set(getContext(),Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(getContext(),Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(getContext(),Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(getContext(),Nan::New("album").ToLocalChecked(), stringToV8(songs[i].album));
            songObj->Set(getContext(),Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(getContext(),Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(getContext(),i, songObj);
        }
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Create a playlist
NAN_METHOD(CreatePlaylist) {
    if (info.Length() < 1) {
        Nan::ThrowError("CreatePlaylist requires 1 argument: title");
        return;
    }
    
    try {
        std::string title = v8ToString(info[0]->ToString(getContext()).ToLocalChecked());
        MusicAPI::Playlist playlist = MusicAPI::createPlaylist(title);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(),Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlist.id));
        result->Set(getContext(),Nan::New("title").ToLocalChecked(), stringToV8(playlist.title));
        result->Set(getContext(),Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlist.length));
        result->Set(getContext(),Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlist.numSongs));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get all playlists
NAN_METHOD(GetAllPlaylists) {
    try {
        std::vector<MusicAPI::Playlist> playlists = MusicAPI::getAllPlaylists();
        Local<Array> result = Nan::New<Array>(playlists.size());
        
        for (size_t i = 0; i < playlists.size(); i++) {
            Local<Object> playlistObj = Nan::New<Object>();
            playlistObj->Set(getContext(),Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlists[i].id));
            playlistObj->Set(getContext(),Nan::New("title").ToLocalChecked(), stringToV8(playlists[i].title));
            playlistObj->Set(getContext(),Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlists[i].length));
            playlistObj->Set(getContext(),Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlists[i].numSongs));
            result->Set(getContext(),i, playlistObj);
        }
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Add song to playlist
NAN_METHOD(AddSongToPlaylist) {
    if (info.Length() < 2) {
        Nan::ThrowError("AddSongToPlaylist requires 2 arguments: songId, playlistId");
        return;
    }
    
    try {
        int songId = info[0]->Int32Value(getContext()).FromJust();
        int playlistId = info[1]->Int32Value(getContext()).FromJust();
        bool result = MusicAPI::addSongToPlaylist(songId, playlistId);
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get songs in playlist
NAN_METHOD(GetSongsInPlaylist) {
    if (info.Length() < 1) {
        Nan::ThrowError("GetSongsInPlaylist requires 1 argument: playlistId");
        return;
    }
    
    try {
        int playlistId = info[0]->Int32Value(getContext()).FromJust();
        std::vector<MusicAPI::Song> songs = MusicAPI::getSongsInPlaylist(playlistId);
        Local<Array> result = Nan::New<Array>(songs.size());
        
        for (size_t i = 0; i < songs.size(); i++) {
            Local<Object> songObj = Nan::New<Object>();
            songObj->Set(getContext(),Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(getContext(),Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(getContext(),Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(getContext(),Nan::New("album").ToLocalChecked(), stringToV8(songs[i].album));
            songObj->Set(getContext(),Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(getContext(),Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(getContext(),i, songObj);
        }
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get database stats
NAN_METHOD(GetDatabaseStats) {
    try {
        MusicAPI::DatabaseStats stats = MusicAPI::getDatabaseStats();
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(),Nan::New("totalSongs").ToLocalChecked(), Nan::New<Number>(stats.totalSongs));
        result->Set(getContext(),Nan::New("totalPlaylists").ToLocalChecked(), Nan::New<Number>(stats.totalPlaylists));
        result->Set(getContext(),Nan::New("totalSubtitles").ToLocalChecked(), Nan::New<Number>(stats.totalSubtitles));
        result->Set(getContext(),Nan::New("totalPlaylistSongs").ToLocalChecked(), Nan::New<Number>(stats.totalPlaylistSongs));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get last error
NAN_METHOD(GetLastError) {
    std::string error = MusicAPI::getLastError();
    info.GetReturnValue().Set(stringToV8(error));
}

// Module initialization
// Update song
NAN_METHOD(UpdateSong) {
    if (info.Length() < 4) {
        Nan::ThrowError("UpdateSong requires 4 arguments: songId, title, artist, album");
        return;
    }
    
    try {
        int songId = info[0]->Int32Value(getContext()).FromJust();
        std::string title = v8ToString(info[1]->ToString(getContext()).ToLocalChecked());
        std::string artist = v8ToString(info[2]->ToString(getContext()).ToLocalChecked());
        std::string album = v8ToString(info[3]->ToString(getContext()).ToLocalChecked());
        
        MusicAPI::Song song = MusicAPI::updateSong(songId, title, artist, album);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(song.id));
        result->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(song.title));
        result->Set(getContext(), Nan::New("artist").ToLocalChecked(), stringToV8(song.artist));
        result->Set(getContext(), Nan::New("album").ToLocalChecked(), stringToV8(song.album));
        result->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(song.length));
        result->Set(getContext(), Nan::New("path").ToLocalChecked(), stringToV8(song.path));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get a specific song
NAN_METHOD(GetSong) {
    if (info.Length() < 1) {
        Nan::ThrowError("GetSong requires 1 argument: songId");
        return;
    }
    
    try {
        int songId = info[0]->Int32Value(getContext()).FromJust();
        MusicAPI::Song song = MusicAPI::getSong(songId);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(song.id));
        result->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(song.title));
        result->Set(getContext(), Nan::New("artist").ToLocalChecked(), stringToV8(song.artist));
        result->Set(getContext(), Nan::New("album").ToLocalChecked(), stringToV8(song.album));
        result->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(song.length));
        result->Set(getContext(), Nan::New("path").ToLocalChecked(), stringToV8(song.path));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Delete playlist
NAN_METHOD(DeletePlaylist) {
    if (info.Length() < 1) {
        Nan::ThrowError("DeletePlaylist requires 1 argument: playlistId");
        return;
    }
    
    try {
        int playlistId = info[0]->Int32Value(getContext()).FromJust();
        bool result = MusicAPI::deletePlaylist(playlistId);
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Update playlist
NAN_METHOD(UpdatePlaylist) {
    if (info.Length() < 2) {
        Nan::ThrowError("UpdatePlaylist requires 2 arguments: playlistId, title");
        return;
    }
    
    try {
        int playlistId = info[0]->Int32Value(getContext()).FromJust();
        std::string title = v8ToString(info[1]->ToString(getContext()).ToLocalChecked());
        
        MusicAPI::Playlist playlist = MusicAPI::updatePlaylist(playlistId, title);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlist.id));
        result->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(playlist.title));
        result->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlist.length));
        result->Set(getContext(), Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlist.numSongs));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Get a specific playlist
NAN_METHOD(GetPlaylist) {
    if (info.Length() < 1) {
        Nan::ThrowError("GetPlaylist requires 1 argument: playlistId");
        return;
    }
    
    try {
        int playlistId = info[0]->Int32Value(getContext()).FromJust();
        MusicAPI::Playlist playlist = MusicAPI::getPlaylist(playlistId);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(getContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlist.id));
        result->Set(getContext(), Nan::New("title").ToLocalChecked(), stringToV8(playlist.title));
        result->Set(getContext(), Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlist.length));
        result->Set(getContext(), Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlist.numSongs));
        
        info.GetReturnValue().Set(result);
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

// Remove song from playlist
NAN_METHOD(RemoveSongFromPlaylist) {
    if (info.Length() < 2) {
        Nan::ThrowError("RemoveSongFromPlaylist requires 2 arguments: songId, playlistId");
        return;
    }
    
    try {
        int songId = info[0]->Int32Value(getContext()).FromJust();
        int playlistId = info[1]->Int32Value(getContext()).FromJust();
        bool result = MusicAPI::removeSongFromPlaylist(songId, playlistId);
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
    } catch (const std::exception& e) {
        Nan::ThrowError(e.what());
    }
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New("initialize").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(Initialize)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("isInitialized").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(IsInitialized)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("shutdown").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(Shutdown)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("addSong").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AddSong)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("deleteSong").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(DeleteSong)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("updateSong").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(UpdateSong)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getSong").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetSong)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getAllSongs").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetAllSongs)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("searchSongs").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SearchSongs)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("createPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CreatePlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("deletePlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(DeletePlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("updatePlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(UpdatePlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getAllPlaylists").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetAllPlaylists)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("addSongToPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AddSongToPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("removeSongFromPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(RemoveSongFromPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getSongsInPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetSongsInPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getDatabaseStats").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetDatabaseStats)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getLastError").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetLastError)).ToLocalChecked());
}

NODE_MODULE(music_api, Init)
