// Made by: 0xDarkStar
#include "sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = sql::create_tables();
    // output = sql::delete_song(0);
    // output = sql::add_song(1, "Testing2", "Tester", false, 355);
    // output = sql::add_subtitles(0, 0, "en");
    // for (int i = 0; i < 10; i++) {
    //     sql::add_song(i, "Title", "Artist", false, 255);
    // }
    // for (int i = 0; i < 10; i++) {
    //     sql::delete_song(i);
    // }
    // sql::create_playlist(0, "Test Playlist", 355, 1);
    // sql::add_song_to_playlist(0, 0);
    output = sql::read_table("songs");
    // output = sql::get_table_length("songs");
    output = sql::read_table_item("songs", 0, "Has_subs");
    return output;
}
