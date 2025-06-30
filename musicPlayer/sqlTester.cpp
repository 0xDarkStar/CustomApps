/*
Made By: 0xDarkStar
This is a simple music player that uses SQLite3 to keep track of the songs, playlists, and subtitles.
This file specifically tests the sqlCommands.h file.
*/
#include "sqlSetup.h"
#include "sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = sql::create_tables();
    output = sql::add_song(0, "Testing", "Tester", 355, "Fake/Path/To/Song.mp3");
    // output = sql::delete_song(0);
    output = sql::add_subtitles(0, 0, "fr");
    // output = sql::delete_subtitles(0, 0);
    // for (int i = 0; i < 10; i++) {
    //     sql::add_song(i, "Title", "Artist", 255);
    // }
    // for (int i = 0; i < 10; i++) {
    //     sql::delete_song(i);
    // }
    // sql::create_playlist(0, "Test Playlist", 355, 1);
    // sql::add_song_to_playlist(0, 0);
    output = sql::read_table("songs");
    output = sql::read_table("songSubs");
    // output = sql::get_table_length("songs");
    // output = sql::read_table_item("songs", 0, "Title");
    return output;
}
