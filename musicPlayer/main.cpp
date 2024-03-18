// Made by: 0xDarkStar
#include "sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = create_tables();
    // output = add_song_to_table(2, "Title", "Artist", false, 255);
    // output = delete_song(1);
    // for (int i = 0; i < 10; i++) {
    //     add_song_to_table(i, "Title", "Artist", false, 255);
    // }
    // for (int i = 0; i < 10; i++) {
    //     delete_song(i);
    // }
    // output = read_table("songs");
    output = get_table_length("songs");
    return output;
}
