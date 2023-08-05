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
    ) {
        this->id = id;
        this->artist = artist;
        this->title = title;
        this->album = album; 
        this->genre = genre;
        this->year = year;
        this->danceability = danceability;
        this->feeling = feeling;
        this->noise = noise;
        this->median = median;
        this->is_liked = false;
    }

    Song() {
        id = 0;
    }

    void toggle_like() {
        this->is_liked = !this->is_liked;
    }

    // Print song details
    void print() {
        std::cout << "Id: " << id << std::endl;
        std::cout << "Artist: " << artist << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Album: " << album << std::endl;
        std::cout << "Genre: " << genre << std::endl;
        std::cout << "Year: " << year << std::endl;
        std::cout << "Danceability: " << danceability << std::endl;
        std::cout << "Feeling: " << feeling << std::endl;
        std::cout << "Noise: " << noise << std::endl;
        std::cout << "Median: " << median << std::endl;
    }

    // Parse csv file and create a vector of songs.
    std::vector<Song> read_from_csv(const std::string& filename) {
        std::ifstream arquivo(filename);
        std::string linha;
        std::vector<Song> musicas;

        if (!arquivo.is_open()) {
            std::cout << "Erro ao abrir o arquivo." << std::endl;
            return musicas;
        }

        std::getline(arquivo, linha);

        while (std::getline(arquivo, linha)) {
            std::stringstream linhaStream(linha);
            std::string dado;
            std::vector<std::string> valores;

            while (std::getline(linhaStream, dado, ',')) {
                valores.push_back(dado);
            }

            const int NUM_VALORES_ESPERADOS = 10;
            if (valores.size() != NUM_VALORES_ESPERADOS) {
                std::cout << "Erro: linha inválida: " << linha << std::endl;
                continue;
            }

            int id;
            try {
                id = std::stoi(valores[0]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter id: " << e.what() << std::endl;
                continue;
            }

            std::string artist = valores[1];
            std::string title = valores[2];
            std::string album = valores[3];
            std::string genre = valores[4];

            int year;
            try {
                year = std::stoi(valores[5]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter year: " << e.what() << std::endl;
                continue;
            }

            int danceability;
            try {
                danceability = std::stoi(valores[6]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter danceability: " << e.what() << std::endl;
                continue;
            }

            int feeling;
            try {
                feeling = std::stoi(valores[7]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter feeling: " << e.what() << std::endl;
                continue;
            }

            int noise;
            try {
                noise = std::stoi(valores[8]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter noise: " << e.what() << std::endl;
                continue;
            }

            double median;
            try {
                median = std::stod(valores[9]);
            } catch (const std::invalid_argument& e) {
                std::cout << "Erro ao converter median: " << e.what() << std::endl;
                continue;
            }

            Song musica(id, artist, title, album, genre, year, danceability, feeling, noise, median);
            musicas.push_back(musica);
        }

        return musicas;
    }
};