/*
Made By: 0xDarkStar
This is a simple music player that uses SQLite3 to keep track of the songs, playlists, and subtitles.
This file specifically tests the sqlCommands.h file.
*/
#include "../backend/sqlSetup.h"
#include "../backend/sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = sql::create_tables();
    // output = sql::add_song(1, "Testing", "Tester", 355, "Fake/Path/To/Song.mp3");
    output = sql::delete_song(0);
    // output = sql::add_subtitles(0, 0, "fr");
    // output = sql::delete_subtitles(0, 0);
    // for (int i = 0; i < 10; i++) {
    //     sql::add_song(i, "Title", "Artist", 255);
    // }
    // for (int i = 0; i < 10; i++) {
    //     sql::delete_song(i);
    // }
    // sql::create_playlist(0, "Test Playlist", 355, 1);
    // sql::add_song_to_playlist(0, 0);
    output = sql::read_table("song");
    output = sql::read_table("subtitle");
    output = sql::read_table("playlist");
    output = sql::read_table("playlist_song");
    // output = sql::get_table_length("song");
    // output = sql::read_table_item("song", 0, "Title");
    return output;
}
