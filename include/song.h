#ifndef SONG_H
#define SONG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Song {
public:
    // ### ATTRIBUTES ###
    int id;
    std::string title;    
    std::string artist;
    std::string album;
    std::string genre;
    int year;
    int danceability;
    int feeling;
    int noise;
    double median;
    bool is_liked;

    // ### METHODS ###
    Song(
        int id, const std::string& artist, const std::string& title, const std::string& album, 
        const std::string& genre, int year, int danceability, int feeling, int 
        noise, double median
    );

    Song();

    // Print song details
    void print() const;

    // Parse csv file and create a vector of songs.
    static std::vector<Song> read_from_csv(const std::string& filename);
};

#endif