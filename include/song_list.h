#ifndef SONG_LIST_H
#define SONG_LIST_H

#include <gtkmm/listbox.h>
#include <gtkmm/builder.h>
#include "song_item.h"

class SongList : public Gtk::ListBox {
public:
    SongList(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder>& builder);

    void append_song(
        Song* data, 
        int& like_count, 
        ListBox* songs_liked_list, 
        Gtk::Label* song_count_label
    );
};

#endif