#include <vector>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/listbox.h>
#include "../include/song_list.h"
#include "../include/song_item.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);
    auto builder = Gtk::Builder::create_from_file("gui.glade");
    int like_count = 0;

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
    window->maximize();

    // Get song-list from the builder
    SongList* song_list;
    builder->get_widget_derived("song-list", song_list);

    // Get the Dialog
    Gtk::Dialog* dialog;
    builder->get_widget("dialog", dialog);
    dialog->set_default_size(700, 700);

    // Get the playlist box
    Gtk::EventBox* playlist_wrapper;
    builder->get_widget("playlist-wrapper", playlist_wrapper);

    // Show dialog when playlist box is clicked
    playlist_wrapper->signal_button_press_event().connect([dialog](GdkEventButton* event) -> bool {
        dialog->run();

        return true;
    });

    Gtk::ListBox* songs_liked_list;
    builder->get_widget("songs-liked-list", songs_liked_list);

    // Get song_count label
    Gtk::Label* song_count_label;
    builder->get_widget("song-count", song_count_label);

    // Vector containing the song data
    std::vector<Song> songs = Song::read_from_csv("songs.csv");

    // Show all the songs
    for (Song& song : songs) {

        song_list->append_song(
            &song, 
            like_count,
            songs_liked_list,
            song_count_label
        );

    }

    app->run(*window);
}