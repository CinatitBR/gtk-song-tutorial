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

    Gtk::ListBox* songs_liked_list;
    builder->get_widget("songs-liked-list", songs_liked_list);

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

        song_item->signal_like_pressed().connect([&like_count, song_count_label, songs_liked_list](SongItem* song_item_target) -> void {
            std::string heart_solid = "./images/icons/heart-solid-icon.png";
            std::string heart = "./images/icons/heart-icon.png";

            Song* data = song_item_target->data;
            song_item_target->data->is_liked = !data->is_liked;

            // Song is being liked
            if (data->is_liked) {
                song_item_target->like_icon->set(heart_solid);
                like_count++;

                SongItem* new_song_item;
                auto song_builder = Gtk::Builder::create_from_file("song-box.xml");
                song_builder->get_widget_derived("song-box", new_song_item, data);

                new_song_item->like_icon->set(heart_solid);
                new_song_item->data->is_liked = true;

                // Add like_pressed handler to new_sonng_item
                new_song_item->signal_like_pressed().connect([song_item_target](SongItem* new_music_item) -> void {
                    // Emit unlike event from the initial song_item.
                    song_item_target->signal_like_pressed().emit(song_item_target);
                });

                songs_liked_list->append(*new_song_item);
            }
            // Song is being unliked
            else {
                song_item_target->like_icon->set(heart);
                like_count--;

                // ### Remove unliked song from songs_liked_list ###
                int id = data->id;

                // Get song_liked_list children.
                std::vector<Gtk::Widget *> children = songs_liked_list->get_children();

                for (auto child : children) {
                   auto row = dynamic_cast<Gtk::ListBoxRow*>(child);
                   auto song_child = dynamic_cast<SongItem*>(row->get_child());

                    // Row could not be casted to MusicItem.
                    if (song_child == nullptr) {
                        continue;
                    }

                    int child_id = song_child->data->id;

                    // Remove child with the requested id. 
                    if (child_id == id) {
                        songs_liked_list->remove(*child);
                    }
                }
            }

            // song_item_target->data->is_liked = !song_item_target->data->is_liked;
            song_count_label->set_label(std::to_string(like_count) + " songs");
        });

        // Append song box to the list
        song_list->append(*song_item);
    }

    app->run(*window);
}