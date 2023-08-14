#include <gtkmm/builder.h>
#include <gtkmm/image.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include "song.cpp"

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

    SongItem(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder>& builder, Song* data) 
        : Gtk::Box(cobject)
    {

        this->data = data;
        cssProvider = Gtk::CssProvider::create();

        builder->get_widget("song-box", song_box);
        builder->get_widget("cover-wrapper", cover);
        builder->get_widget("title", title);
        builder->get_widget("artist", artist);
        builder->get_widget("like-icon", like_icon);
        builder->get_widget("like-wrapper", like_wrapper);

        std::string cover_path = "./images/covers/" + std::to_string(data->id) + ".png";

        title->set_label(data->title);
        artist->set_label(data->artist);
        this->set_cover(cover_path);

        like_wrapper->signal_button_press_event().connect([this](GdkEventButton* event) -> bool {
            this->signal_like_pressed_.emit(this);

            return true;
        });
    }

    // Update song cover
    void set_cover(std::string cover_path) {
        auto styleContext = cover->get_style_context();

        std::string css_data = "box {"
                          "   background-image: url('" + cover_path + "');"
                          "}";

        cssProvider->load_from_data(css_data);
        styleContext->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    // Signals
    sigc::signal<void, SongItem*> signal_like_pressed() {
        return signal_like_pressed_;
    }

private:
    sigc::signal<void, SongItem*> signal_like_pressed_;
};