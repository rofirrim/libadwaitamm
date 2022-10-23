#pragma once

#include <glibmm/extraclassinit.h>
#include <libadwaitamm.h>

namespace Adw {

class DemoPreferencesWindow : public Glib::ExtraClassInit,
                              public Adw::PreferencesWindow {
public:
  DemoPreferencesWindow();
  virtual ~DemoPreferencesWindow();

  static GType get_type() {
    // Let's cache once the type does exist.
    if (!gtype)
      gtype = g_type_from_name("gtkmm__CustomObject_AdwDemoPreferencesWindow");
    return gtype;
  }

private:
  static void class_init(void *g_class, void *class_data);
  static void instance_init(GTypeInstance *instance, void *g_class);

  Gtk::Widget *subpage1;
  Gtk::Widget *subpage2;

  void subpage1_activated();
  void subpage2_activated();
  void return_to_preferences();
  void toast_show();

  static GType gtype;
};
} // namespace Adw
