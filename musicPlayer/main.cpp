// Made by: 0xDarkStar
#include "sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = sql::create_tables();
    // output = sql::add_song_to_table(0, "Title", "Artist", false, 255);
    // output = sql::delete_song(1);
    // for (int i = 0; i < 10; i++) {
    //     sql::add_song_to_table(i, "Title", "Artist", false, 255);
    // }
    // for (int i = 0; i < 10; i++) {
    //     sql::delete_song(i);
    // }
    // output = sql::read_table("songs");
    // sql::create_playlist(0, "Test Playlist", 355, 1);
    // sql::add_song_to_playlist(0, 0);
    output = sql::get_table_length("songs");
    return output;
}
