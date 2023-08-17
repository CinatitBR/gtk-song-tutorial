#include "../include/song_list.h"

SongList::SongList(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::ListBox(cobject)
{}

void SongList::append_song(
    Song* data, 
    int& like_count, 
    ListBox* songs_liked_list, 
    Gtk::Label* song_count_label
) 
{
    // Create song_item with the requested data
    SongItem* song_item;
    auto song_builder = Gtk::Builder::create_from_file("song-box.xml");
    song_builder->get_widget_derived("song-box", song_item, data);

    song_item->signal_like_pressed().connect([&like_count, song_count_label, songs_liked_list](SongItem* song_item_target) -> void {
        Song* data = song_item_target->data;
        song_item_target->toggle_like();

        // Song is being liked
        if (data->is_liked) {
            like_count++;

            SongItem* new_song_item;
            auto song_builder = Gtk::Builder::create_from_file("song-box.xml");
            song_builder->get_widget_derived("song-box", new_song_item, data);

            // Add like_pressed handler to new_sonng_item
            new_song_item->signal_like_pressed().connect([song_item_target](SongItem* new_music_item) -> void {
                // Emit unlike event from the initial song_item.
                song_item_target->signal_like_pressed().emit(song_item_target);
            });

            songs_liked_list->append(*new_song_item);
        }
        // Song is being unliked
        else {
            // song_item_target->like_icon->set(heart);
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

        song_count_label->set_label(std::to_string(like_count) + " songs");
    });

    this->append(*song_item);
}

