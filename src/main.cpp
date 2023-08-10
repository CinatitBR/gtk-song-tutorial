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
    Gtk::Image* like_icon;
    Gtk::EventBox* like_wrapper;

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
        song_builder->get_widget("like-icon", like_icon);
        song_builder->get_widget("like-wrapper", like_wrapper);

        // Set song_box cover
        std::string cover_path = "./images/covers/" + std::to_string(song.id) + ".png";

        std::string css_data = "box {"
                          "   background-image: url('" + cover_path + "');"
                          "}";

        auto styleContext = cover->get_style_context();
        auto cssProvider = Gtk::CssProvider::create();
        cssProvider->load_from_data(css_data);
        styleContext->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

        title->set_label(song.title);
        artist->set_label(song.artist);

        like_wrapper->signal_button_press_event().connect([&song, like_icon](GdkEventButton* event) -> bool {
            std::string heart_solid = "./images/icons/heart-solid-icon.png";
            std::string heart = "./images/icons/heart-icon.png";

            if (song.is_liked) {
                like_icon->set(heart);
            }
            else {
                like_icon->set(heart_solid);
            }

            song.is_liked = !song.is_liked;

            return true;
        });

        // Append song box to the list
        song_list->append(*song_box);
    }

    app->run(*window);
}