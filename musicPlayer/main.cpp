// Made by: 0xDarkStar
#include "sqlCommands.h"
#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    int output = 0;
    // output = create_table();
    output = populate_songs_table(1, "Title", "Artist", false, 255);
    output = delete_song(1);
    return output;
}
