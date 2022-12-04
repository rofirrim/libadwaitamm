#include "adw-demo-page-avatar.h"

#include <glib/gi18n.h>

#include <array>

namespace Adw {

const char DemoPageAvatar::class_name[] = "AdwDemoPageAvatar";

void DemoPageAvatar::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/avatar/adw-demo-page-avatar.ui");

  s.bind_widget("avatar");
  s.bind_widget("text");
  s.bind_widget("file_chooser_label");
  s.bind_widget("contacts");

  s.install_action("avatar.open",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageAvatar::avatar_open>());
  s.install_action("avatar.remove",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageAvatar::avatar_remove>());
  s.install_action("avatar.save",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageAvatar::avatar_save>());
}

void DemoPageAvatar::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();

  i.bind_widget(avatar, "avatar");
  i.bind_widget(text, "text");
  i.bind_widget(file_chooser_label, "file_chooser_label");
  i.bind_widget(contacts, "contacts");

  Glib::ustring name = create_random_name();

  text->set_text(name);

  populate_contacts();
  avatar_remove();
}

void DemoPageAvatar::populate_contacts() {
  for (int i = 0; i < 30; i++) {
    Glib::ustring name = create_random_name();
    Adw::ActionRow* contact = Gtk::make_managed<Adw::ActionRow>();
    Gtk::Widget *new_avatar = Gtk::make_managed<Adw::Avatar>(40, name, true);

    avatar->set_margin_top(12);
    avatar->set_margin_bottom(12);

    contact->set_title(name);
    contact->add_prefix(*new_avatar);

    contacts->append(*contact);
  }
}

void DemoPageAvatar::avatar_open() {
  Gtk::Root *root = get_root();
  auto chooser = Gtk::FileChooserNative::create(
      _("Select an Avatar"), dynamic_cast<Gtk::Window &>(*root),
      Gtk::FileChooser::Action::OPEN, _("_Select"), _("_Cancel"));

  chooser->set_modal(true);

  chooser->signal_response().connect([chooser, this](int response_id) mutable {
    switch (response_id) {
    case Gtk::ResponseType::ACCEPT: {
      auto file = chooser->get_file();
      auto info = file->query_info(G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME,
                                   Gio::FileQueryInfoFlags::NONE);
      if (info)
        file_chooser_label->set_label(info->get_display_name());

      action_set_enabled("avatar.remove", true);

      Glib::RefPtr<Gdk::Texture> texture;
      try {
        texture = Gdk::Texture::create_from_file(file);
      } catch (...) {
        g_critical("Failed to create texture from file: %s",
                   info->get_display_name().c_str());
      }

      avatar->set_custom_image(texture);
      break;
    }
    default:
      break;
    }

    chooser.reset();
  });

  chooser->show();
}

void DemoPageAvatar::avatar_remove() {
  file_chooser_label->set_label(_("(None)"));
  action_set_enabled("avatar.remove", false);
  avatar->set_custom_image(nullptr);
}

void DemoPageAvatar::avatar_save() {
  Gtk::Root *root = get_root();
  auto chooser = Gtk::FileChooserNative::create(
      _("Save Avatar"), dynamic_cast<Gtk::Window &>(*root),
      Gtk::FileChooser::Action::SAVE, _("_Save"), _("_Cancel"));

  chooser->set_modal(true);

  chooser->signal_response().connect([chooser, this](int response_id) mutable {
    switch (response_id) {
    case Gtk::ResponseType::ACCEPT: {
      auto file = chooser->get_file();
      auto texture = avatar->draw_to_texture(get_scale_factor());
      texture->save_to_png(file->get_path());
      break;
    }
    default:
      break;
    }
  });

  chooser->show();
}

Glib::ustring DemoPageAvatar::create_random_name(void) {
  static std::array first_names = {
      "Adam",    "Adrian",    "Anna",     "Charlotte", "Frédérique", "Ilaria",
      "Jakub",   "Jennyfer",  "Julia",    "Justin",    "Mario",      "Miriam",
      "Mohamed", "Nourimane", "Owen",     "Peter",     "Petra",      "Rachid",
      "Rebecca", "Sarah",     "Thibault", "Wolfgang",
  };
  static std::array last_names = {
      "Bailey", "Berat",    "Chen",  "Farquharson", "Ferber",
      "Franco", "Galinier", "Han",   "Lawrence",    "Lepied",
      "Lopez",  "Mariotti", "Rossi", "Urasawa",     "Zwickelman",
  };

  Glib::ustring result = first_names[g_random_int_range(0, first_names.size())];
  result += " ";
  result += last_names[g_random_int_range(0, last_names.size())];

  return result;
}
} // namespace Adw
