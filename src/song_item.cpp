#include "../include/song_item.h"

SongItem::SongItem(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder>& builder, Song* data) 
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

    // Check if widget is liked and update like icon
    if (data->is_liked) {
        std::string heart_solid = "./images/icons/heart-solid-icon.png";
        like_icon->set(heart_solid);
    }

    std::string cover_path = "./images/covers/" + std::to_string(data->id) + ".png";

    title->set_label(data->title);
    artist->set_label(data->artist);
    this->set_cover(cover_path);

    like_wrapper->signal_button_press_event().connect([this](GdkEventButton* event) -> bool {
        this->signal_like_pressed_.emit(this);

        return true;
    });
}

void SongItem::set_cover(std::string cover_path) {
    auto styleContext = cover->get_style_context();

    std::string css_data = "box {"
                        "   background-image: url('" + cover_path + "');"
                        "}";

    cssProvider->load_from_data(css_data);
    styleContext->add_provider(cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void SongItem::toggle_like() {
    std::string heart_solid = "./images/icons/heart-solid-icon.png";
    std::string heart = "./images/icons/heart-icon.png";

    std::cout << "Before: " << data->is_liked << std::endl;

    // Update like state
    data->is_liked = !data->is_liked;

    std::cout << "Then: " << data->is_liked << std::endl;

    // Update like icon
    if (data->is_liked) {
        like_icon->set(heart_solid);
    }
    else {
        like_icon->set(heart);
    }
}

sigc::signal<void, SongItem*> SongItem::signal_like_pressed() {
    return signal_like_pressed_;
}