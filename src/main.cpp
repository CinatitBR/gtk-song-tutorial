#include <vector>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/listbox.h>
#include "song_item.cpp"

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

    // Get song-list from the builder
    Gtk::ListBox* song_list;
    builder->get_widget("song-list", song_list);

    // Get the Dialog
    Gtk::Dialog* dialog;
    builder->get_widget("dialog", dialog);

    // Get the playlist box
    Gtk::EventBox* playlist_wrapper;
    builder->get_widget("playlist-wrapper", playlist_wrapper);

    // Show dialog when playlist box is clicked
    playlist_wrapper->signal_button_press_event().connect([dialog](GdkEventButton* event) -> bool {
        dialog->run();

        return true;
    });

    // Get song_count label
    Gtk::Label* song_count_label;
    builder->get_widget("song-count", song_count_label);

    // Vector containing the song data
    std::vector<Song> songs = Song::read_from_csv("songs.csv");

    // Show all the songs
    for (Song& song : songs) {
        // Create builer representing the song box
        auto song_builder = Gtk::Builder::create_from_file("song-box.xml");

        SongItem* song_item;
        song_builder->get_widget_derived("song-box", song_item, &song);

        song_item->signal_like_pressed().connect([&like_count, song_count_label](SongItem* song_item_target) -> void {
            std::string heart_solid = "./images/icons/heart-solid-icon.png";
            std::string heart = "./images/icons/heart-icon.png";

            if (song_item_target->data->is_liked) {
                song_item_target->like_icon->set(heart);
                like_count--;
            }
            else {
                song_item_target->like_icon->set(heart_solid);
                like_count++;
            }

            song_item_target->data->is_liked = !song_item_target->data->is_liked;
            song_count_label->set_label(std::to_string(like_count) + " songs");
        });

        // Append song box to the list
        song_list->append(*song_item);
    }

    app->run(*window);
}