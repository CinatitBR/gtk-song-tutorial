#include <gtkmm.h>
#include <vector>
#include "song.cpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);
    auto builder = Gtk::Builder::create_from_file("gui.glade");

    // Load the CSS file
    auto cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_path("style.css");

    // Apply the CSS styles to the default screen
    auto screen = Gdk::Screen::get_default();
    auto styleContext = Gtk::StyleContext::create();
    styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Get window
    Gtk::Window* window;
    builder->get_widget("window", window);

    // Get song-list from the builder
    Gtk::ListBox* song_list;
    builder->get_widget("song-list", song_list);

    // Create song box
    Gtk::Box* song_box;
    Gtk::Box* cover;
    Gtk::Label* title;
    Gtk::Label* artist;

    // Vector containing the song data
    std::vector<Song> songs = Song::read_from_csv("songs.csv");

    // Show all the songs
    for (Song& song : songs) {
        // Create builer representing the song box
        auto song_builder = Gtk::Builder::create_from_file("song-box.xml");

        song_builder->get_widget("song-box", song_box);
        song_builder->get_widget("cover-wrapper", cover);
        song_builder->get_widget("title", title);
        song_builder->get_widget("artist", artist);

        title->set_label(song.title);
        artist->set_label(song.artist);

        // Append song box to the list
        song_list->append(*song_box);
    }

    app->run(*window);
}