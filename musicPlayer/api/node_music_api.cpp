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
    if (info.Length() < 4) {
        Nan::ThrowError("AddSong requires 4 arguments: title, artist, length, path");
        return;
    }
    
    try {
        Local<Context> context = getContext();
        std::string title = v8ToString(info[0]->ToString(context).ToLocalChecked());
        std::string artist = v8ToString(info[1]->ToString(context).ToLocalChecked());
        int length = info[2]->Int32Value(context).FromJust();
        std::string path = v8ToString(info[3]->ToString(context).ToLocalChecked());
        
        MusicAPI::Song song = MusicAPI::addSong(title, artist, length, path);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(context, Nan::New("id").ToLocalChecked(), Nan::New<Number>(song.id));
        result->Set(context, Nan::New("title").ToLocalChecked(), stringToV8(song.title));
        result->Set(context, Nan::New("artist").ToLocalChecked(), stringToV8(song.artist));
        result->Set(context, Nan::New("length").ToLocalChecked(), Nan::New<Number>(song.length));
        result->Set(context, Nan::New("path").ToLocalChecked(), stringToV8(song.path));
        
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
            songObj->Set(context, Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(context, Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(context, Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(context, Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(context, Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(context, i, songObj);
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
        std::string query = v8ToString(info[0]->ToString());
        std::vector<MusicAPI::Song> songs = MusicAPI::searchSongs(query);
        Local<Array> result = Nan::New<Array>(songs.size());
        
        for (size_t i = 0; i < songs.size(); i++) {
            Local<Object> songObj = Nan::New<Object>();
            songObj->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(i, songObj);
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
        std::string title = v8ToString(info[0]->ToString());
        MusicAPI::Playlist playlist = MusicAPI::createPlaylist(title);
        
        Local<Object> result = Nan::New<Object>();
        result->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlist.id));
        result->Set(Nan::New("title").ToLocalChecked(), stringToV8(playlist.title));
        result->Set(Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlist.length));
        result->Set(Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlist.numSongs));
        
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
            playlistObj->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(playlists[i].id));
            playlistObj->Set(Nan::New("title").ToLocalChecked(), stringToV8(playlists[i].title));
            playlistObj->Set(Nan::New("length").ToLocalChecked(), Nan::New<Number>(playlists[i].length));
            playlistObj->Set(Nan::New("numSongs").ToLocalChecked(), Nan::New<Number>(playlists[i].numSongs));
            result->Set(i, playlistObj);
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
        int songId = info[0]->Int32Value();
        int playlistId = info[1]->Int32Value();
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
        int playlistId = info[0]->Int32Value();
        std::vector<MusicAPI::Song> songs = MusicAPI::getSongsInPlaylist(playlistId);
        Local<Array> result = Nan::New<Array>(songs.size());
        
        for (size_t i = 0; i < songs.size(); i++) {
            Local<Object> songObj = Nan::New<Object>();
            songObj->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(songs[i].id));
            songObj->Set(Nan::New("title").ToLocalChecked(), stringToV8(songs[i].title));
            songObj->Set(Nan::New("artist").ToLocalChecked(), stringToV8(songs[i].artist));
            songObj->Set(Nan::New("length").ToLocalChecked(), Nan::New<Number>(songs[i].length));
            songObj->Set(Nan::New("path").ToLocalChecked(), stringToV8(songs[i].path));
            result->Set(i, songObj);
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
        result->Set(Nan::New("totalSongs").ToLocalChecked(), Nan::New<Number>(stats.totalSongs));
        result->Set(Nan::New("totalPlaylists").ToLocalChecked(), Nan::New<Number>(stats.totalPlaylists));
        result->Set(Nan::New("totalSubtitles").ToLocalChecked(), Nan::New<Number>(stats.totalSubtitles));
        result->Set(Nan::New("totalPlaylistSongs").ToLocalChecked(), Nan::New<Number>(stats.totalPlaylistSongs));
        
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
    
    Nan::Set(target, Nan::New("getAllSongs").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetAllSongs)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("searchSongs").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(SearchSongs)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("createPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(CreatePlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getAllPlaylists").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetAllPlaylists)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("addSongToPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(AddSongToPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getSongsInPlaylist").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetSongsInPlaylist)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getDatabaseStats").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetDatabaseStats)).ToLocalChecked());
    
    Nan::Set(target, Nan::New("getLastError").ToLocalChecked(),
             Nan::GetFunction(Nan::New<FunctionTemplate>(GetLastError)).ToLocalChecked());
}

NODE_MODULE(music_api, Init)
