#pragma once

#include "templatewidget.h"
#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>

namespace Adw {

class DemoPageAvatar : public Gtk::TemplateWidget<DemoPageAvatar, Adw::Bin> {
  friend CppClassType;

protected:
  DemoPageAvatar(GtkWidget *obj);

private:
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static const char class_name[];
  void init_widget(Gtk::TemplateWidgetInit &i);

  Adw::Avatar *avatar;
  Adw::EntryRow *text;
  Gtk::Label *file_chooser_label;
  Gtk::ListBox *contacts;

  static Glib::ustring create_random_name();

  void populate_contacts();
  void avatar_open();
  void avatar_remove();
  void avatar_save();
};
}
