#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define ADW_TYPE_DEMO_WINDOW (adw_demo_window_get_type())

G_DECLARE_FINAL_TYPE (AdwDemoWindow, adw_demo_window, ADW, DEMO_WINDOW, AdwApplicationWindow)

AdwDemoWindow *adw_demo_window_new (GtkApplication *application);

G_END_DECLS

#include <glibmm/extraclassinit.h>
#include <libadwaitamm.h>

namespace Adw {

class DemoWindow : public Glib::ExtraClassInit, public Adw::ApplicationWindow {

public:
  DemoWindow(const Glib::RefPtr<Gtk::Application>& application);
  virtual ~DemoWindow();

  static GType get_type() {
    // Let's cache once the type does exist.
    if (!gtype)
      gtype = g_type_from_name("gtkmm__CustomObject_AdwDemoWindow");
    return gtype;
  }

private:
  static void class_init(void *g_class, void *class_data);
  static void instance_init(GTypeInstance *instance, void *g_class);

  void color_scheme_button_clicked_cb();
  void notify_visible_child_cb();
  void back_clicked_cb();
  void leaflet_back_clicked_cb();
  void leaflet_next_page_cb();
  void toast_undo_cb();
  void notify_system_supports_color_schemes_cb();

  Adw::Leaflet *main_leaflet = nullptr;
  Adw::Leaflet *subpage_leaflet = nullptr;
  Gtk::Widget *color_scheme_button = nullptr;

  static GType gtype;
};
} // namespace Adw
