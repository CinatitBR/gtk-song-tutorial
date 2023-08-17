#ifndef SONG_ITEM_H
#define SONG_ITEM_H

#include <gtkmm/builder.h>
#include <gtkmm/image.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include "song.h"

class SongItem : public Gtk::Box {
public:
    Song* data;
    Glib::RefPtr<Gtk::CssProvider> cssProvider;

    Gtk::Box* song_box;
    Gtk::Box* cover;
    Gtk::Label* title;
    Gtk::Label* artist;
    Gtk::Image* like_icon;
    Gtk::EventBox* like_wrapper;

    SongItem(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder>& builder, Song* data);

    // Update song cover
    void set_cover(std::string cover_path);

    void toggle_like();

    // Signals
    sigc::signal<void, SongItem*> signal_like_pressed();

private:
    sigc::signal<void, SongItem*> signal_like_pressed_;
};

#endif